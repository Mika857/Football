class Schuss
{
    private:
    int pin;
    unsigned long premil;
    bool shot;
    #define delayValue 50

    public:
    Schuss(int pinNum);
    void Begin();
    void Schiese();
    void update();
};