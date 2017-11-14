#include "Arduino.h"
#include <Servo.h>

class Motor
{
public:
    Motor() :
        position_{1500},
        position_ref_{1500},
        time_{500}
    {
    }

    void attach(int pin)
    {
        servo_.attach(pin);
    }

    void loop()
    {
        position_ += 3.0 * (position_ref_ - position_) / time_;
        servo_.writeMicroseconds(static_cast<int>(position_));
    }

    void set(double position_ref, double time)
    {
        position_ref_ = position_ref;
        time_ = time;
    }

private:
    Servo servo_;
    double position_;
    double position_ref_;
    double time_;  // us

};


constexpr int motors_number = 6;
Motor motors[motors_number];

void setup()
{
    for(int n=0; n<motors_number; ++n) {
        motors[n].attach(13 - n);
    }
    Serial.begin(9600);
}

int getMotor()
{
    if (Serial.read() == '#') {
        return Serial.parseInt();
    }
    return -1;
}

int getPosition()
{
    if (Serial.read() == 'P') {
        return Serial.parseInt();
    }
    return -1;
}

int getTime()
{
    if (Serial.read() == 'T') {
        return Serial.parseInt();
    }
    return -1;
}


void motorLoop()
{
    for(auto& motor : motors) {
        motor.loop();
    }
    delay(1);
}

void loop()
{
    while (Serial.available() > 0) {
        auto n = getMotor();
        auto p = getPosition();
        auto t = getTime();
        if (Serial.read() == '\n') {
            motors[n - 1].set(p, t);
        }
    }
    motorLoop();
}

