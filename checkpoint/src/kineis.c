/**
 * File containing the functions to interact with the Kinéis module.
 * @file kineis.c
 * @author Noé Maillet & Mathéo Mercier
 * based on the code of Quentin Rosinki
 * @date 2023-02-07
 *
*/

#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "kineis.h"

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;//struct termios to configure our COM port

    //we copy the actual conf of the COM port
    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    /********control flags*********/
    tty.c_cflag &= ~CRTSCTS;//no flow control
    tty.c_cflag &= ~CSIZE;//no byte size set
    tty.c_cflag |= CS8;//8 bits byte size
    tty.c_cflag &= ~CSTOPB;//one stop bit
    tty.c_cflag |= CREAD;//allow to read data
    tty.c_cflag |= CLOCAL;//no modem control

    /**********local flags**********/
    tty.c_lflag &= ~ICANON; // non-canonical mode
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

    /**********input flags**********/
    //we disable anything to juste get raw data on the port
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); //disable software flow control
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes

    /**********output flags**********/
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    /***********time flags***********/
    tty.c_cc[VMIN] = 1;//read block until 1 char is received
    //or block until
    tty.c_cc[VTIME] = 10;//1 second passed

    /********** BAUDRATE ************/
    cfsetospeed(&tty, speed);//set input baudrate
    cfsetispeed(&tty, speed);//set output baudrate

    //finally we save the conf for the port COM
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int kineis_init()
{
    int fd = open(TTY_PATH, O_RDWR | O_NOCTTY | O_SYNC);//open the COM port in read/write mode, fd is the descriptor of the port

    if (fd < 0) {
        printf("error %d opening %s: %s", errno, TTY_PATH, strerror(errno));
        return -1;
    }
    set_interface_attribs(fd, BAUDRATE);  // set speed to 115,200 bps, 8n1 (no parity)
    fcntl(fd, F_SETFL, O_NONBLOCK);//set non blocking read
    printf("Kineis module initialized\n");
    kineis_set_band(fd, BAND);
    kineis_set_power(fd, POWER);
    return fd;
}

void kineis_close(int fd)
{
    close(fd);
}

void kineis_send_data(int fd, char* data, int len)
{
    char write_buffer[MAX_KINEIS_DATA_SIZE];

    if (len < MAX_KINEIS_DATA_SIZE && fd > 0) {
        strcpy(write_buffer, "AT+TX=");
        strcat(write_buffer, data);
        strcat(write_buffer, "\r\n");
        printf("we send to kineis : %s\n", write_buffer);
        write(fd, &write_buffer, strlen(write_buffer));
        sleep(10);//the data sometime long to transmit through Kineis
    }

}

void kineis_read_data(int fd, char* response)
{
    char read_buffer;
    char tmp[MAX_SERIAL_READ_SIZE];
    int i = 0;
    sleep(1);

    while (read(fd, &read_buffer, 1) > 0 && i < MAX_SERIAL_READ_SIZE) {
        tmp[i] = read_buffer;
        i++;
    }
    tmp[i] = '\0';

    usleep((strlen(tmp) + 25) * 1000); // sleep enough to transmit the command
    strcpy(response, tmp);
    printf("Read %d bytes from kineis : %s\n", i, response);
}

void kineis_set_band(int fd, char* band)
{
    char write_buffer[MAX_KINEIS_DATA_SIZE];
    char read_buffer[MAX_KINEIS_DATA_SIZE];

    if (fd > 0) {
        strcpy(write_buffer, "AT+BAND=");
        strcat(write_buffer, band);
        strcat(write_buffer, "\r\n");
        printf("we send to kineis : %s\n", write_buffer);
        write(fd, &write_buffer, strlen(write_buffer));
        usleep((strlen(write_buffer) + 25) * 1000); // sleep enough to transmit the command
    }
}

void kineis_set_power(int fd, int power)
{
    char write_buffer[MAX_KINEIS_DATA_SIZE];
    char tmp[10];
    char read_buffer[MAX_KINEIS_DATA_SIZE];

    if (fd > 0) {
        strcpy(write_buffer, "AT+PWR=");
        sprintf(tmp, "%d", power);//convert int to string
        strncat(write_buffer, tmp, 4);//to avoid \0 character
        strcat(write_buffer, "\r\n");
        printf("we send to kineis : %s\n", write_buffer);
        write(fd, &write_buffer, strlen(write_buffer));
        usleep((strlen(write_buffer) + 25) * 1000); // sleep enough to transmit the command
    }
}
