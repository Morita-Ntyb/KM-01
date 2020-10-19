 #include <Wire.h>
#include <SPI.h>
#include <PCA9685.h>
#include <SparkFunLSM9DS1.h>



PCA9685 pwm = PCA9685(0x40);//オブジェクト, アドレス指定

#define SERVOMIN 150 //1周期4096(ステップ)に対するサーボのパルス幅(ステップ)
#define SERVOMAX 500 //
#define PWMFREQ 50  //PWM周期

LSM9DS1 imu;  //LSM9DS1のオブジェクトを作成

#define PRINT_SPEED 20  //計算の周期 ms

static unsigned long lastPrint = 0;


//プロトタイプ宣言
void printAccel();
void shiftArray();
double average(const double*, int);

void startUpSequence();


#define filterPoint 15  //移動平均フィルタのポイント数

//移動平均フィルタ用のバッファー
double xAccelBuffer[filterPoint];
double yAccelBuffer[filterPoint];
double zAccelBuffer[filterPoint];

static double a1 = 76.4952;
static double a2 = 105.8476 ;
static double a3 = 87.7931;
static double b1 = 0;
static double b2 = 105.8476 ;
static double b3 = 0;
static double c1 = 219.1524;

static double d1 = -76.4952;
static double d2 = 105.8476 ;
static double d3 = -87.7931;
static double e1 = 0;
static double e2 = 105.8476 ;
static double e3 = 0;
static double f1 = 219.1524;

static double g1 = -76.4952;
static double g2 = 105.8476 ;
static double g3 = -87.7931;
static double h1 = 0;
static double h2 = 105.8476 ;
static double h3 = 0;
static double i1 = 219.1524;

static double j1 = -84.8004;
static double j2 = 196.5143 ;
static double j3 = -97.3249;
static double k1 = 0;
static double k2 = 96.5476 ;
static double k3 = 0;
static double l1 = 200.4857;

void setup()
{
  pwm.begin();                   //初期設定
  pwm.setPWMFreq(PWMFREQ);       //PWM周期を設定

  Serial.begin(115200);         // テスト用途のシリアル通信

  Wire.begin();                  // I2Cを開く
  
  if (imu.begin() == false) // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
  {
    while (1);
  }

  startUpSequence();

}

void loop()
{

  
  if ( imu.accelAvailable() )
  {
    imu.readAccel(); //IMUの値を更新
  }


 if ((lastPrint + PRINT_SPEED) < millis())
  {
    
    shiftArray(); //配列を一つ後ろにずらす

    //配列の先頭にセンサー値を代入する
    xAccelBuffer[0] = imu.calcAccel(imu.ax);
    yAccelBuffer[0] = imu.calcAccel(imu.ay);
    zAccelBuffer[0] = imu.calcAccel(imu.az);
    
    printAccel();
    
    double xFAccel = average(xAccelBuffer, filterPoint);
    double yFAccel = average(yAccelBuffer, filterPoint);
    double zFAccel = average(zAccelBuffer, filterPoint);

    Serial.print(xFAccel, 2);
    Serial.print(", ");
    Serial.print(yFAccel, 2);
    Serial.print(", ");
    Serial.print(zFAccel, 2);
    Serial.println();
    Serial.println();

    double servoDeg1 = a1*xFAccel*xFAccel*xFAccel + a2*xFAccel*xFAccel + a3*xFAccel
                     + b1*zFAccel*zFAccel*zFAccel + b2*zFAccel*zFAccel + b3*zFAccel + c1;
    double servoDeg2 = d1*yFAccel*yFAccel*yFAccel + d2*yFAccel*yFAccel + d3*yFAccel
                     + e1*zFAccel*zFAccel*zFAccel + e2*zFAccel*zFAccel + e3*zFAccel + f1;

    double servoDeg3 = g1*xFAccel*xFAccel*xFAccel + g2*xFAccel*xFAccel + g3*xFAccel
                     + h1*zFAccel*zFAccel*zFAccel + h2*zFAccel*zFAccel + h3*zFAccel + i1;
    double servoDeg4 = j1*yFAccel*yFAccel*yFAccel + j2*yFAccel*yFAccel + j3*yFAccel
                     + k1*zFAccel*zFAccel*zFAccel + k2*zFAccel*zFAccel + k3*zFAccel + l1;
                     
    pwm.setPWM(1, 0, servoDeg1);
    pwm.setPWM(0, 0, servoDeg2);
    pwm.setPWM(3, 0, servoDeg3);
    pwm.setPWM(2, 0, servoDeg4);
    
    lastPrint = millis(); // 前回の時間を更新

  }


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
    

void printAccel()
{

  Serial.print(imu.calcAccel(imu.ax), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.ay), 2);
  Serial.print(", ");
  Serial.print(imu.calcAccel(imu.az), 2);
  Serial.print(", ");

}


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
}
