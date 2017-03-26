#include <Arduino.h>
#include "sensors.h"

#define CANCEL_BUTTON_LED_PIN 6
#define CANCEL_BUTTON_PIN 8
#define DOOR_SWITCH_PIN 7
#define LIGHT_SENSOR_PIN A0



#define _CANCEL_TIMEOUT_SEC 10
#define CANCEL_TIMEOUT _CANCEL_TIMEOUT_SEC * 20



Sensors sensors;
Switch cancel_button;

void blink_light(uint16_t i);

void setup()
{
    sensors.init(DOOR_SWITCH_PIN, LIGHT_SENSOR_PIN);
    cancel_button.init(CANCEL_BUTTON_PIN, Switch::Polarity::ACTIVE_LOW);
    pinMode(CANCEL_BUTTON_PIN, INPUT_PULLUP);
    pinMode(CANCEL_BUTTON_LED_PIN, OUTPUT);
    digitalWrite(CANCEL_BUTTON_LED_PIN, LOW);
    Serial.begin(9600);
}

void loop()
{
    bool opened = sensors.mailbox_opened();
    if(opened == true)
    {
        Serial.println("Detected mailbox opened");
        bool button_pressed = false;

        //wait for up to 30 seconds
        Serial.println("waiting for cancel button press...");
        for (uint16_t i = 0; i < CANCEL_TIMEOUT; ++i)
        {
            blink_light(i);
            if (cancel_button.read() == true)
            {
                Serial.println("cancel button press received!");
                button_pressed = true;
                break;
            }

            delay(50);
        }
        digitalWrite(CANCEL_BUTTON_LED_PIN, 0);
        if(button_pressed != true)
        {

            Serial.println("sending gsm data");
            //send message
        }
        else //cancel button pressed, wait for door to close
        {
            while(sensors.mailbox_opened() == true)
            {
                delay(200);
            }

        }
    }

    delay(200);
}

void blink_light(uint16_t i)
{
    if (i/20 < _CANCEL_TIMEOUT_SEC - 3)
    {
        if (i % 10 == 0)
        {
            digitalWrite(CANCEL_BUTTON_LED_PIN,
                         !digitalRead(CANCEL_BUTTON_LED_PIN));
        }
    }
    else
    {
        if (i % 1 == 0)
        {
            digitalWrite(CANCEL_BUTTON_LED_PIN,
                         !digitalRead(CANCEL_BUTTON_LED_PIN));
        }
    }
}
