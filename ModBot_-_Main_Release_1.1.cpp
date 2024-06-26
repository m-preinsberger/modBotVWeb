#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include "index.h"
#include <Ultrasonic.h>
#include <esp_now.h>

#define CMD_STOP 0
#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_LEFT 4
#define CMD_RIGHT 8

#define TRIG_PIN 33
#define ECHO_PIN 25

#define IN1_PIN 12 // The ESP32 pin GPIO27 connected to the IN1 pin L298N
#define IN2_PIN 14 // The ESP32 pin GPIO26 connected to the IN2 pin L298N
#define IN3_PIN 27 // The ESP32 pin GPIO25 connected to the IN3 pin L298N
#define IN4_PIN 26 // The ESP32 pin GPIO33 connected to the IN4 pin L298N

const char *ssid = "htl-schueler";      // CHANGE IT
const char *password = "20NeuesWLAN17"; // CHANGE IT
int ultra_distance;

Ultrasonic main_ultrasonic(TRIG_PIN, ECHO_PIN);
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81); // WebSocket server on port 81

// Forward declarations
void CAR_stop();
void CAR_moveForward();
void CAR_moveBackward();
void CAR_turnLeft();
void CAR_turnRight();

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[%u] Disconnected!\n", num);
    break;
  case WStype_CONNECTED:
  {
    IPAddress ip = webSocket.remoteIP(num);
    Serial.printf("[%u] Connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
  }
  break;
  case WStype_TEXT:
    // Serial.printf("[%u] Received text: %s\n", num, payload);
    String angle = String((char *)payload);
    int command = angle.toInt();
    Serial.print("command: ");
    Serial.println(command);

    switch (command)
    {
    case CMD_STOP:
      Serial.println("Stop");
      CAR_stop();
      break;
    case CMD_FORWARD:
      Serial.println("Move Forward");
      CAR_moveForward();
      break;
    case CMD_BACKWARD:
      Serial.println("Move Backward");
      CAR_moveBackward();
      break;
    case CMD_LEFT:
      Serial.println("Turn Left");
      CAR_turnLeft();
      break;
    case CMD_RIGHT:
      Serial.println("Turn Right");
      CAR_turnRight();
      break;
    default:
      Serial.println("Unknown command");
    }

    break;
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.macAddress());

  // Initialize WebSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // Serve a basic HTML page with JavaScript to create the WebSocket connection
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    Serial.println("Web Server: received a web page request");
    String html = HTML_CONTENT;  // Use the HTML content from the servo_html.h file
    request->send(200, "text/html", html); });

  server.begin();
  Serial.print("ESP32 Web Server's IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  webSocket.loop();
  // TO DO: Your code here
}

void CAR_moveForward()
{
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
  digitalWrite(IN1_PIN, HIGH);
}

void CAR_moveBackward()
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  digitalWrite(IN3_PIN, HIGH);
  digitalWrite(IN2_PIN, HIGH);
}

void CAR_turnLeft()
{
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN1_PIN, HIGH);
}

void CAR_turnRight()
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN4_PIN, HIGH);
}

void CAR_stop()
{
  digitalWrite(IN1_PIN, LOW);
  digitalWrite(IN2_PIN, LOW);
  digitalWrite(IN3_PIN, LOW);
  digitalWrite(IN4_PIN, LOW);
}
