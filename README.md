# ねこねこみみみみ
## 概要
VRCとの連携を目的として、市販されている猫耳カチューシャに取り付けられるハプティクスデバイスを作成する。ケーブル部分には、3Dプリンターで作成したケーブルチェーンを用いる。senseshift-hardwareというOSSプロジェクトを参考にしながら作成する。

## 参考
[自作ハプティクスベストを作ってみた。｜KirisameNanoha](https://note.com/kirisamenanoha/n/n8a3842eb5d57)
[GitHub - senseshift/senseshift-hardware: Hardware schematics to build haptic-feedback devices](https://github.com/senseshift/senseshift-hardware)

## 予備知識

### ハプティクスモーターについて
ERM 偏心回転質量
* PWAで制御
	種類
	* コイン型
	* 円筒型
	* コア型

LRA 線形共振
* 注意：ドライバが必要
	種類
	* コイン型
	* バー型
	* Y軸(平行)
	* Z軸(垂直)

## 案
### 案１
* マイコン esp32系
	* xiao esp32s3 or c6
* ドライバ DRV2605 x2
* LRAコイン型(Z軸)を直線に複数配置
* 左右の耳の下に3つずつ(計6つ)
* 電源をどうするかは検討
	* リポバッテリー？
	* 最初はモバイルバッテリーかな
→LRAが入手しづらい

### 案２ →採用
 * マイコン esp32系
	* xiao esp32s3 or c6
* ERMコイン型を直線に複数配置
* 左右の耳の下に3つずつ(計6つ)
* MOSFET x6
* 電源をどうするかは検討
	* リポバッテリー？
	* 最初はモバイルバッテリー

## 回路図
1チャンネル分
```
                 VMOTOR (+3.0V～4.2V)
                        |
                        |
                    +---+---+
                    |  ERM  |
                    |Motor  |
                    +---+---+
                        |
                        +-------------------+
                        |                   |
                        |                 |<| D1
                        |            SS14 / 1N5819
                        |                 | |
                        |                   |
                        |                   +---- VMOTOR
                        |
                      Drain
                    Q1 N-MOSFET ---- Gate
                      Source
                        |
                       GND

XIAO GPIO PWM ----[100Ω]---- Gate
                         |
                       [100kΩ]
                         |
                        GND
```

MOSFETをどれだけON・OFFするかによって強さを決める



## 配置図
```
        [前（おでこ）]

   左側               右側
   ●──●──●       ●──●──●
   前  中  後       前  中  後

        [後（後頭部）]
```



## 部品
秋月電子の場合

| 品名            | 型番                  |  個数 |    値段 | 購入リンク                                                  |
| ------------- | ------------------- | --: | ----: | ------------------------------------------------------ |
| DMOSトランジスタアレイ | TBD62003APG         |   1 |   100 | [109943](https://akizukidenshi.com/catalog/g/g109943/) |
| ミニ振動モーター      | (記載なし / 秋月ミニ振動モーター) |   6 |   200 | [16777](https://akizukidenshi.com/catalog/g/g116777/)  |
| マイコンボード       | XIAO ESP32S3        |   1 | 1,480 | [118078](https://akizukidenshi.com/catalog/g/g118078/) |
| ショットキーダイオード   | 1S4                 |   1 |   140 | [116384](https://akizukidenshi.com/catalog/g/g116384/) |
| 電解コンデンサ       | 1000µF（耐圧10V〜）      |   1 |    20 | [108424](https://akizukidenshi.com/catalog/g/g108424/) |
| LDO 3.3V1A    | BA033CC0T           |   1 |   100 | [113675](https://akizukidenshi.com/catalog/g/g113675/) |

合計3220円


振動モーターはアリエクで買ってもいいかも？
