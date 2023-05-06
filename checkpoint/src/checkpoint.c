/**
 * Main file for the checkpoint's program.
 * @file checkpoint.c
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#include "checkpoint.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <dirent.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <math.h>
#include "../lib/WiringPi/wiringPi/wiringPi.h"

/* ------------------------------------------------------------------------- */
/*                             Global variables                              */
/* ------------------------------------------------------------------------- */

// Mutexes
pthread_mutex_t mut_display = PTHREAD_MUTEX_INITIALIZER; // Mutex for the display

// Variables to interact with the display
ssd1306_i2c_t* p_display; // Display
ssd1306_framebuffer_t* p_framebuffer; // Framebuffer for the display
display_routine_arg_t display_routine_arg; // Argument for the display routine

// Variables to interact with the Kinéis module
int fd_kineis_port; // File descriptor for the Kinéis port

/* ------------------------------------------------------------------------- */
/*                               Main function                               */
/* ------------------------------------------------------------------------- */

int main(int argc, char* argv[])
{
    pthread_t th_button, th_rfid, th_send_data, th_usb_key;

    if (!init()) exit(EXIT_FAILURE);

    // Create threads
    pthread_create(&th_button, NULL, button_routine, NULL);
    pthread_create(&th_rfid, NULL, rfid_routine, NULL);
    pthread_create(&th_send_data, NULL, send_data_routine, NULL);
    pthread_create(&th_usb_key, NULL, usb_key_routine, NULL);

    // Wait for threads to finish
    pthread_join(th_button, NULL);
    pthread_join(th_rfid, NULL);
    pthread_join(th_send_data, NULL);
    pthread_join(th_usb_key, NULL);

    exit(EXIT_SUCCESS);
}

/* ------------------------------------------------------------------------- */
/*                         Thread routine functions                          */
/* ------------------------------------------------------------------------- */

void* button_routine(void* arg)
{
    pthread_exit(NULL);
}

void* rfid_routine(void* arg)
{
    rfid_read_t rfid_read;
    pthread_t th_display;

    while (1) {
        if (!wait_rfid_read(&rfid_read)) {
            printf("Problem when reading RFID card.\n");
            pthread_create(&th_display, NULL, display_error_routine, &display_routine_arg);
            continue;
        }

        printf("Received code : %d\n", rfid_read.code);
        printf("Admin : %s\n", rfid_read.admin ? "true" : "false");
        if (!rfid_read.admin) {
            printf("Received first name : %s\n", rfid_read.first_name);
            printf("Received last name : %s\n", rfid_read.last_name);
        }

        // If admin card, resolve emergency
        if (rfid_read.admin) {
            if (is_emergency_active()) {
                bool created = create_emergency_solved_log();
                pthread_create(&th_display, NULL, created ? display_ok_routine : display_error_routine, &display_routine_arg);
            }
            else pthread_create(&th_display, NULL, display_ok_routine, &display_routine_arg);
            continue;
        }

        // Log the participant passage
        char log_description[MAX_LENGTH_LOG_DESCRIPTION];
        strcpy(log_description, rfid_read.first_name);
        strcat(log_description, " ");
        strcat(log_description, rfid_read.last_name);
        bool created = create_log(rfid_read.code, log_description);
        if (!created) {
            printf("Error when logging code:'%d' description:'%s'", rfid_read.code, log_description);
            pthread_create(&th_display, NULL, display_error_routine, &display_routine_arg);
            continue;
        }
        pthread_create(&th_display, NULL, display_ok_routine, &display_routine_arg);

        // Print the last participants passages
        // TODO
    }

    pthread_exit(NULL);
}

void* send_data_routine(void* arg)
{
    char data_to_send[MAX_KINEIS_DATA_SIZE];
    char read_buffer[MAX_KINEIS_DATA_SIZE];
    while (1) {
        retrieve_data_to_send(data_to_send, MAX_KINEIS_DATA_SIZE);
        printf("Data to send: %s\n", data_to_send);
        kineis_send_data(fd_kineis_port, data_to_send, strlen(data_to_send));
        kineis_read_data(fd_kineis_port, read_buffer);

        sleep(SEND_DATA_PERIOD_S);
    }

    pthread_exit(NULL);
}

void* usb_key_routine(void* arg)
{
    usb_event_t usb_event;
    pthread_t th_display;

    while (1) {
        if (!wait_usb_event(&usb_event)) {
            printf("Problem with USB event.\n");
            pthread_create(&th_display, NULL, display_error_routine, &display_routine_arg);
            continue;
        }

        // A usb device event has occured
        switch (usb_event.event) {
        case USB_EVENT_CONNECT:
            printf("USB device connected: %s\n", usb_event.device);

            // Mount device to defined path (first partition)
            char partition[MAX_DEVICE_NAME_LENGTH + 2];
            strcpy(partition, usb_event.device);
            strcat(partition, "1");
            if (mount(partition, USB_MOUNT_PATH, "vfat", 0, NULL) != 0) {
                perror("mount failed");
                pthread_create(&th_display, NULL, display_error_routine, &display_routine_arg);
                break;
            }
            printf("%s was mounted successfully.\n", partition);

            // Copy local log file to USB device
            if (!copy_log_file_to_usb())
                pthread_create(&th_display, NULL, display_error_routine, &display_routine_arg);

            // Unmount device
            if (umount(USB_MOUNT_PATH) != 0) perror("umount failed");
            else printf("%s was unmounted successfully.\n", partition);

            pthread_create(&th_display, NULL, display_ok_routine, &display_routine_arg);
            break;
        case USB_EVENT_DISCONNECT:
            printf("USB device disconnected: %s\n", usb_event.device);
            break;
        }
    }

    pthread_exit(NULL);
}

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

