class IrSensor
{
    private:
    int pinNum,offset;

    public:
    IrSensor(int _pinNum);
    int Read();
    int ReadNormal();
    int GetPinNum();
    void SetOffset(int _offset);
    int GetOffset();
};