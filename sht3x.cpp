#include "sht3x.h"

/**
 * @brief Setup the sht3x sensor communication.
 * 
 * @param filename I2C bus to open.
 * @param address I2C address of the sensor.
 * @return int SUCCESS or ERROR.
 */
int sht3x::setup(std::string filename, int address)
{
    const char *filename_const = filename.c_str();

    // Open i2c bus
    file_i2c = open(filename_const, O_RDWR);
    if (file_i2c < 0){
        std::cout << "Failed to open the i2c bus\n";
        return ERROR;
    }

    // Talk to device
    if (ioctl(file_i2c, I2C_SLAVE, address) < 0){
        std::cout << "Failed to acquire bus access and/or talk to slave.\n";
        return ERROR;
    }

    return SUCCESS;
};

/**
 * @brief Sample the sensor and update the temperatureC and humidity values.
 * 
 * @return int SUCCESS or ERROR
 */
int sht3x::sample(){
    // Start a measurement: High repeatablility = 12.5ms measurement time
    unsigned char bytesToSend[2] = {clockStretchEnabled, HiRepeatablilityWithStretch};
    if (writeBytes(bytesToSend) != SUCCESS){
        return ERROR;
    }

    // Wait for measurements to be finished
    sleep(0.2);

    // Read response
    if (readIntoBuffer() != SUCCESS){
        return ERROR;
    }

    // Set temperature and humidity values
    temperatureC = calculateTemperatureC();
    humidity = calculateHumidity();

    return SUCCESS;
}


/**
 * @brief Write 2 bytes to the sensor
 * 
 * @param bytesToSend The two bytes to send via i2c.
 * @return int SUCCESS or ERROR
 */
int sht3x::writeBytes(unsigned char bytesToSend[2])
{
    if (write(file_i2c, bytesToSend, 2) != 2)
    {
        std::cout << "Failed to write to the i2c bus.\n";
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Read the temperature and humidity reponse from the sensor.
 * 
 * @return int SUCCESS or ERROR
 */
int sht3x::readIntoBuffer(){
    // Read the 6 bytes - [tempHi, tempLo, tempCRC, humidityHi, humidityLo, humidityCRC]
    if (read(file_i2c, buffer, 6) != 6)
    {
        printf("Failed to read from the i2c bus.\n");
        return ERROR;
    }
    return SUCCESS;
}

/**
 * @brief Calculate the temperature celsius based on the values in the buffer.
 * 
 * @return float 
 */
float sht3x::calculateTemperatureC(){
    float temp = buffer[0] * 256 + buffer[1];
    float cTemp = -45 + (175 * temp / 65535);

    return cTemp;
}

/**
 * @brief Calculate the humidity based on the values in the buffer.
 * 
 * @return float 
 */
float sht3x::calculateHumidity(){
    float temp = buffer[3] * 256 + buffer[4];
    float humidity = 100 * (temp/(65536-1));

    return humidity;
}
