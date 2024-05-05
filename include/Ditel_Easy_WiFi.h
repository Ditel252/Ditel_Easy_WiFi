#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>
#include "Ditel_Easy_WiFi_Setting.h"

#ifdef DITEL_ESP32
#include <esp_wifi.h>
#endif

#define NUMBER_OF_SCAN_NETWORK  64  //Number of networks to detect
#define SERIAL_SPEED    115200

#define ERROR_SET_WIFI_MODE  -1
#define ERROR_WIFI_DISCONNECTED -2
#define ERROR_BEGIN_WIFI -3
#define ERROR_WIFI_CONNECT -4
#define ERROR_UDP_BEGIN -11

#define DWIFI_OK 1U

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

    Network *(allocateMemoryForScan(uint16_t _numberOfMemoryForAllocate = NUMBER_OF_SCAN_NETWORK));
    void scanNetworkToSerial(bool _wasSerialBegun = false, uint32_t _serialSpeed = SERIAL_SPEED);
    int32_t scanNetwork(Ditel_Easy_WiFi::Network *_variableForStoringDetectedNetwork, uint16_t _numberOfScanNetWork = NUMBER_OF_SCAN_NETWORK);
    int connectToNetwork(Ditel_Easy_WiFi::Network _networkToConnect, uint16_t _timeoutOfTryConnect);
    void getMyIpAddress(char *_myIpAddress);
};