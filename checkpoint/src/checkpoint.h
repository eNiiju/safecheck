/**
 * Header for the file containing the functions to interact with the Kinéis module.
 * @file kineis.h
 * @author Noé Maillet & Mathéo Mercier
 * based on the code of Quentin Rosinki
 * @date 2023-02-07
 *
 */

#ifndef KINEIS_H
#define KINEIS_H

#include <termios.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

/*  Termios parameters / inits           */
/*
#define BUFFER_SIZE 50
struct termios tio;
struct termios stdio;
struct termios old_stdio;
int serial_tty, errno;
*/
/*  Semaphore parameters / inits         */
/*
#define SEM_FILENAME "termios.sem"
#define SEM_FLAG O_CREAT
#define SEM_RIGHTS 0666
#define SEM_INIT_VALUE 0
void * sem_termios_addr;

/***************************** PROTOTYPES *****************************/
/*
void * my_init();
int initSerialKineis();
int closeSerialKineis();
int readSerialKineis(char *buffer, void * sem_addr);
int writeSerialKineis(int command, void * sem_addr);
*/
/*  EOF                                  */
//code matheo
/******************************* DEFINES ********************************/
#define MAX_KINEIS_DATA_SIZE 50 //max size of the data sent to the kineis module
#define MAX_SERIAL_READ_SIZE 100 //max size of the data read from the serial port
#define TTY_PATH "/dev/ttyUSB0" //path of the serial port of kineis
#define BAUDRATE B4800 //baudrate of the serial port
#define POWER 1000 //power of transmission
#define BAND B1//band of frequence use by the kineis module

/***************************** PROTOTYPES *****************************/
/**
 * @brief Set the interface attributes for serial port
 * inspired by https://stackoverflow.com/questions/6947413/how-to-open-read-and-write-from-serial-port-in-c
 * and Quentin Rosinki
 * @param fd file descriptor of the serial port
 * @param speed baudrate to be used
 * @return 0 if success, -1 if error
*/
int set_interface_attribs (int fd, int speed);

/**
 * @brief Initialize the serial port of kineis for communication
 * @return file descriptor which will be used as the serial port of kineis in other functions
 * @return -1 if error
*/
int kineis_init();

/**
 * @brief Close the serial port of kineis
 * @param fd file descriptor of the serial port
*/
void kineis_close(int fd);

/**
 * @brief Read data from the serial port of kineis read up to MAX_SERIAL_READ_SIZE bytes
 * @param fd file descriptor of the serial port 
 * @param response buffer to store the data read (if you need to)
 * @return number of bytes read
*/
int kineis_read_data(int fd, char* response);

/**
 * @brief Send data to the serial port of kineis
 * @param fd file descriptor of the serial port
 * @param data data to be sent
 * @param len length of the data to be sent (must be < MAX_KINEIS_DATA_SIZE)
*/
void kineis_send_data(int fd, char *data, int len);

/**
 * @brief Set the power of transmission of the kineis module
 * @param fd file descriptor of the serial port
 * @param power power of transmission (between 0 and 2500)
*/
void kineis_set_power(int fd, int power);

/**
 * @brief Set the band of frequence of the kineis module
 * @param fd file descriptor of the serial port
 * @param band band of frequence 
*/
void kineis_set_band(int fd, int band);

#endif //
