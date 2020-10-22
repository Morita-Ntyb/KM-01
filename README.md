# KM-01
ミミがうごくヘッドホン

一つのミミにつきサーボ2つ(2軸) ヨーとピッチ方向で左右合計4軸を制御  
IMU(加速度センサ)の値を読み取り, サーボの角度に反映させる

# ハードウェア構成

## マイコン
Arduino UNO R3  
5V

## サーボ用PWM
PCA 9685 (秋月電子)  
https://akizukidenshi.com/catalog/g/gK-10350/  
16チャネル I2C接続 電源 5V サーボ用別系統5V

## IMU
SFE-SEN-13944 (スイッチサイエンス)  
https://www.switch-science.com/catalog/2881/
9Dof I2C接続 電源は3.3V

## サーボ(テスト用として)
FS90  
https://akizukidenshi.com/download/ds/feetech/fs90.pdf  
9gサーボ 180度

## バッテリー
Anker PowerCore+ 26800 PD  
PD対応(5V = 3A, 9V = 3A, 15V = 2A, 20V = 1.5A)　約100Wh  
Type-A - 2.1mm DCケーブルを自作する 

## 使用ライブラリ

PCA9685  https://akizukidenshi.com/catalog/g/gK-10350/  

SFE-SEN-13944 https://github.com/sparkfun/SparkFun_LSM9DS1_Arduino_Library  


## I2C アドレス設定
PCA 9685  0x40  
SFE-SEN-13944 0x6B 0x1E


# タスクリスト


* パーツを仮接続する ○

* パーツの動作試験用ソフトを完成させる I2Ctest ○

* 左側のミミだけ動かすソフトを完成させる proto_left ○
  * map 関数を使用しない実装 ○

* 両側のミミを動かすソフトを完成させる ○

10/02 完了　https://twitter.com/Morita_Ntyb/status/1311973911555309568


* 展示用のヘッドホン, フレーム部分のモデリング, プリント 組付け
  * フレームをFusion360で設計する ○
  * プリント ○
  * サーボ, IMUを取り付ける ○


10/20 完了


* ミミの形状を考える モデリング, プリント ○

* 配線する
  * Type-A - 2.1mm DCケーブルを作る ○

* 振動が起きる問題の修正
* y軸周りに回転させたとき下段サーボの動きが大きい問題の修正


* ジャイロセンサーの値をサーボの角度に反映させる
* I2Cについて調べる
* IMU(3.3V)とArduino(5V)間のI2Cにレベル変換回路を実装する https://www.switch-science.com/catalog/2396/  ○
* 電源スイッチを追加
* ロータリーエンコーダー(インクリメント)や可変抵抗でミミの角度を調整する機能を実装

以上

# ソフトウェア △

## I2Ctest ○

### サーボとIMUの動作を確認 I2C, ライブラリの使い方 ○
完成 確認

## proto_left ○

左側のみの動作

* IMUのデータに移動平均フィルタをかける ○
* 加速度センサのデータ(3次元ベクトル)から, ミミができるだけ垂直(+初期のオフセット)を維持するサーボの角度を計算する ○
    * 3変数関数を2つ(片側2軸)用意する ○

## exhibit ○

### proto_leftを基本に両方が動くようにする ○
