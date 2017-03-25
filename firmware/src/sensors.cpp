#include "sensors.h"
#include <Arduino.h>

void Sensors::init(void)
{
    //TODO: figure out light sensor pins and threshold values
    //light_sensor.init()
}

bool Sensors::mailbox_opened(void)
{
    bool opened = false;

    opened |= light_sensor.read();

    return opened;
}

bool LightSensor::read(void)
{
    uint16_t val = analogRead(pin);

    if (val > threshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}
