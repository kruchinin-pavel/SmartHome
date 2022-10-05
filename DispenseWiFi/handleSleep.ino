#if not defined(HANDLESLEEP_H)
#define HANDLESLEEP_H 1
long next_sleep = -1;
void check_sleep() {
  auto currentMilis = millis();
  if (next_sleep < 0) {
    next_sleep = currentMilis + 20000;
  }
  if (next_sleep < currentMilis) {
    // sleep 30 seconds == 30_000ms == 30_000_000microsecoonds
    next_sleep = currentMilis + 20000;
    ESP.deepSleep(30e6);
  }
}

#endif
