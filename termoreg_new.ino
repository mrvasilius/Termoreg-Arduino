#include <Wire.h>
#include <OneWire.h>

#define REQUIRESALARMS false
#include <DallasTemperature.h>

#include <iarduino_RTC.h>

#include "GyverTimer.h"
#include "GyverEncoder.h"

#define RELAY_BUS 12
#include "relayControl.h"

#define MIN_TEMP 0
#define MAX_TEMP 30
#include "tuningTemp.h"

#define DISP_CLK 8
#define DISP_DIO 9
#include "displayShow.h"

#define POWER_OFF 0
#define POWER_ON 1
#define POWER_AUTO 2

#define ENC_CLK 2
#define ENC_DT 3
#define ENC_SW 4

#define TIME_RST 5
#define TIME_CLK 7
#define TIME_DAT 6

#define ONE_WIRE_BUS 10

// timers
GTimer_ms dispTimer(100);
GTimer_ms tempTimer(60000);
GTimer_ms editTimer(10000);
GTimer_ms blinkTimer(1000);

// temperature
//const byte sens[8] = {0x28, 0xFF, 0x7F, 0x27, 0x53, 0x17, 0x04, 0xC0}; // На столе
//const byte sens[8] = {0x28, 0xAA, 0x40, 0x6C, 0x40, 0x14, 0x01, 0x5E}; // РЕСАНТА 1500 - ванная
const byte sens[8] = {0x28, 0xAA, 0xB4, 0x2D, 0x40, 0x14, 0x01, 0x88}; // SCOOLE 1500 х2 - гостинная
OneWire sensDs(ONE_WIRE_BUS);
DallasTemperature ds(&sensDs);
float temperature = 0;
int theTemp = 20;

// time
iarduino_RTC time(RTC_DS1302, TIME_RST, TIME_CLK, TIME_DAT);

// encoder
Encoder enc1(ENC_CLK, ENC_DT, ENC_SW);
bool editMode = false;
bool timeMode = false;
byte power = POWER_AUTO;

boolean isShowTemp = true;
boolean isLight = true;

extern int __bss_end;
extern void *__brkval;

void setup()
{
    // Serial.begin(9600);

    Display.init(&theTemp, &temperature);

    // relay
    RC.init(&theTemp, &temperature);

    // tuning
    enc1.setType(TYPE2);
    enc1.setTickMode(AUTO);
    Tuning.init(&theTemp);

    // timers
    editTimer.setMode(MANUAL);

    time.begin();
    time.gettime();

    Wire.begin();
    ds.begin();
    ds.requestTemperatures();
    delay(1000);
    temperature = ds.getTempC(sens);
    ds.requestTemperatures();
}

void loop()
{

    if (enc1.isHolded())
    {
        // Serial.println("Holded");
        editMode = false;
        Display.point(false);
        timeMode = !timeMode;
    }

    if (tempTimer.isReady())
    {
        temperature = ds.getTempC(sens);
        if (power == POWER_AUTO)
        {
            time.gettime();
            RC.setRelay(time.Hours);
        }
        ds.requestTemperatures();
    }

    if (timeMode)
    {
        // if (enc1.isClick())
        // {
        //     editMode = true;

        // }
        // // add settings
        // if (enc1.isTurn())
        // {
        //     if (enc1.isRight())
        //     {
        //     }

        //     if (enc1.isLeft())
        //     {
        //     }
        // }
        if (blinkTimer.isReady())
        {
            isLight = !isLight;
        }
        if (dispTimer.isReady())
        {
            Display.point(isLight);
            Display.displayClock(time.Hours, time.minutes);
        }
    }
    else
    {
        if (enc1.isTurn())
        {
            editMode = true;
            editTimer.start();
            if (enc1.isRight())
            {
                Tuning.raiseTemp();
            }

            if (enc1.isLeft())
            {
                Tuning.reduceTemp();
            }
            editTimer.reset();
        }
        if (enc1.isClick())
        {
            power = power + 1 > 2 ? 0 : power + 1;
            if (power == POWER_OFF)
            {
                RC.setRelayOff();
            }
            if (power == POWER_ON)
            {
                RC.setRelayOn();
            }
            if (power == POWER_AUTO)
            {
                RC.setRelay(time.Hours);
            }
            isShowTemp = false;
            blinkTimer.reset();
        }
        if (editTimer.isReady())
        {
            editMode = false;
            RC.setRelay(time.Hours);
            editTimer.reset();
            editTimer.stop();
        }
        if (dispTimer.isReady())
        {
            if (power == POWER_AUTO)
            {
                editMode ? Display.showEdit() : Display.showTemp();
            }
            if (power == POWER_ON)
            {
                isShowTemp ? Display.showTemp() : Display.showOn();
            }
            if (power == POWER_OFF)
            {
                isShowTemp ? Display.showTemp() : Display.showOff();
            }
        }
        if (blinkTimer.isReady())
        {
            isShowTemp = !isShowTemp;
        }
    }
}
int memoryFree()
{
    int freeValue;
    if ((int)__brkval == 0)
        freeValue = ((int)&freeValue) - ((int)&__bss_end);
    else
        freeValue = ((int)&freeValue) - ((int)__brkval);
    return freeValue;
}