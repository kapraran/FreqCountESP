#include "FreqCountESP.h"

volatile uint8_t _FreqCountESP::sIsFrequencyReady = false;
volatile uint32_t _FreqCountESP::sCount = 0;
volatile uint32_t _FreqCountESP::sFrequency = 0;

portMUX_TYPE _FreqCountESP::sCountMux = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE _FreqCountESP::sTimerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onCount()
{
  portENTER_CRITICAL_ISR(&_FreqCountESP::sCountMux);
  _FreqCountESP::sCount++;
  portEXIT_CRITICAL_ISR(&_FreqCountESP::sCountMux);
}

void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&_FreqCountESP::sTimerMux);
  _FreqCountESP::sFrequency = _FreqCountESP::sCount;
  _FreqCountESP::sCount = 0;
  _FreqCountESP::sIsFrequencyReady = true;
  portEXIT_CRITICAL_ISR(&_FreqCountESP::sTimerMux);
}

_FreqCountESP::_FreqCountESP()
{
  mTimer = NULL;
}

_FreqCountESP::~_FreqCountESP()
{
  end();
}

void _FreqCountESP::begin(uint8_t pin, uint16_t timerMs)
{
  mPin = pin;
  mTimerMs = timerMs;
  sIsFrequencyReady = false;
  sCount = 0;
  sFrequency = 0;

  pinMode(mPin, INPUT_PULLUP);

  attachInterrupt(mPin, &onCount, RISING);

  // timer = timerBegin(0, 80, true);
  // timerAttachInterrupt(timer, &onTimer, true);
  // timerAlarmWrite(timer, 1000000, true);
  // timerAlarmEnable(timer);
}

uint32_t _FreqCountESP::read()
{
  sIsFrequencyReady = false;
  return sFrequency;
}

uint8_t _FreqCountESP::available()
{
  return sIsFrequencyReady;
}

void _FreqCountESP::end()
{

}

_FreqCountESP FreqCountESP;
