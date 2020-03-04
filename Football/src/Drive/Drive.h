#include <Adafruit_MotorShield.h>

class Drive
{
    private:
    void Motor(int _dir,int _speed,Adafruit_DCMotor _motor);

    public:
    Adafruit_DCMotor *motora;
    Adafruit_DCMotor *motorb;
    Adafruit_DCMotor *motorc;
    Adafruit_DCMotor *motord;
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    Drive(/* args */);
    void Begin();
    void StartAllMotors();
    void RotateClockwise(int _speed);
    void RotateCounterClockwise(int _speed);
    void DriveForward(int _speed);
    void DriveBackward(int _speed);
    void DriveLeft(int _speed);
    void DriveRight(int _speed);
    void DriveDiagonalRechtsHinten(int _speed);

    void Stop();
};


