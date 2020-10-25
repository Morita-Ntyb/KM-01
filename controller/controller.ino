const int potentiometer0 = A0;
const int potentiometer1 = A1;
const int potentiometer2 = A2;

int potentiometer0_Value = 0;
int potentiometer1_Value = 0;
int potentiometer2_Value = 0;

void setup() 
{  
  
  Serial.begin(9600);

}

void loop() 
{
  
  potentiometer0_Value = analogRead(potentiometer0);
  potentiometer1_Value = analogRead(potentiometer1);
  potentiometer2_Value = analogRead(potentiometer2);

  Serial.print(potentiometer0_Value);
  Serial.print(",");
  Serial.print(potentiometer0_Value);
  Serial.print(",");
  Serial.println(potentiometer0_Value);

  delay(2); //ADC待機のため
  
}
