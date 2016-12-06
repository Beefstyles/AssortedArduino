const int ldrPin_1 = A0; //LDR 1 analog connection
const int ldrPin_2 = A1; //LDR 2 analog connection
const int ldrPin_3 = A2; //LDR 3 analog connection
const int delayPin = A5; //Delay timer pot
const int relayPin = 6; //Led pin that lights up on trip
int lightResSensitivity_1 = 900, //LDR 1 cut off value
    lightResSensitivity_2 = 900, //LDR 2 cut off value
    lightResSensitivity_3 = 900; //LDR 3 cut off value
int lightRes_1, lightRes_2, lightRes_3; //Value for light sensor
long delayTimerPotValue;

void setup()
{
  Serial.begin(9600); //Set up serial for testing
  lightRes_1 = analogRead(ldrPin_1); //Read ldr1 and output a value
  lightRes_2 = analogRead(ldrPin_2); //Read ldr2 and output a value
  lightRes_3 = analogRead(ldrPin_3); //Read ldr3 and output a value
  SetLightResSens();
  pinMode(relayPin, OUTPUT); //Testing pin
}

void SetLightResSens()
{
  lightResSensitivity_1 = lightRes_1 + 15; //Sets LDR 1 cut off value - Min 730
  lightResSensitivity_2 = lightRes_2 + 15; //Sets LDR 2 cut off value - Min 880
  lightResSensitivity_3 = lightRes_3 + 15; //Sets LDR 3 cut off value - Min 775
}

void loop()
{
  delayTimerPotValue = analogRead(delayPin); //Reads pot
  /*Serial.print("Value is :");
  Serial.println(delayTimerPotValue); 
  */ 
  delayTimerPotValue = map(delayTimerPotValue, 0, 1023, 40000, 2000); //Maps the pot value from 40 seconds to 2 seconds
  /*Serial.print("Delay set to: "); //Troubleshooting
  Serial.print(delayTimerPotValue / 1000);
  Serial.println(" seconds");
  */
  lightRes_1 = analogRead(ldrPin_1); //Read ldr1 and output a value
  lightRes_2 = analogRead(ldrPin_2); //Read ldr2 and output a value
  lightRes_3 = analogRead(ldrPin_3); //Read ldr3 and output a value
  /*Serial.print("LR1 "); //Only needed for testing - can uncomment
  Serial.print(lightRes_1);
  Serial.println();
  Serial.print("LR2 ");
  Serial.print(lightRes_2);
  Serial.println();
  Serial.print("LR3 ");
  Serial.print(lightRes_3);
  Serial.println();
  */

  if(lightRes_1 >= lightResSensitivity_1 || // || means OR
  lightRes_2 >= lightResSensitivity_2 || 
  lightRes_3 >= lightResSensitivity_3
  )
  {
    Serial.println("Light sensor triggered");
    digitalWrite(relayPin, HIGH); //Triggers relay
    //Function logic on trip
    if(lightRes_1 >= lightResSensitivity_1)
    {
      Serial.print("LR1 tripped above ");
      Serial.println(lightResSensitivity_1);
    }
    
    if(lightRes_2 >= lightResSensitivity_2)
    {
      Serial.print("LR2 tripped above ");
      Serial.println(lightResSensitivity_2);
    }
    
    if(lightRes_3 >= lightResSensitivity_3)
    {
    Serial.print("LR3 tripped above ");
    Serial.println(lightResSensitivity_3);
    }
  
    delay(delayTimerPotValue); //Delays and doesn't read - Set by delayTimerPotValue
    digitalWrite(relayPin, LOW);
  }
  delay(1);
}