bool init(void)
{
    // Create usb mount path directory if it doesn't exist
    opendir(USB_MOUNT_PATH);
    if (errno == ENOENT && mkdir(USB_MOUNT_PATH, 0777) != 0) {
        perror("Can't create usb mount path directory");
        return false;
    }
    printf("USB mount path OK.\n");

    // Create log directory if it doesn't exist
    opendir(LOG_DIRECTORY);
    if (errno == ENOENT && mkdir(LOG_DIRECTORY, 0777) != 0) {
        perror("Can't create log directory");
        return false;
    }
    printf("Log directory OK.\n");

    // Create log file if it doesn't exist
    FILE* log_file = fopen(LOG_FILE_PATH, "a");
    if (log_file == NULL) {
        perror("Can't create log file");
        return false;
    }
    fclose(log_file);
    printf("Log file OK.\n");

    // Init display
    p_display = ssd1306_i2c_open(DISPLAY_I2C_BUS_ADDRESS, DISPLAY_I2C_DEVICE_ADDRESS, DISPLAY_WIDTH, DISPLAY_HEIGHT, NULL);
    if (!p_display) {
        printf("Error: Could not open display\n");
        return false;
    }
    if (ssd1306_i2c_display_initialize(p_display) < 0) {
        printf("Error: Could not initialize display\n");
        ssd1306_i2c_close(p_display);
        return false;
    }
    sleep(3);
    p_framebuffer = ssd1306_framebuffer_create(p_display->width, p_display->height, p_display->err);
    display_routine_arg = (display_routine_arg_t){ p_display, p_framebuffer };

    // Init Kinéis module
    fd_kineis_port = kineis_init();
    if (fd_kineis_port < 0) {
        printf("Error: Could not initialize Kinéis COM port\n");
        return false;
    }

    // Init emergency button interrupt
    wiringPiSetup();
    pinMode(EMERGENCY_BUTTON_PIN, INPUT);
    wiringPiISR(EMERGENCY_BUTTON_PIN, INT_EDGE_FALLING, &create_emergency_log);

    return true;
}

bool copy_log_file_to_usb()
{
    // Build path string
    char path_to_log[30];
    strcpy(path_to_log, USB_MOUNT_PATH);
    strcat(path_to_log, LOG_FILE_NAME);

    FILE* log_file = fopen(path_to_log, "w");
    if (log_file == NULL) {
        perror("Can't open log file\n");
        return false;
    }

    FILE* local_log_file = fopen(LOG_FILE_PATH, "r");
    if (local_log_file == NULL) {
        perror("Can't open local log file\n");
        return false;
    }

    // Read local log file line by line and write it to the USB device
    char line[MAX_LENGTH_LOG_LINE];
    while (fgets(line, sizeof(line), local_log_file) != NULL)
        fputs(line, log_file);
    printf("Log file copied to USB device.\n");

    fclose(local_log_file);
    fclose(log_file);

    return true;
}

void retrieve_data_to_send(char* data_to_send, int max_size)
{
    const int participants_per_char = 4;
    log_t log[participants_per_char];
    bool passages[participants_per_char];

    // First byte represents the emergency state
    data_to_send[0] = is_emergency_active() ? 'f' : '0';

    // Other characters represent the participants passages
    // Test every participant code
    for (int i = 1; i <= max_size * participants_per_char; i += participants_per_char) {
        int index = (i / participants_per_char) + 1;

        // Search for the last log of the 4 next participants (start counting at code = 1)
        for (int j = 0; j < participants_per_char; j++)
            passages[j] = get_last_log_by_code(&log[j], i + j);

        // Number representing the 4 participant passage states (e.g. 1001 means
        // the first and last did pass the checkpoint)
        int number_representation = 0;
        number_representation = passages[0] ? number_representation + 1000 : number_representation;
        number_representation = passages[1] ? number_representation + 100 : number_representation;
        number_representation = passages[2] ? number_representation + 10 : number_representation;
        number_representation = passages[3] ? number_representation + 1 : number_representation;

        // Add character (0-F) depending on the 4 parcitipant passage sates
        switch (number_representation) {
        case    0: data_to_send[index] = '0'; break;
        case    1: data_to_send[index] = '1'; break;
        case   10: data_to_send[index] = '2'; break;
        case   11: data_to_send[index] = '3'; break;
        case  100: data_to_send[index] = '4'; break;
        case  101: data_to_send[index] = '5'; break;
        case  110: data_to_send[index] = '6'; break;
        case  111: data_to_send[index] = '7'; break;
        case 1000: data_to_send[index] = '8'; break;
        case 1001: data_to_send[index] = '9'; break;
        case 1010: data_to_send[index] = 'A'; break;
        case 1011: data_to_send[index] = 'B'; break;
        case 1100: data_to_send[index] = 'C'; break;
        case 1101: data_to_send[index] = 'D'; break;
        case 1110: data_to_send[index] = 'E'; break;
        case 1111: data_to_send[index] = 'F'; break;
        }
    }
}
