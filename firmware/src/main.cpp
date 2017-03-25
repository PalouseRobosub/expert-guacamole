#include <Arduino.h>
#include "sensors.h"

void setup()
{
}

void loop()
{
    bool opened = mailbox_opened();
    if(opened == true)
    {
        //send message
    }

    delay(1000);
}
