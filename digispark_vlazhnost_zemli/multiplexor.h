//int humidity;
#define channelA 1
#define channelB 2
#define channelC 4

// 1 - A, 2 - B, 3 - C
int readChannel(int channelNum) {
  if (channelNum == 1) {
    digitalWrite(channelA, LOW);
    digitalWrite(channelB, LOW);
    digitalWrite(channelC, LOW);
    return (analogRead(A0));
  }
  if (channelNum== 2) {
    digitalWrite(channelA, HIGH);
    digitalWrite(channelB, LOW);
    digitalWrite(channelC, LOW);
    return (analogRead(A0));
  }
  if (channelNum== 3) {
    digitalWrite(channelA, LOW);
    digitalWrite(channelB, HIGH);
    digitalWrite(channelC, LOW);
    return (analogRead(A0));
  }
  return -1;

}
