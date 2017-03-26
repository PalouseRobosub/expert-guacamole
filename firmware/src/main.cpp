#include <Arduino.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include "sensors.h"

#define LIGHT_SENSOR_PIN A0
#define FONA_VIO 3
#define FONA_RST 4
#define FONA_RX 5
#define FONA_TX 6
#define FONA_KEY 7
#define CANCEL_BUTTON_PIN 8
#define DOOR_SWITCH_PIN 9
#define CANCEL_BUTTON_LED_PIN 10

#define _CANCEL_TIMEOUT_SEC 10
#define CANCEL_TIMEOUT _CANCEL_TIMEOUT_SEC * 20

SoftwareSerial fonaSerial = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

Sensors sensors;
Switch cancel_button;

void blink_light(uint16_t i);

bool send_to(char *server, int port, char *data)
{
    if(!fona.TCPconnect(server, port))
    {
        Serial.print("TCPconnect failed to connect to: ");
        Serial.println(server);

        fona.TCPclose();
        return false;
    }

    if(!fona.TCPsend(data, strlen(data)))
    {
        Serial.println("TCPsend failed");

        fona.TCPclose();
        return false;
    }

    fona.TCPclose();

    return true;
}

void setup()
{
    sensors.init(DOOR_SWITCH_PIN, LIGHT_SENSOR_PIN);
    cancel_button.init(CANCEL_BUTTON_PIN, Switch::Polarity::ACTIVE_LOW);

    pinMode(CANCEL_BUTTON_PIN, INPUT_PULLUP);
    pinMode(CANCEL_BUTTON_LED_PIN, OUTPUT);
    pinMode(FONA_RST, OUTPUT);
    pinMode(FONA_VIO, OUTPUT);
    pinMode(FONA_KEY, OUTPUT);

    digitalWrite(CANCEL_BUTTON_LED_PIN, LOW);
    digitalWrite(FONA_RST, LOW);
    digitalWrite(FONA_VIO, HIGH);
    digitalWrite(FONA_KEY, LOW);

    Serial.begin(9600);
    fonaSerial.begin(4800);
}

void loop()
{
    Serial.println("checking mailbox door");
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

            // Send message
            if(!fona.begin(fonaSerial))
            {
                Serial.println("Couldn't find FONA");
                while (1);
            }

            while(!fona.enableGPRS(true))
            {
                Serial.println("Could not enable GPRS. Retrying");
                delay(3000);
            }

            while(!fona.enableNTPTimeSync(true, 0))
            {
                Serial.println("Could not enable NTP Time Sync. Retrying");
                delay(3000);
            }

            send_to("robosub.eecs.wsu.edu", 5959, "i need alcohol");
        }

        // wait for door to close before continuing
        Serial.println("waiting for door to close");
        while(sensors.mailbox_opened() == true)
        {
            delay(200);
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
