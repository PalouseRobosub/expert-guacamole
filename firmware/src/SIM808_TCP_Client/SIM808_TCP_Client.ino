#include "Adafruit_FONA.h"

#define FONA_RX 2
#define FONA_TX 3
#define FONA_RST 4

char replybuffer[255];

#include <SoftwareSerial.h>
SoftwareSerial fonaSerial = SoftwareSerial(FONA_TX, FONA_RX);

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

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
    while (!Serial);
    Serial.begin(115200);

    fonaSerial.begin(4800);

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
}

void loop() 
{
    send_to("robosub.eecs.wsu.edu", 5959, "i need alcohol");

    char time_str[20];
    fona.getTime(time_str, 20);
    Serial.print("Time: ");
    Serial.println(time_str);

    delay(5000);
}
