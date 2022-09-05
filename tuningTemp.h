#ifndef MIN_TEMP
#define MIN_TEMP 0
#endif

#ifndef MAX_TEMP
#define MAX_TEMP 30
#endif

class TuningTemp
{
public:
    TuningTemp();
    void init(int *theTemp);
    void raiseTemp();
    void reduceTemp();

private:
    int *_theTemp;
};

extern TuningTemp Tuning;

TuningTemp::TuningTemp() {}

void TuningTemp::init(int *theTemp)
{
    _theTemp = theTemp;
}

void TuningTemp::raiseTemp()
{
    *_theTemp = *_theTemp < MAX_TEMP ? *_theTemp + 1 : MAX_TEMP;
}
void TuningTemp::reduceTemp()
{
    *_theTemp = *_theTemp > MIN_TEMP ? *_theTemp - 1 : MIN_TEMP;
}

TuningTemp Tuning = TuningTemp();