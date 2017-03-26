#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <stdint.h>

//sensor base class
class Sensor
{
public:
    virtual bool read(void) = 0;
};

class LightSensor : Sensor
{
public:
    void init(uint8_t _pin, uint16_t _threshold)
    {
        pin = _pin;
        threshold = _threshold;
        pinMode(pin, INPUT);
    }
    bool read(void);
private:
    uint8_t pin;
    uint16_t threshold;
};

class Switch : Sensor
{
public:
    enum class Polarity : uint8_t
    {
        ACTIVE_HIGH,
        ACTIVE_LOW
    };
    void init(uint8_t _pin, Polarity _polarity)
    {
        pin = _pin;
        polarity = _polarity;
        pinMode(pin, INPUT);
    }
    bool read(void);
private:
    uint8_t pin;
    Polarity polarity;
};


//superclass for reading all sensors
class Sensors
{
public:
    Sensors() {}
    ~Sensors() {}

    //initializes all sensors
    void init(uint8_t door_switch_pin, uint8_t light_sensor_pin);

    // reads all of our sensors to determine if the mailbox has been opened
    // returns true if mailbox has been opened
    // returns false if mailbox has not been opened
    bool mailbox_opened(void);

private:
    LightSensor light_sensor;
    Switch reed_switch;
};

#endif // SENSORS_H
