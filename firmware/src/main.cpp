#include <Arduino.h>
#include "sensors.h"

Sensors sensors;
Switch cancel_button;

void setup()
{
    sensors.init();
    cancel_button.init(3, Switch::Polarity::ACTIVE_HIGH);
}

void loop()
{
    bool opened = sensors.mailbox_opened();
    if(opened == true)
    {
        bool button_pressed = false;
        //wait for up to 30 seconds
        for (uint8_t i = 0; i < 30*50; ++i)
        {
            if (cancel_button.read() == true)
            {
                button_pressed = true;
                break;
            }

            delay(50);
        }
        if(button_pressed != true)
        {
            //send message
        }
    }

    delay(1000);
}
