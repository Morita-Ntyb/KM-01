# KM-01
ミミがうごくヘッドホン

# ハードウェア構成

## マイコン
Arduino UNO R3  
5V

## サーボ用PWM
PCA 9685 (秋月電子)
https://akizukidenshi.com/catalog/g/gK-10350/
16チャネル I2C IC 5V サーボ用別系統5V

## IMU
SFE-SEN-13944 (スイッチサイエンス)
https://www.switch-science.com/catalog/2881/
9Dof I2C IC 3.3V

## サーボ(テスト用として)
FS90  
9gサーボ 180度

## バッテリー
PowerCore+ 26800 PD
パワーデリバリー対応(5V = 3A, 9V = 3A, 15V = 2A, 20V = 1.5A)　約100Wh  
Type-A - 2.1mm DCケーブルを自作する 

## 使用ライブラリ

PCA9685 秋月の商品ページから https://akizukidenshi.com/catalog/g/gK-10350/  

SFE-SEN-13944 https://github.com/sparkfun/SparkFun_LSM9DS1_Arduino_Library  

ExponentialSmoothFunc https://github.com/ttatsf/ExponentialSmoothFunc
Copyright (c) 2016 ttatsf
Released under the MIT license
https://opensource.org/licenses/mit-license.php  



## I2C アドレス設定
PCA 9685  0x40  
SFE-SEN-13944 0x6B 0x1E


# タスクリスト


* パーツを接続する ○

* パーツの動作試験用ソフトを完成させる I2Ctest ○

* 左側のミミだけ動かすソフトを完成させる proto_left

* 両側のミミを動かすソフトを完成させる


上記を10月1週目までに


* I2Cのプルアップ抵抗について調べる 現在(09/26)IMUとPCA9685の両方に接続されている.
* I2Cをフラットケーブルで1.5mほど延長できるか確かめる
    * 延長できそうなら  
      腰にArduinoとバッテリー, ヘッドホン側にIMUとPCA9685
    * できなさそう  
        * RCAケーブル確かめる or 腰につけるのはバッテリーのみ

* 展示用のヘッドホン, フレーム部分のモデリング, プリント 組付け


上記を10月2週目までに


* ミミの形状を考える モデリング, プリント

* バッテリーを腰につけるマウンタをモデリング, プリント


以上

# 制御ソフト 

## I2Ctest ○

### サーボとIMUの動作を確認 ライブラリの使い方 I2Cを学ぶ
完成 確認

## proto_left

左側のみの動作

* IMUのデータに移動平均フィルタをかける
* 加速度センサのデータ(3次元ベクトル)から, ミミができるだけ垂直(+初期のオフセット)を維持するサーボの角度を計算する
    * 範囲内で動きをできるだけ大げさにする(オーバーシュートあり)
* サーボの指定角度にノイズを乗せる

## exhibit

### proto_leftを基本に両方が動くようにする
