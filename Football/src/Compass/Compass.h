class Compass
{
    private:
    int pinNum;
    unsigned int startValue;
    public:
    Compass(int _pinNum);
    int GetValue();
};