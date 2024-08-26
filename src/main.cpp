//ตัวส่ง ESP32
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// แก้ไขค่า mac ตัวที่ต้องการส่งไปหา
// uint8_t broadcastAddress[] = {0xEC, 0x94, 0xCB, 0x6F, 0x11, 0x60};//ส่งไปหาเฉพาะ mac address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};//ส่งไปหาทุกตัว

typedef struct struct_message { // สร้างตัวแปรแพ็จเกจแบบ struct
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

struct_message myData; // ตัวแปรแพ็คเกจที่ต้องการส่ง

//เมื่อส่งข้อมูลมาทำฟังก์ชั่นนี้
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  //ตั้งเป็นโหมด Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // สั่งให้เริ่ม ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  //เมื่อส่งให้ทำฟังก์ชั่น OnDataSend ที่เราสร้างขึ้น
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // เชื่อมต่ออุปกรณ์ที่ต้องการสื่อสาร
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  // สร้างตัวแปรแพ็คเกจที่ต้องการส่ง
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1, 20);
  myData.c = 1.2;
  myData.d = false;

  // สั่งให้ส่งข้อมูล
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}