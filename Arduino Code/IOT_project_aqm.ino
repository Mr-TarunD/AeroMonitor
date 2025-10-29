#define SECRET_SSID "OPPO A78 5G"		
#define SECRET_PASS "12341234"
#define SECRET_CH_ID 2504082
#define SECRET_WRITE_APIKEY "VY9BXO2OX6HCRYA7"

#include "WiFiEsp.h"
#include "ThingSpeak.h"
#include "DHT.h"
#include <LiquidCrystal_I2C.h>

// Network credentials
char ssid[] = SECRET_SSID;  
char pass[] = SECRET_PASS;   
int keyIndex = 0;           

WiFiEspClient client;

#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(19, 18); // RX, TX
#define ESP_BAUDRATE 19200
#else
#define ESP_BAUDRATE 115200
#endif

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

// DHT sensor setup
#define DHTPIN 2     // DHT sensor pin
#define DHTTYPE DHT11   // DHT 11 
DHT dht(DHTPIN, DHTTYPE);

// Sensor pins
#define MQ135_PIN A2
#define MQ2_PIN A3
#define MQ6_PIN A4
#define MQ7_PIN A5
#define MQ3_PIN A6
#define FLAME_PIN 52
#define BUZZER_PIN 53

// LCD setup
#define LCD_ADDRESS 0x3F
#define LCD_COLUMNS 20
#define LCD_ROWS 4
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Serial.begin(115200); // Initialize serial
  while (!Serial) {
    ; // Wait for serial port to connect
  }  
  
  // Initialize serial for ESP module  
  
  setEspBaudRate(ESP_BAUDRATE);

  Serial.print("Searching for ESP8266..."); 
  // Initialize ESP module
  WiFi.init(&Serial1);

  // Check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // Don't continue
    while (true);
  }
  Serial.println("found it!");
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Connecting to WiFi");
  
  ThingSpeak.begin(client);  // Initialize ThingSpeak

  // Initialize sensor pins
  pinMode(FLAME_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(MQ6_PIN, INPUT);
  pinMode(MQ7_PIN, INPUT);
  pinMode(MQ3_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT); 
}

void loop() {
  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
    lcd.clear();
    lcd.print("WiFi Connected");
    delay(2000);
    lcd.clear();
  }
  
  // Read DHT sensor values
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Read other sensors
  int airQuality = analogRead(MQ135_PIN);
  int smokeValue = analogRead(MQ2_PIN);
  int flameValue = digitalRead(FLAME_PIN);
  int lpgValue = analogRead(MQ6_PIN);
  int alcoholValue = analogRead(MQ7_PIN);
  int coValue = analogRead(MQ3_PIN);
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Display sensor readings on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: " + String(temperature, 1) + "C ");
  lcd.setCursor(0, 1);
  lcd.print("Hum: " + String(humidity, 1) + "%");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("AirQ: " + String(airQuality));
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smoke: " + String(smokeValue));
  lcd.setCursor(0, 1);
  lcd.print(smokeValue > 850 ? "Smoke detected" : "No smoke");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Flame: " + String(flameValue == 0 ? "Yes" : "No"));
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("LPG: " + String(lpgValue));
  lcd.setCursor(0, 1);
  lcd.print(lpgValue > 400 ? "LPG detected" : "No LPG");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Alcohol: " + String(alcoholValue));
  lcd.setCursor(0, 1);
  lcd.print(alcoholValue > 700 ? "Alcohol detected" : "No Alcohol");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CO: " + String(coValue));
  delay(2000);

  if (smokeValue > 850 || flameValue == 0 || lpgValue > 400 || alcoholValue > 700 || coValue > 800) {
    digitalWrite(BUZZER_PIN, HIGH); // Activate buzzer
  } else {
    digitalWrite(BUZZER_PIN, LOW);  // Deactivate buzzer
  }

  lcd.clear();
  lcd.print("Uploading");
  
  // Write to ThingSpeak
  ThingSpeak.setField(1, airQuality);    // Air quality field
  ThingSpeak.setField(2, temperature);   // Temperature field
  ThingSpeak.setField(3, humidity);      // Humidity field
  ThingSpeak.setField(4, flameValue);    // Flame value field
  ThingSpeak.setField(5, coValue);       // CO value field
  ThingSpeak.setField(6, smokeValue);    // Smoke value field
  ThingSpeak.setField(7, alcoholValue);  // Alcohol value field
  ThingSpeak.setField(8, lpgValue);      // LPG value field

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
    lcd.clear();
    lcd.print("Upload successful");
  }
   else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
    lcd.clear();
    lcd.print("Upload failed");
  }

  digitalWrite(BUZZER_PIN, LOW);

  
  delay(10000); // Wait 10 seconds to update the channel again
} 

void setEspBaudRate(unsigned long baudrate) {
  long rates[6] = {115200, 74880, 57600, 38400, 19200, 9600};

  Serial.print("Setting ESP8266 baudrate to ");
  Serial.print(baudrate);
  Serial.println("...");

  for (int i = 0; i < 6; i++) {
    Serial1.begin(rates[i]);
    delay(100);
    Serial1.print("AT+UART_DEF=");
    Serial1.print(baudrate);
    Serial1.print(",8,1,0,0\r\n");
    delay(100);  
  }  
  Serial1.begin(baudrate);
}


