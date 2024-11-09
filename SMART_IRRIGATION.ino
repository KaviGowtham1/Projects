#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin Definitions
#define SOIL_MOISTURE_PIN A0
#define RELAY_PIN 7
#define DHT_PIN 2
#define DHT_TYPE DHT11  // Change to DHT22 if using a DHT22 sensor

// Initialize DHT sensor
DHT dht(DHT_PIN, DHT_TYPE);

// Initialize LCD Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Threshold values
const int moistureThreshold = 400; // Adjust this value based on your soil sensor
const int irrigationDelay = 5000;  // Watering duration in milliseconds

void setup() {
  // Serial Monitor for debugging
  Serial.begin(9600);
  
  // Initialize DHT sensor
  dht.begin();
  
  // Initialize LCD Display
  lcd.init();
  lcd.backlight();
  
  // Initialize Relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Make sure the pump is off

  // Initial display on LCD
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
}

void loop() {
  // Read soil moisture level
  int soilMoistureValue = analogRead(SOIL_MOISTURE_PIN);
  
  // Read temperature and humidity from DHT sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Display readings on LCD
  lcd.setCursor(0, 1);
  lcd.print("Moist:");
  lcd.print(soilMoistureValue);
  lcd.print(" Temp:");
  lcd.print(temperature);
  
  // Print readings to Serial Monitor
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  // Check soil moisture level
  if (soilMoistureValue > moistureThreshold) {
    Serial.println("Soil is dry. Pump ON.");
    digitalWrite(RELAY_PIN, LOW); // Turn on the pump
    lcd.setCursor(0, 1);
    lcd.print("Watering...       ");
    delay(irrigationDelay);
  } else {
    Serial.println("Soil is wet. Pump OFF.");
    digitalWrite(RELAY_PIN, HIGH); // Turn off the pump
    lcd.setCursor(0, 1);
    lcd.print("Soil OK          ");
  }
  
  // Wait before next reading
  delay(2000);
}
