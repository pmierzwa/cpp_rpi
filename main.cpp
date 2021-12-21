#include "global.h"

int main()
{
    // Initialize sensor
    sht3x sensor;
    if (sensor.setup("/dev/i2c-1", 0x44) != SUCCESS){
        return ERROR;
    }

    // Read temperature
    sensor.sample();

    std::cout << "Temperature: "<< sensor.temperatureC << " °C \n";

    std::cout << "Humidity: " << sensor.humidity << " % \n";

    return SUCCESS;
}
