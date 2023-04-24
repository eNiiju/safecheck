/**
 * File containing the functions to interact with the Kinéis module.
 * @file kineis.c
 * @author Noé Maillet & Mathéo Mercier
 * based on the code of Quentin Rosinki
 * @date 2023-02-07
 *
*/

#include "kineis.h"
#include <errno.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

/* ------------------------------------------------------------------------- */
/*                                 Functions                                 */
/* ------------------------------------------------------------------------- */
int set_interface_attribs (int fd, int speed){
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }
    cfsetospeed(&tty,BAUDRATE);
    cfsetispeed(&tty,BAUDRATE);

    tty.c_iflag = 0;
    tty.c_oflag = 0;
    tty.c_cflag=CS8|CREAD|CLOCAL;
    tty.c_lflag = 0;
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 5;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}

int kineis_init(){
    int fd = open (TTY_PATH, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0){
        printf("error %d opening %s: %s", errno, TTY_PATH, strerror (errno));
        return -1;
    }
    set_interface_attribs (fd, BAUDRATE);  // set speed to 115,200 bps, 8n1 (no parity)
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);//set non blocking read
    return fd;
}

void kineis_close(int fd){
    close(fd);
}

void kineis_send_data(int fd, char *data, int len) {
    char write_buffer[MAX_KINEIS_DATA_SIZE];
    char read_buffer[MAX_KINEIS_DATA_SIZE];

    if(len < MAX_KINEIS_DATA_SIZE && fd > 0) {
        strcpy(write_buffer,"AT+TX=");
        strcat(write_buffer,data);
        strcat(write_buffer,"\r\n");
        write(fd, &write_buffer,strlen(write_buffer));
        usleep ((strlen(write_buffer) + 25) * 100); // sleep enough to transmit the data
        kineis_read_data(fd,read_buffer);
    }
}

int kineis_read_data(int fd, char* response){
    char read_buffer[MAX_KINEIS_DATA_SIZE];
    int res;
    
    res = read(fd, &read_buffer,MAX_SERIAL_READ_SIZE);
    if(res < 0) {
        printf("Error reading from serial port\r\n");
        return -1;
    }
    strcpy(response,read_buffer);
    printf("Read %d bytes from kineis : %s\r\n", res, read_buffer);
    return res;
}

void kineis_set_band(int fd, int band) {
    char write_buffer[MAX_KINEIS_DATA_SIZE];
    char tmp[10];
    char read_buffer[MAX_KINEIS_DATA_SIZE];

    if(fd > 0) {
        strcpy(write_buffer, "AT+BAND=");
        sprintf(tmp, "%d", band);
        strcat(write_buffer, tmp);
        strcat(write_buffer, "\r\n");
        write(fd, &write_buffer, strlen(write_buffer));
        usleep ((strlen(write_buffer) + 25) * 100); // sleep enough to transmit the command
        kineis_read_data(fd, read_buffer);
    }
}

void kineis_set_power(int fd, int power) {
    char write_buffer[MAX_KINEIS_DATA_SIZE];
    char tmp[10];
    char read_buffer[MAX_KINEIS_DATA_SIZE];

    if(fd > 0) {
        strcpy(write_buffer, "AT+POWER=");
        sprintf(tmp, "%d", power);
        strcat(write_buffer, tmp);
        strcat(write_buffer, "\r\n");
        write(fd, &write_buffer, strlen(write_buffer));
        usleep ((strlen(write_buffer) + 25) * 100); // sleep enough to transmit the command
        kineis_read_data(fd, read_buffer);
    }
}

/*
void *my_init() {
    void *addr = sem_open(SEM_FILENAME, SEM_FLAG, SEM_RIGHTS, SEM_INIT_VALUE);
    return addr;
}

int initSerialKineis() {
    tcgetattr(STDOUT_FILENO,&old_stdio);
    memset(&stdio,0,sizeof(stdio));
    stdio.c_iflag = 0;
    stdio.c_oflag = 0;
    stdio.c_cflag = 0;
    stdio.c_lflag = 0;
    stdio.c_cc[VMIN] = 1;
    stdio.c_cc[VTIME] = 0;
    tcsetattr(STDOUT_FILENO,TCSANOW,&stdio);
    tcsetattr(STDOUT_FILENO,TCSAFLUSH,&stdio);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);       //make the reads non-blocking
    memset(&tio,0,sizeof(tio));
    tio.c_iflag = 0;
    tio.c_oflag = 0;
    tio.c_cflag=CS8|CREAD|CLOCAL;
    tio.c_lflag = 0;
    tio.c_cc[VMIN] = 1;
    tio.c_cc[VTIME] = 5;

    if((serial_tty = open(TTY_PATH, O_RDWR | O_NONBLOCK )) == -1) {
        printf("Erreur à l'ouverture du port %d\r\n", errno);
        return -1;
    } else printf("Ouverture du port OK \r\n");

    cfsetospeed(&tio,BAUDRATE);
    cfsetispeed(&tio,BAUDRATE);
    tcsetattr(serial_tty,TCSANOW,&tio);
    return 0;
}

int closeSerialKineis() {
    close(serial_tty);
    tcsetattr(STDOUT_FILENO, TCSANOW, &old_stdio);
}

int readSerialKineis(char *buffer, void * sem_addr) {
    char read_buffer;
    int i = 0;

    sleep(1);

    while (read(serial_tty, &read_buffer, 1) > 0) {
        buffer[i]=read_buffer;
        i++;
    }

    sem_post(sem_addr);
    return 0;
}

int writeSerialKineis(int command, void * sem_addr) {
    char TX_buffer[BUFFER_SIZE];
    char RX_buffer[BUFFER_SIZE];

    switch(command) {
        case SET_POWER:
            printf("Envoi de la configuration du gain de l'antenne\r\n");
            strcpy(TX_buffer,"AT+PWR=1000\r\n");
            write(serial_tty,&TX_buffer,strlen(TX_buffer));
            readSerialKineis(RX_buffer, sem_addr);
            sem_wait(sem_addr);
            printf("Réponse du module Kinéis : %s\r\n",RX_buffer);
            break;

        case SET_BAND:
            printf("Envoi de la configuration des bandes de fréquences\r\n");
            strcpy(TX_buffer,"AT+BAND=B1\r\n");
            write(serial_tty,&TX_buffer,strlen(TX_buffer));
            readSerialKineis(RX_buffer, sem_addr);
            sem_wait(sem_addr);
            printf("Réponse du module Kinéis : %s\r\n", RX_buffer);
            break;

        case TX_DATA:
            printf("Envoi de données : 454647\r\n");
            strcpy(TX_buffer,"AT+TX=454647\r\n");
            write(serial_tty,&TX_buffer,strlen(TX_buffer));
            readSerialKineis(RX_buffer, sem_addr);
            sem_wait(sem_addr);
            printf("Réponse du module Kinéis : %s\r\n", RX_buffer);
            break;

        default:
            break;
    }
    return 0;
}
*/