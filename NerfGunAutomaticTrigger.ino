const int switchPin = 5; 
const int motorPin = 6;
const int loadPin_Forward = 7;
const int loadPin_Backward = 8;
int startMotorDelay = 1000, 
    forwardLoadDelay = 500,
    reverseLoadDelay = 500; 
int switchPinValue;
bool fireOn;

void setup()
{
  Serial.begin(9600); //Set up serial for testing
  pinMode(switchPin, INPUT);
  pinMode(motorPin, OUTPUT);
  pinMode(loadPin_Forward, OUTPUT);
  pinMode(loadPin_Backward, OUTPUT);
  fireOn = false;
}

void loop()
{
  switchPinValue = digitalRead(switchPin);
  
  if(switchPinValue == LOW)
  {
  //Serial.println(switchPinValue);
  if(fireOn)
  {
    fireOn = false;
  }
  }

  else if (switchPinValue == HIGH && !fireOn)
  {
      Serial.println("Starting firing sequence");
      digitalWrite(motorPin, HIGH);
      Serial.print("Motor Pin has been set to: ");
      Serial.println("HIGH");
      Serial.print("Delaying this many milliseconds: ");
      Serial.println(startMotorDelay);
      delay(startMotorDelay);
      digitalWrite(loadPin_Forward, HIGH);
      Serial.print("LoadPin Forward has been set to: ");
      Serial.println("HIGH");
      Serial.print("Delaying this many milliseconds: ");
      Serial.println(forwardLoadDelay);
      delay(forwardLoadDelay);
      digitalWrite(loadPin_Forward, LOW);
      Serial.print("LoadPin Forward has been set to: ");
      Serial.println("LOW");
      digitalWrite(motorPin, LOW);
      Serial.print("Motor Pin has been set to: ");
      Serial.println("LOW");
      digitalWrite(loadPin_Backward, HIGH);
      Serial.print("LoadPin Backward has been set to: ");
      Serial.println("HIGH");
      Serial.print("Delaying this many milliseconds: ");
      Serial.println(reverseLoadDelay);
      delay(reverseLoadDelay);
      digitalWrite(loadPin_Backward, LOW);
      Serial.print("LoadPin Backward has been set to: ");
       Serial.println("LOW");
      fireOn = true;
      Serial.print("FireOn is: ");
      Serial.println("TRUE");
      Serial.println("Finished firing sequence");
  }
  delay(1);
}


