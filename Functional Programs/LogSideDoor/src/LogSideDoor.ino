/*
 * Project LogSideDoor
 * Description: Functional program to log whenever the side door is opened or closed
 * Author: Trevor T
 * Date: 7-26-17
 */

void checkAndLogDoorPosition();
unsigned long timeLastOccurred;
int doorDelay = 1.5;  // Minimum time between door opening and closing (prevent bounce)

String eventName;
bool queueTheEvent;

void setup() {
  queueTheEvent = false;
  timeLastOccurred = 0;
  pinMode(D2, INPUT);
  interrupts();
  bool attached = attachInterrupt(D2, checkAndLogDoorPosition, CHANGE);
  if(!attached)
    Particle.publish("InterruptError", "Not attached", 60, PRIVATE);

  Particle.publish("SetupComplete", NULL, 60, PRIVATE);
}

void loop() {
  if(queueTheEvent)
  {
    Particle.publish(eventName, NULL, 60, PRIVATE);
    queueTheEvent = false;
  }
}

void checkAndLogDoorPosition() {
  if(millis() - timeLastOccurred < doorDelay*1000)
  {
    eventName = "timeout";
    queueTheEvent = true;
    return;
  }

  bool didOpen = digitalRead(D2) == LOW ? true : false;

  if(didOpen)
  {
    eventName = "Door-Opened";
  }
  else
  {
    eventName = "Door-Closed";
  }
  timeLastOccurred = millis();
  queueTheEvent = true;
}
