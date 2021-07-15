#if !defined(DIGISPARK)
#define DIGISPARK 1
int pins_read[] = { 1, 3, 2, 0};
#define analogRead_(pin) analogRead(pins_read[pin-2])
#endif
