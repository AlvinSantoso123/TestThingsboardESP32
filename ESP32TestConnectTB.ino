#include <WiFi.h>
#include <Arduino_HTTP_Client.h>
#include <ThingsBoardHttp.h>

constexpr char WIFI_SSID[] = "ssid";
constexpr char WIFI_PASSWORD[] = "password";
constexpr char TOKEN[] = "aaaaa";
constexpr char THINGSBOARD_SERVER[] = "ip.ip.ip.ip";
constexpr uint16_t THINGSBOARD_PORT = 8080U;
constexpr uint16_t MAX_MESSAGE_SIZE = 128U;
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;

constexpr char TEMPERATURE_KEY[] = "temperature";
constexpr char HUMIDITY_KEY[] = "humidity";

WiFiClient espClient;
Arduino_HTTP_Client httpClient(espClient, THINGSBOARD_SERVER, THINGSBOARD_PORT);
ThingsBoardHttp tb(httpClient, TOKEN, THINGSBOARD_SERVER, THINGSBOARD_PORT);

void InitWiFi() {
  Serial.println("Connecting to AP ...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    // Delay 500ms until a connection has been successfully established
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

/// @brief Reconnects the WiFi uses InitWiFi if the connection has been removed
/// @return Returns true as soon as a connection has been established again
bool reconnect() {
  // Check to ensure we aren't connected yet
  const wl_status_t status = WiFi.status();
  if (status == WL_CONNECTED) {
    return true;
  }

  InitWiFi();
  return true;
}

void setup() {
  randomSeed(analogRead(0));
  // Initalize serial connection for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  delay(1000);
  InitWiFi();
}

void loop() {
  // Serial.print("Free Heap: ");
  // Serial.println(ESP.getFreeHeap());
  delay(1000);

  if (!reconnect()) {
    return;
  }

  Serial.println("Sending temperature data...");
  tb.sendTelemetryData(TEMPERATURE_KEY, random(10, 31));

  Serial.println("Sending humidity data...");
  tb.sendTelemetryData(HUMIDITY_KEY, random(40, 90));
}
