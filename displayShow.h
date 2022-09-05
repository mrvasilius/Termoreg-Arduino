#include "GyverTM1637.h"

#ifndef DISP_CLK
#define DISP_CLK 8
#endif

#ifndef DISP_DIO
#define DISP_DIO 9
#endif

class DisplayShow : public GyverTM1637
{
public:
    DisplayShow();
    void init(int *theTemp, float *temperature);
    void showTemp();
    void showEdit();
    void showOn();
    void showOff();

private:
    //массив изображений 7 сегментных символов от 0 до 9 и пустота
    const byte simv[12] = {
        B00111111, //0
        B00000110, //1
        B01011011, //2
        B01001111, //3
        B01100110, //4
        B01101101, //5
        B01111101, //6
        B00000111, //7
        B01111111, //8
        B01101111, //9
        B00000000, //пустота
        B01100011, //o
    };

    float *_temperature;
    int *_theTemp;
};

extern DisplayShow Display;

DisplayShow::DisplayShow() : GyverTM1637(DISP_CLK, DISP_DIO) {}

void DisplayShow::init(int *theTemp, float *temperature)
{
    _theTemp = theTemp;
    _temperature = temperature;
    clear();
    brightness(7); // яркость, 0 - 7 (минимум - максимум)
}

void DisplayShow::showTemp()
{
    byte dig3 = (*_temperature >= 10 || *_temperature <= -10) ? simv[(int)*_temperature / 10] : simv[10];
    byte dig4 = simv[(int)*_temperature % 10];

    displayByte(_t, simv[11], dig3, dig4);
}

void DisplayShow::showEdit()
{
    byte dig3 = (*_theTemp >= 10 || *_theTemp <= -10) ? simv[*_theTemp / 10] : simv[10];
    byte dig4 = simv[*_theTemp % 10];

    displayByte(_E, _d, dig3, dig4);
}

void DisplayShow::showOn()
{
    displayByte(_O, _n, _empty, _empty);
}

void DisplayShow::showOff()
{
    displayByte(_O, _f, _f, _empty);
}

DisplayShow Display = DisplayShow();