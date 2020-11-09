#include <Wire.h>

const int potentiometer0 = A0;
const int potentiometer1 = A1;
const int potentiometer2 = A2;

int potentiometer0_Value = 0;
int potentiometer1_Value = 0;
int potentiometer2_Value = 0;

void print_4_digi(int value)
{
  // 1234

  Serial.print(value % 10);
  value /= 10;
  Serial.print(value % 10);
  value /= 10;
  Serial.print(value % 10);
  value /= 10;
  Serial.print(value % 10);
  value /= 10;

  //4, 3, 2, 1
}

void setup() 
{  
  
  Serial.begin(9600);
  pinMode(3, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);

}

void loop() 
{
  
  potentiometer0_Value = analogRead(potentiometer0);
  potentiometer1_Value = analogRead(potentiometer1);
  potentiometer2_Value = analogRead(potentiometer2);

  print_4_digi(potentiometer0_Value);
  print_4_digi(potentiometer1_Value);
  print_4_digi(potentiometer2_Value);

  if (digitalRead(3) != digitalRead(7))
  {
    Serial.print(digitalRead(3));
  }
  else
  {
    Serial.print(0);
  }
  Serial.print('D');
  Serial.print('E');
  Serial.print('F');

  delay(100);
}
