#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include <PCA9685.h>
#include <ExponentialSmoothFunc.h>



PCA9685 pwm = PCA9685(0x40);

#define SERVOMIN 150
#define SERVOMAX 600 //サーボのパルス幅を設定

LSM9DS1 imu;  //LSM9DS1のオブジェクトを作成

#define PRINT_SPEED 50  //計算の周期 ms

const long REACT_RATE = 12;
ExponentialSmooth expSmooth( REACT_RATE );

static unsigned long lastPrint = 0;


void printAccel();

void setup()
{
  pwm.begin();                   //初期設定
  pwm.setPWMFreq(60);            //PWM周期を60Hzに設定

  Serial.begin(115200);         // テスト用途のシリアル通信

  Wire.begin();                  // I2Cを開く
  if (imu.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
  {
    while (1);
  }

}

void loop()
{

  
  if ( imu.accelAvailable() )
  {
    imu.readAccel();
  }


   const long SMOOTHED_DATA_z = expSmooth((int)(imu.calcAccel(imu.az)*1000));

    Serial.print("SA: ");
    Serial.print(((SMOOTHED_DATA_z)/1000), 2);
    Serial.println(" g");


    
    printAccel();

    Serial.println();

    
    Serial.print(millis()-lastPrint);Serial.println("ms");
    
    lastPrint = millis(); // 前回の時間を更新
/*
 if ((lastPrint + PRINT_SPEED) < millis())
  {
    
   const long SMOOTHED_DATA_z = expSmooth((int)(imu.calcAccel(imu.az)*1000));

    Serial.print("SA: ");
    Serial.print(((SMOOTHED_DATA_z)/1000), 2);
    Serial.println(" g");


    
    printAccel();

    Serial.println();

    
    Serial.print(millis()-lastPrint);Serial.println("ms");
    
    lastPrint = millis(); // 前回の時間を更新
    
  }

*/
}


void printAccel()
{

  Serial.print("A: ");

  Serial.print(imu.calcAccel(imu.ax), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.ay), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.az), 2);
  Serial.println(" g");

}

void servo_write(int ch, int ang)
{ //動かすサーボチャンネルと角度を指定
  ang = map(ang, 0, 180, SERVOMIN, SERVOMAX); //角度（0～180）をPWMのパルス幅（150～600）に変換
  pwm.setPWM(ch, 0, ang);
}
