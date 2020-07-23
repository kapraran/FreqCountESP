#ifndef FreqCountESP_h
#define FreqCountESP_h

#include <Arduino.h>

void IRAM_ATTR onCount();
void IRAM_ATTR onTimer();

class _FreqCountESP
{
private:
  uint8_t mPin;
  uint16_t mTimerMs;
  hw_timer_t *mTimer;

public:
  static volatile uint8_t sIsFrequencyReady;
  static volatile uint32_t sCount;
  static volatile uint32_t sFrequency;

  static portMUX_TYPE sCountMux;
  static portMUX_TYPE sTimerMux;

  _FreqCountESP();
  ~_FreqCountESP();

  void begin(uint8_t pin, uint16_t timerMs);
  uint32_t read();
  uint8_t available();
  void end();
};

extern _FreqCountESP FreqCountESP;

#endif // FreqCountESP_h
