#include <FS.h>                   //this needs to be first, or it all crashes and burns...

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include <MQTT.h>

#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// #define HASBUTTON
#ifdef HASBUTTON
#include <PMButton.h>
PMButton button(D3);
#endif

Adafruit_BME280 bme; // I2C

WiFiClient net;
MQTTClient client;

unsigned long lastMillis = 0;

//define your default values here, if there are different values in config.json, they are overwritten.
char mqtt_server[40];
char mqtt_port[6] = "8080";
char mqtt_topic[34] = "home/wohnzimmer/temperatur";
#ifdef HASBUTTON
char mqtt_button[50] = "home/wohnzimmer/temperatur/button";
#endif
char mqtt_client[16] = "bme280-001";
char mqtt_user[16] = "user";
char mqtt_pass[16] = "pass";

//flag for saving data
bool shouldSaveConfig = false;


void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect(mqtt_client, mqtt_user, mqtt_pass)) {
    Serial.print(".");
    delay(5000);
  }

  Serial.println("\nconnected!");

  // client.subscribe("/hello");
  // client.unsubscribe("/hello");
}

void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
}

//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println();

  //clean FS, for testing
  //SPIFFS.format();

  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        Serial.println("opened config file");
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject(buf.get());
        json.printTo(Serial);
        if (json.success()) {
          Serial.println("\nparsed json");

          strcpy(mqtt_server, json["mqtt_server"]);
          strcpy(mqtt_port, json["mqtt_port"]);
          strcpy(mqtt_topic, json["mqtt_topic"]);
#ifdef HASBUTTON
          strcpy(mqtt_button, json["mqtt_topic"]);
          sprintf(mqtt_button, "%s%s", mqtt_topic, "/button");
#endif
          strcpy(mqtt_client, json["mqtt_client"]);
          strcpy(mqtt_user, json["mqtt_user"]);
          strcpy(mqtt_pass, json["mqtt_pass"]);

        } else {
          Serial.println("failed to load json config");
        }
      }
    }
  } else {
    Serial.println("failed to mount FS");
  }
  //end read

  // The extra parameters to be configured (can be either global or just in the setup)
  // After connecting, parameter.getValue() will get you the configured value
  // id/name placeholder/prompt default length
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server, 40);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port, 5);
  WiFiManagerParameter custom_mqtt_topic("topic", "mqtt topic", mqtt_topic, 32);
  WiFiManagerParameter custom_mqtt_client("client", "mqtt client", mqtt_client, 32);
  WiFiManagerParameter custom_mqtt_user("user", "mqtt user", mqtt_user, 32);
  WiFiManagerParameter custom_mqtt_pass("pass", "mqtt pass", mqtt_pass, 32);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //set static ip
  // wifiManager.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  //add all your parameters here
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_topic);
  wifiManager.addParameter(&custom_mqtt_client);
  wifiManager.addParameter(&custom_mqtt_user);
  wifiManager.addParameter(&custom_mqtt_pass);

  //reset settings - for testing
  //wifiManager.resetSettings();

  //set minimu quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
  
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  //wifiManager.setTimeout(120);

  //fetches ssid and pass and tries to connect
  //if it does not connect it starts an access point with the specified name
  //here  "AutoConnectAP"
  //and goes into a blocking loop awaiting configuration
  if (!wifiManager.autoConnect("TemperaturSensor", "password")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(5000);
  }

  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");

  //read updated parameters
  strcpy(mqtt_server, custom_mqtt_server.getValue());
  strcpy(mqtt_port, custom_mqtt_port.getValue());
  strcpy(mqtt_topic, custom_mqtt_topic.getValue());
#ifdef HASBUTTON
  strcpy(mqtt_button, custom_mqtt_topic.getValue());
  sprintf(mqtt_button, "%s%s", mqtt_topic, "/button");
#endif
  strcpy(mqtt_client, custom_mqtt_client.getValue());
  strcpy(mqtt_user, custom_mqtt_user.getValue());
  strcpy(mqtt_pass, custom_mqtt_pass.getValue());

  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["mqtt_server"] = mqtt_server;
    json["mqtt_port"] = mqtt_port;
    json["mqtt_topic"] = mqtt_topic;
    json["mqtt_client"] = mqtt_client;
    json["mqtt_user"] = mqtt_user;
    json["mqtt_pass"] = mqtt_pass;

    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    json.printTo(Serial);
    json.printTo(configFile);
    configFile.close();
    //end save
  }

  Serial.println("local ip");
  Serial.println(WiFi.localIP());

  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
  }

  // Note: Local domain names (e.g. "Computer.local" on OSX) are not supported by Arduino.
  // You need to set the IP address directly.
  client.begin(mqtt_server, net);
  client.setOptions(10, true, 5000);
  client.onMessage(messageReceived);

  connect();

#ifdef HASBUTTON
  button.begin();
  button.holdTime(5000);
  button.longHoldTime(8000);
#endif

}

void loop() {
  // put your main code here, to run repeatedly:
  client.loop();
  delay(10);  // <- fixes some issues with WiFi stability

  if (!client.connected()) {
    connect();
  }

#ifdef HASBUTTON
  button.checkSwitch();
  if(button.clicked()) {
    client.publish(mqtt_button, "clicked");
  }
  if(button.held()) {
    client.publish(mqtt_button, "held");
  }
  if(button.heldLong()) {
    client.publish(mqtt_button, "heldLong");
  }
#endif

  // publish a message roughly every minute.
  if (millis() - lastMillis > 60000) {
    lastMillis = millis();
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    json["temp"] = bme.readTemperature();
    json["press"] = bme.readPressure() / 100.0F;
    json["humid"] = bme.readHumidity();
    String bmeJson;
    json.printTo(bmeJson);
    client.publish(mqtt_topic, bmeJson);
  }

}
