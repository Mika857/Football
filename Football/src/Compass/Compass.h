class Compass
{
    private:
    int pinNum;
    unsigned long value,premil;
    bool reading;
    unsigned int startValue;
    public:
    Compass(int _pinNum);
    int Read();
};