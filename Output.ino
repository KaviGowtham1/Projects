#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>  // For GPS communication
#include <OneWire.h>
#include <DallasTemperature.h>

// Firebase settings
FirebaseData firebaseData;
FirebaseConfig firebaseConfig;  // Firebase configuration object
FirebaseAuth firebaseAuth;      // Firebase authentication object

// Firebase host and authentication key
#define FIREBASE_HOST "https://manhole-monitoring-default-rtdb.firebaseio.com/"  // Firebase URL
#define FIREBASE_AUTH "MSWVtKZINZmtyY1Qo52Vrc19ZNrg2SlRTCCGsbwE"  // Firebase Authentication Key

// WiFi settings
const char* ssid = "OnePlus Nord CE 5G";
const char* password = "12345678";

// GPS objects
TinyGPSPlus gps;

// Pin definitions
#define TRIG_PIN D5    // Ultrasonic sensor TRIG pin
#define ECHO_PIN D6    // Ultrasonic sensor ECHO pin
#define MQ02_PIN A0    // MQ-02 Gas sensor connected to analog pin A0
#define ONE_WIRE_BUS D4 // DS18B20 connected to digital pin D4
#define GPS_RX D2      // GPS RX connected to ESP8266 D2
#define GPS_TX D1      // GPS TX connected to ESP8266 D1

// Set up OneWire for temperature sensor
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// GPS serial communication setup
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);

// Variables
long duration;
float distance;
float gasLevel;
float temperatureC;
float latitude = 0, longitude = 0;

// Ultrasonic Sensor - Measure distance
float measureDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;  // Convert to cm
  return distance;
}

// Initialize sensors, Wi-Fi, and GPS
void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600);          // Initialize GPS serial communication
  
  WiFi.begin(ssid, password);

  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize Firebase
  firebaseConfig.host = FIREBASE_HOST;
  firebaseConfig.signer.tokens.legacy_token = FIREBASE_AUTH;  // Use legacy token for authentication
  Firebase.begin(&firebaseConfig, &firebaseAuth);  // Pass firebaseAuth as the second argument
  Firebase.reconnectWiFi(true);

  // Initialize sensors
  sensors.begin();

  // Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MQ02_PIN, INPUT);
}

// Upload data to Firebase
void uploadToFirebase(float dist, float gas, float temp, float lat, float lon) {
  String path = "/drainageSystem";
  Firebase.setFloat(firebaseData, path + "/distance", dist);
  Firebase.setFloat(firebaseData, path + "/gasLevel", gas);
  Firebase.setFloat(firebaseData, path + "/temperature", temp);
  Firebase.setFloat(firebaseData, path + "/latitude", lat);
  Firebase.setFloat(firebaseData, path + "/longitude", lon);

  if (firebaseData.dataAvailable()) {
    Serial.println("Data sent successfully to Firebase");
  } else {
    Serial.println("Failed to send data to Firebase");
    Serial.println(firebaseData.errorReason());
  }
}

void loop() {
  // Measure distance using ultrasonic sensor
  float dist = measureDistance();
  Serial.print("Distance: ");
  Serial.println(dist);

  // Read gas level from MQ-02 sensor
  gasLevel = analogRead(MQ02_PIN);
  Serial.print("Gas level: ");
  Serial.println(gasLevel);

  // Read temperature from DS18B20
  sensors.requestTemperatures();
  temperatureC = sensors.getTempCByIndex(0);
  Serial.print("Temperature: ");
  Serial.println(temperatureC);

  // Read GPS data
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
    if (gps.location.isUpdated()) {
      latitude = gps.location.lat();
      longitude = gps.location.lng();
      Serial.print("Latitude: ");
      Serial.println(latitude, 6);
      Serial.print("Longitude: ");
      Serial.println(longitude, 6);
    }
  }

  // Push data to Firebase when blockage is detected (distance < threshold)
  if (dist < 20) {
    uploadToFirebase(dist, gasLevel, temperatureC, latitude, longitude);
    Serial.println("Blockage detected! Data sent to Firebase.");
  }

  delay(5000);  // Delay between readings
}
