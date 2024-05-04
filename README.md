# Ditel Easy WiFi
## 概要
## 環境
|項目|内容|
|---|---|
|エディタ|`Visual Studio Code` <br> version:`1.88.1`<br> 拡張機能:`PlatformIO`|
|使用ボード|`Wio Terminal`<br>`ESP32-DevKitC ESP-WROOM-32`|
## 関数説明
- `allocateMemoryForScan()`   
  スキャンしたネットワークの情報を, 格納する配列用の動的メモリを確保します.    
  
  - 注意事項
    
  - 引数   
    |引数|型|内容|初期値|
    |---|---|---|---|
    |第1引数|`uint16_t`|確保する配列の個数|`NUMBER_OF_SCAN_NETWORK`|
  
  - 返り値   
    型 : `Ditel_Easy_WiFi::Network *`   
    確保した動的メモリのポインタ


- `scanNetwork()`   
  スキャンしたネットワークの情報を, 渡された配列に格納します.    

  - 注意事項
    - 取得できるネットワークの情報は`SSID`, `RSSI`, 暗号化のタイプ
    - `SSID`に英数字以外の文字が含まれている場合は, 正しく取得できない可能性がある

  - 引数   
    |引数|型|内容|初期値|
    |---|---|---|---|
    |第1引数|`Ditel_Easy_WiFi::Network *`|ネットワーク情報を格納する配列|-|
    |第2引数|`uint16_t`|検出するネットワーク数の最大値|`NUMBER_OF_SCAN_NETWORK`|
  
  - 返り値   
    型 : `int32_t`   
    |値|内容|
    |---|---|
    |`ERROR_SET_WIFI_MODE`|WiFiモードをSTAモードにできない|
    |`ERROR_WIFI_DISCONNECTED`|ネットワーク接続を切断できない|
    |0以上|検出したネットワーク数|
