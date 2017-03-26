#include <Arduino.h>
#include <avr/wdt.h>
#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"
#include "sensors.h"

/*
 * This pin is not actually used, but we are lazy.
 */
#define LIGHT_SENSOR_PIN A5

/*
 * Define the FONA connection pins.
 */
#define FONA_VIO 3
#define FONA_RST 4
#define FONA_RX 5
#define FONA_TX 6
#define FONA_KEY 7

/*
 * Define the sensor inputs.
 */
#define CANCEL_BUTTON_LED_PIN A2
#define CANCEL_BUTTON_PIN A1
#define DOOR_SWITCH_PIN A0

/*
 * Define algorithm timeouts.
 */
#define _CANCEL_TIMEOUT_SEC 10
#define CANCEL_TIMEOUT _CANCEL_TIMEOUT_SEC * 20

/*
 * Set up the FONA serial interface.
 */
SoftwareSerial fonaSerial = SoftwareSerial(FONA_TX, FONA_RX);
Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

Sensors sensors;
Switch cancel_button;

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
    /*
     * Set the IO voltage pin for the FONA and pull it out of reset.
     */
    pinMode(FONA_VIO, OUTPUT);
    digitalWrite(FONA_VIO, HIGH);
    delay(5);
    pinMode(FONA_RST, OUTPUT);
    digitalWrite(FONA_RST, LOW);
    delay(25);

    /*
     * Pull the SIM808 out of reset by setting key low and then pulling it high
     * for the remainder of operation (keeping it enabled). When the micro
     * powers off, the KEY pin will fall low and turn off the SIM808.
     */
    pinMode(FONA_KEY, OUTPUT);
    digitalWrite(FONA_KEY, LOW);

    sensors.init(DOOR_SWITCH_PIN, LIGHT_SENSOR_PIN);

    /*
     * The reed switch is normally closed, but when the mailbox door is closed,
     * the reed switch is therefore open. When the mailbox opens, the
     * reedswitch engages, pulling the pin down to ground and thus making an
     * active low signal.
     */
    cancel_button.init(CANCEL_BUTTON_PIN, Switch::Polarity::ACTIVE_LOW);

    /*
     * Set up the cancel button output pins.
     */
    pinMode(CANCEL_BUTTON_PIN, INPUT_PULLUP);
    pinMode(CANCEL_BUTTON_LED_PIN, OUTPUT);
    digitalWrite(CANCEL_BUTTON_LED_PIN, LOW);

    /*
     * Begin the serial ports to the FONA and the debug console.
     */
    Serial.begin(9600);
    fonaSerial.begin(4800);

    /*
     * Initialize the FONA and turn GPRS on.
     */
    if(!fona.begin(fonaSerial))
    {
        Serial.println("Couldn't find FONA");
        cli();
        wdt_enable(WDTO_15MS);
        while(1);
    }

    while(!fona.enableGPRS(true))
    {
        Serial.println("Could not enable GPRS. Retrying");
        delay(500);
    }

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

            while(!fona.enableNTPTimeSync(true, 0))
            {
                Serial.println("Could not enable NTP Time Sync. Retrying");
                delay(3000);
            }

            send_to("robosub.eecs.wsu.edu", 5959, "I need alcohol");
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
