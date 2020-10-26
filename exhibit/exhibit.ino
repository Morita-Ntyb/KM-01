#include <Wire.h>
#include <SPI.h>
#include <PCA9685.h>
#include <SparkFunLSM9DS1.h>



PCA9685 pwm = PCA9685(0x40);//オブジェクト, アドレス指定

#define SERVOMIN 205 //1周期4096(ステップ)に対するサーボのパルス幅(ステップ)
#define SERVOMAX 983 //
#define PWMFREQ 50  //PWM周期

LSM9DS1 imu;  //LSM9DS1のオブジェクトを作成


//プロトタイプ宣言
void printAccel();
void shiftArray();
double average(const double*, int);

void startUpSequence();

#define filterPoint 80  //移動平均フィルタのポイント数

//移動平均フィルタ用のバッファー
double xAccelBuffer[filterPoint];
double yAccelBuffer[filterPoint];
double zAccelBuffer[filterPoint];

static double a1 = -170.2564;
static double a2 = 195.3476;
static double a3 = -195.4023;
static double b1 = 0;
static double b2 = 195.3476;
static double b3 = 0;
static double c1 = 398.1524;


static double d1 = -170.2564;
static double d2 = 195.3476;
static double d3 = -195.4023;
static double e1 = 0;
static double e2 = 195.3476;
static double e3 = 0;
static double f1 = 398.1524;

static double g1 = 170.2564;
static double g2 = 195.3476;
static double g3 = 195.4023;
static double h1 = 0;
static double h2 = 195.3476;
static double h3 = 0;
static double i1 = 398.1524;

static double j1 = -170.2564;
static double j2 = 195.3476;
static double j3 = -195.4023;
static double k1 = 0;
static double k2 = 195.3476;
static double k3 = 0;
static double l1 = 398.1524;

char serial_string[25];
int serial_counter = 0;

void setup()
{
  pwm.begin();                   //初期設定
  pwm.setPWMFreq(PWMFREQ);       //PWM周期を設定

  Wire.begin();                  // I2Cを開く
  
  if (imu.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
  {
    while (1);
  }

  //startUpSequence();

}

void loop()
{

  
  if ( imu.accelAvailable() )
  {
    imu.readAccel(); //IMUの値を更新
  }

  if (Serial.available() > 0)
  { 
    serial_string[serial_counter] = Serial.readStringUntil("\n");

    if (serial_string[serial_counter] == '\0')
    {
      
      serial_counter = 0;
    }
    else
    {
      serial_counter ++ ;
    }
  }

  shiftArray(); //配列を一つ後ろにずらす

  //配列の先頭にセンサー値を代入する
  xAccelBuffer[0] = imu.calcAccel(imu.ax);
  yAccelBuffer[0] = imu.calcAccel(imu.ay);
  zAccelBuffer[0] = imu.calcAccel(imu.az);

  double xFAccel = average(xAccelBuffer, filterPoint);
  double yFAccel = average(yAccelBuffer, filterPoint);
  double zFAccel = average(zAccelBuffer, filterPoint);


  double servoDeg1 = a1*xFAccel*xFAccel*xFAccel + a2*xFAccel*xFAccel + a3*xFAccel
                   + b1*zFAccel*zFAccel*zFAccel + b2*zFAccel*zFAccel + b3*zFAccel + c1;
  double servoDeg2 = d1*yFAccel*yFAccel*yFAccel + d2*yFAccel*yFAccel + d3*yFAccel
                   + e1*zFAccel*zFAccel*zFAccel + e2*zFAccel*zFAccel + e3*zFAccel + f1;

  double servoDeg3 = g1*yFAccel*yFAccel*yFAccel + g2*yFAccel*yFAccel + g3*yFAccel
                   + h1*zFAccel*zFAccel*zFAccel + h2*zFAccel*zFAccel + h3*zFAccel + i1;
  double servoDeg4 = j1*xFAccel*xFAccel*xFAccel + j2*xFAccel*xFAccel + j3*xFAccel
                   + k1*zFAccel*zFAccel*zFAccel + k2*zFAccel*zFAccel + k3*zFAccel + l1;
                   
  pwm.setPWM(0, 0, (int)((servoDeg1+0.5)/2.0));
  pwm.setPWM(2, 0, (int)((servoDeg2+0.5)/2.0));
  pwm.setPWM(13, 0, (int)((servoDeg3+0.5)/2.0));
  pwm.setPWM(15, 0, (int)((servoDeg4+0.5)/2.0));
  

}

void shiftArray()
{
  
  
  for(int i=filterPoint -1 ; i > 0; --i)
  {
    xAccelBuffer[i] = xAccelBuffer[ i-1 ];
  }
  
  for(int i=filterPoint -1 ; i > 0; --i)
  {
    yAccelBuffer[i] = yAccelBuffer[ i-1 ];
  }
  
  for(int i=filterPoint -1 ; i > 0; --i)
  {
    zAccelBuffer[i] = zAccelBuffer[ i-1 ];
  }
  
}


double average(const double* array, int size)
{
    double result = 0;
    for (int i = 0; i < size; ++i) 
    {
        result += array[i];
    }
    return (result / (double)size ) ;
}
    

/*
void startUpSequence()
{
    pwm.setPWM(1, 0, servoDeg1); //左 さげ
    delay(450);
    pwm.setPWM(0, 0, servoDeg2); //右 さげ
    delay(450);
    
    pwm.setPWM(3, 0, servoDeg3); //左 あげ
    delay(450);
    pwm.setPWM(2, 0, servoDeg4); //右 あげ
    delay(450);

    pwm.setPWM(2, 0, servoDeg4); //左 さげ
    pwm.setPWM(2, 0, servoDeg4); //右 さげ
    delay(240);
    pwm.setPWM(2, 0, servoDeg4); //左 あげ
    pwm.setPWM(2, 0, servoDeg4); //右 あげ

    delay(220);
    delay(500);

    pwm.setPWM(2, 0, servoDeg4); //左 あげ
    pwm.setPWM(2, 0, servoDeg4); //右 あげ

    delay(1100);
    

    //ゆっくり上げ(チルト) 1500ms

    delay(900);

    pwm.setPWM(2, 0, servoDeg4); //左 さげ
    pwm.setPWM(2, 0, servoDeg4); //右 さげ
    //140
}*/
