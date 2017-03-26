#include "sensors.h"
#include <Arduino.h>

void Sensors::init(uint8_t door_switch_pin, uint8_t light_sensor_pin)
{
    //TODO: figure out light sensor pins and threshold values
    //light_sensor.init();

    //TODO: figure out reed switch pin an polarity
    reed_switch.init(door_switch_pin, Switch::Polarity::ACTIVE_LOW);
    pinMode(door_switch_pin, INPUT_PULLUP);
}

bool Sensors::mailbox_opened(void)
{
    bool opened = false;

    //opened |= light_sensor.read();
    opened |= reed_switch.read();

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

bool Switch::read(void)
{
    bool val = digitalRead(pin);

    //flip the polarity if necessary
    if (polarity == Polarity::ACTIVE_LOW)
    {
        val = !val;
    }

    return val;
}
