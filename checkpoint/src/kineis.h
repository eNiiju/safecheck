/**
 * Header for the file containing the functions to interact with the Kinéis module.
 * @file kineis.h
 * @author Noé Maillet & Mathéo Mercier
 * @date 2023-02-07
 *
 */

#ifndef KINEIS_H
#define KINEIS_H

/* ------------------------------------------------------------------------- */
/*                          Constants & Definitions                          */
/* ------------------------------------------------------------------------- */

#define MAX_KINEIS_DATA_SIZE 45 // max size of the data sent to the kineis module
#define MAX_SERIAL_READ_SIZE 100 // max size of the data read from the serial port
#define TTY_PATH "/dev/ttyUSB0" // path of the serial port of kineis
#define BAUDRATE B4800 // baudrate of the serial port
#define POWER 1000 // power of transmission
#define BAND "B1" // band of frequence use by the kineis module

#define CRTSCTS 020000000000 // flow control (hacky)

/* ------------------------------------------------------------------------- */
/*                            Function prototypes                            */
/* ------------------------------------------------------------------------- */

/**
 * @brief configure the COM port associated with the kineis module
 * inspired by https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
 * @param fd file descriptor of the serial port
 * @param speed baudrate to be used
 * @return 0 if success, -1 if error
*/
int set_interface_attribs(int fd, int speed);

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
*/
void kineis_read_data(int fd, char* response);

/**
 * @brief Send data to the serial port of kineis
 * @param fd file descriptor of the serial port
 * @param data data to be sent
 * @param len length of the data to be sent (must be < MAX_KINEIS_DATA_SIZE)
*/
void kineis_send_data(int fd, char* data, int len);

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
void kineis_set_band(int fd, char* band);

#endif
