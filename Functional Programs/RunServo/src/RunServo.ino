/*
 * Project RunServo
 * Description: Functional testing for servo motor to actuate a lightswitch
 * Author: Trevor T
 * Date: 7-25-17
 */

 bool toggleSwitch(String command);

 int neutralAngle = 15;
 int openAngle = 74;
 int closeAngle = -40;

 void setup() {
     pinMode(D0, OUTPUT);
     Particle.function("toggleSwitch", toggleSwitch);
 }

 void loop() {

 }


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

     if(command == "Open")
     {
         for(int i = 0; i < 75; i++)
         {
             digitalWrite(D0, HIGH);
             delayMicroseconds(openHighTime);
             digitalWrite(D0, LOW);
             delayMicroseconds(openLowTime);
         }
     }
     if(command == "Close")
     {
         for(int i = 0; i < 75; i++)
         {
             digitalWrite(D0, HIGH);
             delayMicroseconds(closeHighTime);
             digitalWrite(D0, LOW);
             delayMicroseconds(closeLowTime);
         }
     }

     for(int i = 0; i < 75; i++)
     {
         digitalWrite(D0, HIGH);
         delayMicroseconds(neutralHighTime);
         digitalWrite(D0, LOW);
         delayMicroseconds(neutralLowTime);
     }
     return true;
 }
