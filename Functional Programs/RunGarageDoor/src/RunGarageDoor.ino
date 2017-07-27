/*
 * Project RunGarageDoor
 * Description: Functional testing for opening a garage door using a relay in parallel with the garage door opener
 * Author: Trevor T
 * Date: 7-26-17
 *
 * NOTE: Need to find a sensor or connection on the opener that would allow us to verify the garage door's position
 */

int toggleGarageDoor(String command);
void sendToggleCommand();

bool isOpen;
int timeoutBegan;
int timeout = 12; // Number of seconds to allow garage door to move

void setup() {
  pinMode(D1, OUTPUT);
  Particle.function("toggleDoor", toggleGarageDoor);
  isOpen = false; // Assuming the program begins when the door is down. Replace bool with sensor.
  timeoutBegan = 0;
}

void loop() {

}


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
  digitalWrite(D1, HIGH);
  delay(500);
  digitalWrite(D1, LOW);
}
