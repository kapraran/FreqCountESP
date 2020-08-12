#include "FreqCountESP.h"

volatile uint8_t _FreqCountESP::sIsFrequencyReady = false;
volatile uint32_t _FreqCountESP::sCount = 0;
volatile uint32_t _FreqCountESP::sFrequency = 0;

portMUX_TYPE _FreqCountESP::sMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onRise()
{
  portENTER_CRITICAL_ISR(&_FreqCountESP::sMux);
  _FreqCountESP::sCount++;
  portEXIT_CRITICAL_ISR(&_FreqCountESP::sMux);
}

void IRAM_ATTR onTimer()
{
  portENTER_CRITICAL_ISR(&_FreqCountESP::sMux);
  _FreqCountESP::sFrequency = _FreqCountESP::sCount;
  _FreqCountESP::sCount = 0;
  _FreqCountESP::sIsFrequencyReady = true;
  portEXIT_CRITICAL_ISR(&_FreqCountESP::sMux);
}

_FreqCountESP::_FreqCountESP()
{
  mTimer = NULL;
}

_FreqCountESP::~_FreqCountESP()
{
  end();
}

void _FreqCountESP::begin(uint8_t pin, uint16_t timerMs, uint8_t hwTimerId, uint8_t mode)
{
  mPin = pin;
  mTimerMs = timerMs;
  sIsFrequencyReady = false;
  sCount = 0;
  sFrequency = 0;

  pinMode(mPin, mode);

  attachInterrupt(mPin, &onRise, RISING);

  mTimer = timerBegin(hwTimerId, 80, true);
  timerAttachInterrupt(mTimer, &onTimer, true);
  timerAlarmWrite(mTimer, mTimerMs * 1000, true);
  timerAlarmEnable(mTimer);
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
  detachInterrupt(mPin);

  timerAlarmDisable(mTimer);
  timerDetachInterrupt(mTimer);
  timerEnd(mTimer);
}

_FreqCountESP FreqCountESP;
