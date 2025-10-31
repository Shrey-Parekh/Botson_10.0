#define REMOTEXY_WIFI_SSID "SumoBot"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377
#define REMOTEXY_MODE__ESP32CORE_WIFI_POINT

#include <WiFi.h>
#include <RemoteXY.h>

// Motor pins (per your wiring)
#define IN1 32
#define IN2 33
#define IN3 25
#define IN4 26
#define ENA 14
#define ENB 12

// ====== UPDATED GUI (Four Buttons + Switch) ======
#pragma pack(push, 1)
struct {
  uint8_t switch_01;  // Main Power Switch (1 = ON)
  uint8_t button_01;  // Forward
  uint8_t button_02;  // Left
  uint8_t button_03;  // Right
  uint8_t button_04;  // Backward
  uint8_t connect_flag;
} RemoteXY;
#pragma pack(pop)

// GUI configuration bytes from RemoteXY editor (your layout)
unsigned char RemoteXY_CONF[] = {
  255,5,0,0,0,66,0,19,0,0,0,0,31,1,106,200,1,1,5,0,
  2,7,8,33,17,0,2,26,31,31,79,78,0,79,70,70,0,1,42,93,
  24,24,0,2,31,0,1,18,115,24,24,0,2,31,0,1,68,116,24,24,
  0,2,31,0,1,43,142,24,24,0,2,31,0
};
// ==================================================

void setup() {
  // Setup motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Stop motors initially
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  // Initialize RemoteXY
  RemoteXY_Init();

  Serial.begin(115200);
  Serial.println("Sumo Bot Ready - Button GUI Version!");
  Serial.print("Connect to WiFi: ");
  Serial.println(REMOTEXY_WIFI_SSID);
  Serial.print("Password: ");
  Serial.println(REMOTEXY_WIFI_PASSWORD);
}

void loop() {
  RemoteXY_Handler();

  int leftSpeed = 0;
  int rightSpeed = 0;

  if (RemoteXY.switch_01 == 1) {
    // === DIRECTION LOGIC ===
    if (RemoteXY.button_01 == 1) {
      // Forward
      leftSpeed = 255;
      rightSpeed = 255;
      Serial.println("FORWARD");
    } 
    else if (RemoteXY.button_02 == 1) {
      // Left
      leftSpeed = -255;
      rightSpeed = 255;
      Serial.println("LEFT TURN");
    } 
    else if (RemoteXY.button_03 == 1) {
      // Right
      leftSpeed = 255;
      rightSpeed = -255;
      Serial.println("RIGHT TURN");
    } 
    else if (RemoteXY.button_04 == 1) {
      // Backward
      leftSpeed = -255;
      rightSpeed = -255;
      Serial.println("BACKWARD");
    } 
    else {
      // No button pressed
      leftSpeed = 0;
      rightSpeed = 0;
    }
  } else {
    // Switch off
    leftSpeed = 0;
    rightSpeed = 0;
  }

  setMotors(leftSpeed, rightSpeed);
  delay(10);
}

void setMotors(int left, int right) {
  // Left motor
  if (left > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, abs(left));  // Full speed (≈300 RPM)
  } else if (left < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, abs(left));
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
  }

  // Right motor
  if (right > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, abs(right)); // Full speed (≈300 RPM)
  } else if (right < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, abs(right));
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, 0);
  }
}
