#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "vars.h"

// ----------------------------------------------------------------------------
// ---- STATE & CONFIGURATION -------------------------------------------------
// ----------------------------------------------------------------------------

DHT dht(D1, DHT11);
WiFiClient esp_client;
PubSubClient client(esp_client);

char client_str_buffer[32];
char topic_str_buffer[32];
char reading_str_buffer[8];

#define DATA_PUBLISH_DELAY (10000)
#define MQTT_RECONNECT_DELAY (5000)

// ----------------------------------------------------------------------------
// ---- MAIN HOOKS ------------------------------------------------------------
// ----------------------------------------------------------------------------

void setup() {
  configure_serial();
  configure_wifi();
  configure_dht();
  configure_mqtt();
}

void loop() {
  keep_mqtt_alive();
  publish_sensor_reading(&read_temperature, "temperature");
  publish_sensor_reading(&read_humidity, "humidity");
  sleep_for_a_while();
}

// ----------------------------------------------------------------------------
// ---- SETUP HELPERS ---------------------------------------------------------
// ----------------------------------------------------------------------------

void configure_serial() {
  Serial.begin(9600);
}

void configure_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }

  Serial.print("connected with IP address: ");
  Serial.print(WiFi.localIP());
  Serial.print("...");

  Serial.println("done.");
}

void configure_dht() {
  Serial.print("Initialising DHT11...");
  dht.begin();
  Serial.println("done.");
}

void configure_mqtt() {
  Serial.print("Initialising MQTT...");
  client.setServer(MQTT_SERVER_HOST, MQTT_SERVER_PORT);
  Serial.println("done.");
}

// ----------------------------------------------------------------------------
// ---- LOOP HELPERS ----------------------------------------------------------
// ----------------------------------------------------------------------------

void keep_mqtt_alive() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT server as: ");

    snprintf(client_str_buffer, sizeof(client_str_buffer), "ESP8266/%s", String(random(0xffff), HEX).c_str());
    Serial.print(client_str_buffer);

    if (client.connect(client_str_buffer)) {
      Serial.println("...done.");
    } else {
      Serial.print("...failed with error: ");
      Serial.print(client.state());
      Serial.print("...retrying in ");
      Serial.print(MQTT_RECONNECT_DELAY);
      Serial.println("ms");
      delay(MQTT_RECONNECT_DELAY);
    }
  }

  client.loop();
}

float read_humidity() {
  // @param bool force => true if in force mode
  return dht.readHumidity(false);
}
float read_temperature() {
  // @param bool Scale => true = Fahrenheit, false = Celcius
  // @param bool force => true if in force mode
  return dht.readTemperature(false, false);
}

void publish_sensor_reading(float (*read_value)(void), char* sensor_type) {
  float value = read_value();
  if (isnan(value)) {
    Serial.print("Could not read ");
    Serial.print(sensor_type);
    Serial.print(" sensor!");
    Serial.println();
  } else {
    snprintf(topic_str_buffer, sizeof(topic_str_buffer), "esp8266/%s", sensor_type);
    snprintf(reading_str_buffer, sizeof(reading_str_buffer), "%.2f", value);
    Serial.print(topic_str_buffer);
    Serial.print(": ");
    Serial.println(reading_str_buffer);
    client.publish(topic_str_buffer, reading_str_buffer);
  }
}

void sleep_for_a_while() {
  Serial.println("Going back to sleep, see you when I see you.");
  delay(DATA_PUBLISH_DELAY);
}
