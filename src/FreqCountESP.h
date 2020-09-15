#ifndef kapraran_FreqCountESP_h
#define kapraran_FreqCountESP_h

#include <Arduino.h>

void IRAM_ATTR onRise();
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

  static portMUX_TYPE sMux;

  _FreqCountESP();
  ~_FreqCountESP();

  void begin(uint8_t pin, uint16_t timerMs, uint8_t hwTimerId = 0, uint8_t mode = INPUT);
  uint32_t read();
  uint8_t available();
  void end();
};

extern _FreqCountESP FreqCountESP;

#endif // kapraran_FreqCountESP_h
