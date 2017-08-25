/*
 * Project UnitProgram
 * Description: Program to control elements in the garage
 * Author: Trevor T
 * Date: 8-5-17
 */

 int toggleGarageDoor(String command);
 void sendToggleCommand();
 void checkAndLogDoorPosition();
 bool toggleSwitch(String command);
 int lTemp(String command);

int timeAlive = 0;
int garagePin = C3;
int sideDoorPin = D6;
int tempPin = A0;
int lightPin = D7;

// Variables for controlling Garage Door
bool isOpen;
int timeoutBegan;
int timeout = 12;

// Variables for logging side Door
unsigned long timeLastOccurred;
int doorDelay = 1.5;
String eventName;
bool queueTheEvent;

// Variables for logging temperature
double tempC = 0;

// Variables for controlling lights
int neutralAngle = 15;
int openAngle = 74;
int closeAngle = -40;

void setup() {
  // Setup Garage Door
  pinMode(garagePin, OUTPUT);
  Particle.function("toggleDoor", toggleGarageDoor);
  isOpen = false;
  timeoutBegan = 0;

  // Setup Side Door
  queueTheEvent = false;
  timeLastOccurred = 0;
  pinMode(sideDoorPin, INPUT);
  interrupts();
  bool attached = attachInterrupt(sideDoorPin, checkAndLogDoorPosition, CHANGE);
  if(!attached)
    Particle.publish("InterruptError", "Not attached", 60, PRIVATE);

  // Setup temperature logging
  Particle.function("logTemp", lTemp);
  Particle.variable("tempC", tempC);
  Particle.variable("timeAlive", timeAlive);

  // Setup for controlling lights
  pinMode(lightPin, OUTPUT);
  Particle.function("toggleSwitch", toggleSwitch);
}


void loop() {
  // Side Door
  if(queueTheEvent)
  {
    Particle.publish(eventName, NULL, 60, PRIVATE);
    queueTheEvent = false;
  }

  /*// Temperature
  int readAnalogValue = analogRead(tempPin);
  double readVoltage = 3.3 * ((double)readAnalogValue / 4095.0);

  tempC = 100 * readVoltage - 50;*/

  timeAlive = millis();

  delay(500);
}


// GarageDoor functions
/*
* Returns Positive Numbers if call was successful without action
* Returns Negative Numbers if call was invalid or unsuccessful
*
* Returns -2 if timeout phase hadn't expired
* Returns -1 if command was invalid
* Returns 0 if successful
* Returns 1 if no action was needed
* Returns
*/

int toggleGarageDoor(String command)
{
  // Check if we're still in a timeout
  if(millis() - timeoutBegan < timeout*1000)
    return -2;
  if(command != "Open" && command != "Close")
    return -1;
  // Check if action needs to be taken, and command legitimacy
  if(command == "Open" && isOpen)
    return 1;
  else if(command == "Close" && !isOpen)
    return 1;

  // Toggle the relay to begin moving the garage door
  sendToggleCommand();
  timeoutBegan = millis();
  isOpen = !isOpen;
  return 0;
}

void sendToggleCommand()
{
  digitalWrite(garagePin, HIGH);
  delay(1000);
  digitalWrite(garagePin, LOW);
}


// Side Door functions
void checkAndLogDoorPosition() {
  if(millis() - timeLastOccurred < doorDelay*1000)
  {
    eventName = "timeout";
    queueTheEvent = true;
    return;
  }

  bool didOpen = digitalRead(sideDoorPin) == LOW ? true : false;

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

// Light functions
bool toggleSwitch(String command)
 {
     int oA = openAngle + 90;
     int nA = neutralAngle + 90;
     int cA = closeAngle + 90;

     int openHighTime = ((double)oA/180.0)*1000 + 1000;
     int openLowTime = 20000 - openHighTime;

     int neutralHighTime = ((double)nA/180.0)*1000 + 1000;
     int neutralLowTime = 20000 - neutralHighTime;

     int closeHighTime = ((double)cA/180.0)*1000 + 1000;
     int closeLowTime = 20000 - openHighTime;

     if(command == "On")
     {
         for(int i = 0; i < 75; i++)
         {
             digitalWrite(lightPin, HIGH);
             delayMicroseconds(openHighTime);
             digitalWrite(lightPin, LOW);
             delayMicroseconds(openLowTime);
         }
     }
     if(command == "Off")
     {
         for(int i = 0; i < 75; i++)
         {
             digitalWrite(lightPin, HIGH);
             delayMicroseconds(closeHighTime);
             digitalWrite(lightPin, LOW);
             delayMicroseconds(closeLowTime);
         }
     }

     for(int i = 0; i < 75; i++)
     {
         digitalWrite(lightPin, HIGH);
         delayMicroseconds(neutralHighTime);
         digitalWrite(lightPin, LOW);
         delayMicroseconds(neutralLowTime);
     }
     return true;
 }


 // Log Temperature functions

int lTemp(String command)
{
  int sumOfAnalog = 0;
  for(int i = 0; i < 10; i++)
  {
    sumOfAnalog += analogRead(tempPin);
    delay(100);
  }
  int avgAnalogValue = sumOfAnalog / 10;
  double readVoltage = 3.3 * ((double)avgAnalogValue / 4095.0);
  tempC = 100 * readVoltage - 50;

  bool success = Particle.publish("Temp", String(tempC), 60, PRIVATE);
    return 0;
}
