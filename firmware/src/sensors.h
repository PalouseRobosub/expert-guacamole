#ifndef SENSORS_H
#define SENSORS_H


class Sensors
{
public:
    Sensors() {}
    ~Sensors() {}

    //initializes all sensors
    void init(void);

    // reads all of our sensors to determine if the mailbox has been opened
    // returns true if mailbox has been opened
    // returns false if mailbox has not been opened
    bool mailbox_opened(void);


private:
};

#endif // SENSORS_H
