#ifndef SHT3X_H
#define SHT3X_H

#ifdef GLOBAL_H
#include "global.h"
#else
#include <iostream>
#include <fcntl.h>
#include <string>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>
#define SUCCESS 0
#define ERROR 1
#endif

// WriteBytes options (for reference)
// Byte 0 options:
#define clockStretchEnabled 0x2C
#define clockStretchDisabled 0x24

// Byte 1 options if byte 0 = clockStretchEnabled:
#define HiRepeatablilityWithStretch 0x06
#define MedRepeatablilityWithStretch 0x0D
#define LoRepeatablilityWithStretch 0x10

// Byte 1 options if byte 0 = clockStretchDisabled:
#define HiRepeatablilityNoStretch 0x00
#define MedRepeatablilityNoStretch 0x0B
#define LoRepeatablilityNoStretch 0x16

class sht3x
{
private:
    int file_i2c;
    unsigned char buffer[6] = {0};
    int writeBytes(unsigned char bytesToSend[2]);
    int readIntoBuffer();
    float calculateTemperatureC();
    float calculateHumidity();

public:
    float temperatureC;
    float humidity;
    int setup(std::string filename, int address);
    int sample();
};

#endif