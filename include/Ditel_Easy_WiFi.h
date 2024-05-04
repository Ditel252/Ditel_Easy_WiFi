#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include "Ditel_Easy_WiFi_Setting.h"

#ifdef DITEL_ESP32
#include <esp_wifi.h>
#endif

#define ERROR_WIFI_CONNECT 0x10
#define ERROR_UDP_BEGIN 0x20

#define DWIFI_OK 0x00

class _Ditel_Easy_WiFi_UDP{
public:
    char *serverIpAddress;
    uint16_t serverUdpPort;
    uint16_t myUdpPort;
    WiFiUDP wifiUdp;

    int begin(uint16_t _myUdpPort);

    void setServerInfo(String _serverIpAddress, uint16_t _serverUdpPort);

    int send(uint8_t *sendData, size_t sendDataSize);
    int avaiable();
    int read(uint8_t *readData, size_t readDataSize);
};

class Ditel_Easy_WiFi{
public:
    _Ditel_Easy_WiFi_UDP UDP;
    
    typedef struct{
        char SSID[32];
        int32_t RSSI;
        char Password[64];
        wifi_auth_mode_t EncryptionType;
    } Network;

    Network networkToConnect;

    Network *(allocateMemoryForScan());
    void scanNetworkToSerial();
    int32_t scanNetwork(Ditel_Easy_WiFi::Network *_variableForStoringDetectedNetwork);
    int connectToNetwork(Ditel_Easy_WiFi::Network _networkToConnect, uint16_t _timeoutOfTryConnect);
    void getMyIpAddress(char _myIpAddress[]);
};