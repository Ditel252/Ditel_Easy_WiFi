#include <Arduino.h>
#include "Ditel_Easy_WiFi.h"

Ditel_Easy_WiFi::Network *(Ditel_Easy_WiFi::allocateMemoryForScan()){
    Ditel_Easy_WiFi::Network *_detectedVariable;
    _detectedVariable = (Ditel_Easy_WiFi::Network *)malloc(sizeof(Ditel_Easy_WiFi::Network) * NUMBER_OF_SCAN_NETWORK);

    for(int i = 0; i < NUMBER_OF_SCAN_NETWORK; i++){
        strcpy((_detectedVariable + i)->SSID, "UNKNOWN");
        (_detectedVariable + i)->RSSI = 0;
        (_detectedVariable + i)->EncryptionType = WIFI_AUTH_OPEN;
        strcpy((_detectedVariable + i)->Password, "UNKNOWN");
    }

    return _detectedVariable;
}

int32_t Ditel_Easy_WiFi::scanNetwork(Ditel_Easy_WiFi::Network *_variableForStoringDetectedNetwork){

    if(!WiFi.mode(WIFI_STA))
        return -1;
    
    if(!WiFi.disconnect())
        return -1;

    delay(100);

    int16_t numberOfDetectedNetWorks = WiFi.scanNetworks();

    if(numberOfDetectedNetWorks == 0)
        return 0;    

    for(int i = 0; i < (numberOfDetectedNetWorks > NUMBER_OF_SCAN_NETWORK ? NUMBER_OF_SCAN_NETWORK : numberOfDetectedNetWorks); i++){
        strcpy((_variableForStoringDetectedNetwork + i)->SSID, (WiFi.SSID(i)).c_str());
        (_variableForStoringDetectedNetwork + i)->RSSI = WiFi.RSSI(i);
        (_variableForStoringDetectedNetwork + i)->EncryptionType = WiFi.encryptionType(i);
        
        delay(10);
    }


    return numberOfDetectedNetWorks;
}

void Ditel_Easy_WiFi::scanNetworkToSerial(){
    Serial.begin(SERIAL_SPEED);
    uint8_t enterKeyScanCount = 0;

    while(Serial.read() != 13){
        if(enterKeyScanCount >= 100){
            enterKeyScanCount = 0;
            Serial.println("Please Click Enter Key.");
        }else{
            enterKeyScanCount++;
            delay(10);
        }
    }

    Serial.println("Begin Network Scan Program");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    Serial.println("Setup Done");
    Serial.println("Begin Network Scan");

    int16_t numberOfDetectedNetWorks = WiFi.scanNetworks();
    Serial.println("Network Scan Done");

    Serial.printf("%d Networks Detected", numberOfDetectedNetWorks);

    for(int i = 0; i < numberOfDetectedNetWorks; i++){
        Serial.printf("%d: %s (%d) %s\n", i + 1, (WiFi.SSID(i)).c_str(), WiFi.RSSI(i), (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
        delay(10);
    }
}

int Ditel_Easy_WiFi::connectToNetwork(Ditel_Easy_WiFi::Network _networkToConnect, uint16_t _timeoutOfTryToConnect){
    networkToConnect = _networkToConnect;

    WiFi.disconnect(true);
    WiFi.mode(WIFI_MODE_STA);
    WiFi.begin(networkToConnect.SSID, networkToConnect.Password);

#ifdef DITEL_ESP32
    esp_wifi_set_ps(WIFI_PS_NONE);
#endif

#ifdef DITEL_WIO_TERMINAL
    rpc_wifi_set_power_mode(WIFI_PS_NONE, WIFI_PS_NONE);
#endif

    bool connectOK = false;
    unsigned long int timeOfStartScanNetWork = millis();

    do{
        if(WiFi.status() == WL_CONNECTED){
            connectOK = true;
            break;
        }

        delay(100); 
    }while((millis() - timeOfStartScanNetWork) < _timeoutOfTryToConnect);

    if(connectOK != true)
        return ERROR_WIFI_CONNECT;
    
    return DWIFI_OK;
}

void Ditel_Easy_WiFi::getMyIpAddress(char _myIpAddress[]){
    strcpy(_myIpAddress, WiFi.localIP().toString().c_str());
}

int _Ditel_Easy_WiFi_UDP::begin(uint16_t _myUdpPort){
    _Ditel_Easy_WiFi_UDP::serverIpAddress = (char *)malloc(sizeof(char) * 32);

    _Ditel_Easy_WiFi_UDP::myUdpPort = _myUdpPort;

    if(wifiUdp.begin(_Ditel_Easy_WiFi_UDP::myUdpPort) == 1)
        return DWIFI_OK;
    else
        return ERROR_UDP_BEGIN;
}

void _Ditel_Easy_WiFi_UDP::setServerInfo(String _serverIpAddress, uint16_t _serverUdpPort){
    strcpy(_Ditel_Easy_WiFi_UDP::serverIpAddress, _serverIpAddress.c_str());
    _Ditel_Easy_WiFi_UDP::serverUdpPort = _serverUdpPort;
}

int _Ditel_Easy_WiFi_UDP::send(uint8_t *sendData, size_t sendDataSize){
    wifiUdp.beginPacket(_Ditel_Easy_WiFi_UDP::serverIpAddress, _Ditel_Easy_WiFi_UDP::serverUdpPort);
    wifiUdp.write(sendData, sendDataSize);
    wifiUdp.endPacket();

    return 0;
}

int _Ditel_Easy_WiFi_UDP::avaiable(){
    return wifiUdp.parsePacket();
}

int _Ditel_Easy_WiFi_UDP::read(uint8_t *readData, size_t readDataSize){
    return wifiUdp.read(readData, readDataSize);
}