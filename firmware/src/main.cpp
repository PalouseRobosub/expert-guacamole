#include <Arduino.h>
#include "sensors.h"

Sensors sensors;

void setup()
{
    sensors.init();
}

void loop()
{
    bool opened = sensors.mailbox_opened();
    if(opened == true)
    {
        //send message
    }

    delay(1000);
}
