#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "RAGHAV 7035"; // Add your WiFi SSID
const char* password = "r05L92}7"; // Add your WiFi password

const char* apiKey = "7381572"; // Add your Token number that the bot has sent you on WhatsApp

const char* phone_numbers[] = {
  "+916375",  // Add multiple phone numbers as needed
  "+919879"
};

const int num_phone_numbers = sizeof(phone_numbers) / sizeof(phone_numbers[0]);

int gasSensorPin = 35; // Connect your MQ5 sensor to analog pin A0
int thresholdValue = 2000; // Adjust the threshold value for gas detection (calibrate as needed)

String url;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");
  } else {
    Serial.println("WiFi not connected!");
  }

  delay(10000); // Wait for the sensor to warm up and stabilize
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi is not connected; skipping gas sensor check.");
    return;
  }

  int gasValue = analogRead(gasSensorPin);
  Serial.print("Analog Gas Value: ");
  Serial.println(gasValue);

  if (gasValue > thresholdValue) {
    const char* message = "Alert!!! Gas Leakage Detected.";
    Serial.println("Gas Leakage Detected. Sending messages.");
    for (int i = 0; i < num_phone_numbers; i++) {
      message_to_whatsapp(phone_numbers[i], message);
      delay(15000);
    }
  }

  delay(10000); // Adjust the delay to control how often you check the sensor
}

void message_to_whatsapp(const char* phone_number, const char* message) {
  String encodedMessage = urlencode(message);
  url = "https://api.callmebot.com/whatsapp.php?phone=918107895108&text=Alert!!!+Gas+Leakage+Detected&apikey=7381572";
  postData();
}

void postData() {
  HTTPClient http;
  http.begin(url);
  int httpCode = http.POST(url);
  if (httpCode == 200) {
    Serial.println("Message sent successfully.");
  } else {
    Serial.print("Error sending message. HTTP Code: ");
    Serial.println(httpCode);
  }
  http.end();
}

String urlencode(const char* str) {
  String encodedString = "";
  for (int i = 0; str[i]; i++) {
    char c = str[i];
    if (c == ' ') {
      encodedString += '+';
    } else if (isalnum(c)) {
      encodedString += c;
    } else {
      char hex[3];
      sprintf(hex, "%02X", c);
      encodedString += '%';
      encodedString += hex;
    }
  }
  return encodedString;
}