#include <Arduino.h>
#include "sensors.h"

#define BUTTON_LED_PIN 6
#define BUTTON_PIN 8


Sensors sensors;
Switch cancel_button;



void setup()
{
    sensors.init();
    cancel_button.init(BUTTON_PIN, Switch::Polarity::ACTIVE_LOW);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUTTON_LED_PIN, OUTPUT);
    digitalWrite(BUTTON_LED_PIN, LOW);
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
        for (uint8_t i = 0; i < 10*20; ++i)
        {
            analogWrite(BUTTON_LED_PIN, 255);
            if (cancel_button.read() == true)
            {
                Serial.println("cancel button press received!");
                button_pressed = true;
                break;
            }

            delay(50);
        }
        digitalWrite(BUTTON_LED_PIN, 0);
        if(button_pressed != true)
        {

            Serial.println("sending gsm data");
            //send message
        }
    }

    delay(1000);
}
