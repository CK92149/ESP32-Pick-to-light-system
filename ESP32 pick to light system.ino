#include <TFT_eSPI.h>       // Hardware-specific library
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();  // Invoke custom library
#include "logo.h"
#include <Arduino.h>
#if defined(ESP32) || defined(PICO_RP2040)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <ESP_Google_Sheet_Client.h>

#define WIFI_SSID "WIFI_AP"
#define WIFI_PASSWORD "WIFI_PASSWORD"

// For how to create Service Account and how to use the library, go to https://github.com/mobizt/ESP-Google-Sheet-Client

#define PROJECT_ID "PROJECT_ID"

// Service Account's client email
#define CLIENT_EMAIL "CLIENT_EMAIL"

//LED and pushbuttons
const int buttonPin = 0;  // the number of the pushbutton pin
const int ledPin = 2;    // the number of the LED pin
int buttonState = digitalRead(buttonPin);

// Service Account's private key
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDAEgbC/wiPhST6\nVJw2Ay5la4a0kBNE/plW/Kz71lZuinelaNUJZzpIFQYCLphZeDKYzSzu5cG+beXB\n+TIYU4+1LSgizrqg1S9xn9kSw3sdlIAWgCqU0htmCGZVjE5ewm18Wvqg7fgygwUa\nJWQ2/vNb2HqNYqA1iTGr8vjGGVNRzpUgfjNqX628qiKNVS1LkA6n6BNY4vjwtcgm\nEIyjo/LJkwFW5fmqHgUM1O3gypK91SKHMMuECXoI3JMD7mU+2jYHXlMKlEqbwZEo\nVcoI1z5YwtEsgka4nRQkXLtjo+tgd07AvxCcOfdsthfz/G7cAhzTY5oARv6Pvu+w\nyFt7X6rlAgMBAAECggEAAOCHwG4JInspTlW+AHa7dXWaXGqNrU5p/baYsAgyMJp/\nUqYbzMl2C5/xLJwZSWOs1jbYrTWGUzva2XLrD0hcGxJ/ine+CRPZ6urA/o4PnSZ5\nm0yMgtvtBNzhKQwWbUTsOU8MRP82Ow451M2rS/l505bBbqiI587dNouo2pZbXtUG\nH20WbqQJNqymk8AM46JuCI4j+Rn9tJptpOIwD7HdsHYgZHn8w0Gld5w3ha+CFfge\nDIBnvJjCiMY+0Tpv1MlrN5sgZDejSswcpBlXv9xVm3r01+Z59z88j5aKQ6JFHzEx\nWWqILaz4MkfZyzWSU4ZXyFjV4DlfKBMrNVErECAocQKBgQD0DEL81fj0VfbuYxTx\nknxp0iqcA+W7RZzw2bgufetWtzUE9WHenaNjAJ7j81cCvpax8K0qISyJ/PBGsouI\nYTjxY2AI5FU0OpOdoeS7dwH0PiqsJEen7t+Ywk+ncD4MmZoSYi7QsmFeSM96W9Jn\nSqDJmKSfDXSgK/6EXDUZDGBqFQKBgQDJehmDf69AI8jXY6s1MFUzgTKoelVpQvhQ\nSYDFJs4aKfw+moUg8mtEEpyfkRZ0OSS+9oTSEnbnbm3lFpSbveZ1ZuzHjejC24Hd\n5VwH4IPy9EaOMm7uY3SSQdCw/DdG1GiUByirxapLETcs13IZk3QGIn7KpUYUQGKk\nr+76pNOBkQKBgH0oVKpp6dUUySCUe8cwAtIaUHPWo9iY6NW0MnrVZP4B0xUJ6vlK\nCuLZDKOGxlbH+2yiK4xxmu/hYjA/nMOCaLuS5UtqHjA84jtzLWTesYfa4fOKX51G\nKjGjZUpPi+LWW2eWJsAs2zaJTK5JrOwAH9mDiNlxAyCpr0ueJjUEXeXJAoGAXBLe\nogeyET/JQxWbYl1KMpVyjygI7GfcjuRoYrDPM4nIatu2x03B04Ql644kksyijFqg\nbFhf2nfO5QiitWHddf2pa7DglFw4Ry3QFxF1e4xCyi0mIO92kHhfZOJ+aHIdsLhs\nz8MvhcrCSk3TKPkH5t3Qz88j7ZFXAygbsJOuzOECgYAGV8skURLYH038H0YX9Uoi\ntTv0gMs9/4t/18vIXNmPAax2W0LACOC5sRipAExZcW8Q5GH2EzGk16BOPvw0voLQ\n+lMMx3p5aY0zScwnqAC/9cQzEhejo3qwtZqXsUP9SL329n4f00yJlWp5IcPsjihp\n+0WpXvR+qEhfyDNHfGasvg==\n-----END PRIVATE KEY-----\n";

/**
const char rootCACert[] PROGMEM = "-----BEGIN CERTIFICATE-----\n"
                                  "MIIFVzCCAz+gAwIBAgINAgPlk28xsBNJiGuiFzANBgkqhkiG9w0BAQwFADBHMQsw\n"
                                  "CQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZpY2VzIExMQzEU\n"
                                  "MBIGA1UEAxMLR1RTIFJvb3QgUjEwHhcNMTYwNjIyMDAwMDAwWhcNMzYwNjIyMDAw\n"
                                  "MDAwWjBHMQswCQYDVQQGEwJVUzEiMCAGA1UEChMZR29vZ2xlIFRydXN0IFNlcnZp\n"
                                  "Y2VzIExMQzEUMBIGA1UEAxMLR1RTIFJvb3QgUjEwggIiMA0GCSqGSIb3DQEBAQUA\n"
                                  "A4ICDwAwggIKAoICAQC2EQKLHuOhd5s73L+UPreVp0A8of2C+X0yBoJx9vaMf/vo\n"
                                  "27xqLpeXo4xL+Sv2sfnOhB2x+cWX3u+58qPpvBKJXqeqUqv4IyfLpLGcY9vXmX7w\n"
                                  "Cl7raKb0xlpHDU0QM+NOsROjyBhsS+z8CZDfnWQpJSMHobTSPS5g4M/SCYe7zUjw\n"
                                  "TcLCeoiKu7rPWRnWr4+wB7CeMfGCwcDfLqZtbBkOtdh+JhpFAz2weaSUKK0Pfybl\n"
                                  "qAj+lug8aJRT7oM6iCsVlgmy4HqMLnXWnOunVmSPlk9orj2XwoSPwLxAwAtcvfaH\n"
                                  "szVsrBhQf4TgTM2S0yDpM7xSma8ytSmzJSq0SPly4cpk9+aCEI3oncKKiPo4Zor8\n"
                                  "Y/kB+Xj9e1x3+naH+uzfsQ55lVe0vSbv1gHR6xYKu44LtcXFilWr06zqkUspzBmk\n"
                                  "MiVOKvFlRNACzqrOSbTqn3yDsEB750Orp2yjj32JgfpMpf/VjsPOS+C12LOORc92\n"
                                  "wO1AK/1TD7Cn1TsNsYqiA94xrcx36m97PtbfkSIS5r762DL8EGMUUXLeXdYWk70p\n"
                                  "aDPvOmbsB4om3xPXV2V4J95eSRQAogB/mqghtqmxlbCluQ0WEdrHbEg8QOB+DVrN\n"
                                  "VjzRlwW5y0vtOUucxD/SVRNuJLDWcfr0wbrM7Rv1/oFB2ACYPTrIrnqYNxgFlQID\n"
                                  "AQABo0IwQDAOBgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4E\n"
                                  "FgQU5K8rJnEaK0gnhS9SZizv8IkTcT4wDQYJKoZIhvcNAQEMBQADggIBAJ+qQibb\n"
                                  "C5u+/x6Wki4+omVKapi6Ist9wTrYggoGxval3sBOh2Z5ofmmWJyq+bXmYOfg6LEe\n"
                                  "QkEzCzc9zolwFcq1JKjPa7XSQCGYzyI0zzvFIoTgxQ6KfF2I5DUkzps+GlQebtuy\n"
                                  "h6f88/qBVRRiClmpIgUxPoLW7ttXNLwzldMXG+gnoot7TiYaelpkttGsN/H9oPM4\n"
                                  "7HLwEXWdyzRSjeZ2axfG34arJ45JK3VmgRAhpuo+9K4l/3wV3s6MJT/KYnAK9y8J\n"
                                  "ZgfIPxz88NtFMN9iiMG1D53Dn0reWVlHxYciNuaCp+0KueIHoI17eko8cdLiA6Ef\n"
                                  "MgfdG+RCzgwARWGAtQsgWSl4vflVy2PFPEz0tv/bal8xa5meLMFrUKTX5hgUvYU/\n"
                                  "Z6tGn6D/Qqc6f1zLXbBwHSs09dR2CQzreExZBfMzQsNhFRAbd03OIozUhfJFfbdT\n"
                                  "6u9AWpQKXCBfTkBdYiJ23//OYb2MI3jSNwLgjt7RETeJ9r/tSQdirpLsQBqvFAnZ\n"
                                  "0E6yove+7u7Y/9waLd64NnHi/Hm3lCXRSHNboTXns5lndcEZOitHTtNCjv0xyBZm\n"
                                  "2tIMPNuzjsmhDYAPexZ3FL//2wmUspO8IFgV6dtxQ/PeEMMA3KgqlbbC1j+Qa3bb\n"
                                  "bP6MvPJwNQzcmRk13NfIRmPVNnGuV/u3gm3c\n"
                                  "-----END CERTIFICATE-----\n";
*/

bool taskComplete = false;

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
WiFiMulti multi;
#endif

void tokenStatusCallback(TokenInfo info);

void setup()
{


    Serial.begin(115200);
    Serial.println();
    Serial.println();

    Serial.printf("ESP Google Sheet Client v%s\n\n", ESP_GOOGLE_SHEET_CLIENT_VERSION);
    
  tft.begin();
  tft.setRotation(1);	// landscape

  tft.fillScreen(TFT_BLACK);

  // Swap the colour byte order when rendering
  tft.setSwapBytes(true);

  // Draw the startup image
  tft.pushImage(0, 0, infoWidth, infoHeight, ttgo_logo);

#if defined(ESP32) || defined(ESP8266)
    WiFi.setAutoReconnect(true);
#endif

#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    multi.addAP(WIFI_SSID, WIFI_PASSWORD);
    multi.run();
#else
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
#endif

    Serial.print("Connecting to Wi-Fi");
    unsigned long ms = millis();
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
        if (millis() - ms > 10000)
            break;
#endif
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    // GSheet.setCert(rootCACert); // or GSheet.setCertFile("<.pem cert file name>", esP_google_sheet_file_storage_type_flash /* or esP_google_sheet_file_storage_type_sd */);

    // Set the callback for Google API access token generation status (for debug only)
    GSheet.setTokenCallback(tokenStatusCallback);

    // The WiFi credentials are required for Pico W
    // due to it does not have reconnect feature.
#if defined(ARDUINO_RASPBERRY_PI_PICO_W)
    GSheet.clearAP();
    GSheet.addAP(WIFI_SSID, WIFI_PASSWORD);
#endif

    // Set the seconds to refresh the auth token before expire (60 to 3540, default is 300 seconds)
    GSheet.setPrerefreshSeconds(10 * 60);

    // Begin the access token generation for Google API authentication
    GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

//LED and pusbutton
  pinMode(buttonPin, INPUT);   // Set buttonPin as an input
  pinMode(ledPin, OUTPUT);     // Set ledPin as an output
}

void loop()
{
    // Call ready() repeatedly in loop for authentication checking and processing
    bool ready = GSheet.ready();

    //button to continue
    int buttonState = digitalRead(buttonPin);

    if (ready && !taskComplete)
    {
      
        // For basic FirebaseJson usage example, see examples/FirebaseJson/Create_Edit_Parse/Create_Edit_Parse.ino

        // If you assign the spreadsheet id from your own spreadsheet,
        // you need to set share access to the Service Account's CLIENT_EMAIL
        
        //DynamicJsonDocument response(1024);
        FirebaseJson response;
        // Instead of using FirebaseJson for response, you can use String for response to the functions
        // especially in low memory device that deserializing large JSON response may be failed as in ESP8266

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Serial.println("\nGet spreadsheet values from range...");
        Serial.println("---------------------------------------------------------------");

        // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/get
        //DynamicJsonDocument response(1024);
        bool success = GSheet.values.get(&response /* returned response */, "<spreadsheetId>" /* spreadsheet Id to read */, "Sheet1!A2:G" /* range to read */);
        response.toString(Serial, true);
        Serial.println();

FirebaseJsonData result;
response.get(result,"values");

// Prepare FirebaseJsonArray to take the array from FirebaseJson
FirebaseJsonArray arr;

// Get array data
result.get<FirebaseJsonArray>(arr);

 //Call get with FirebaseJsonData to parse the array at defined index i
for (size_t i = 0; i < arr.size(); i++)
{
  // result now used as temporary object to get the parse results
  arr.get(result, i);

  // Print its value
  Serial.print("Array index: ");
  Serial.print(i);
  Serial.print(", type: ");
  Serial.print(result.type);
  Serial.print(", value: ");
  Serial.println(result.to<String>());

String inputString = result.to<String>();
String parsedStrings[7];
int j = 0;

char *copy = new char[inputString.length() + 1];
strcpy(copy, inputString.c_str());
char *p = strtok(copy, ",");

while (p != NULL) {
    String temp = p;
    temp.replace("\"", "");
    parsedStrings[j++] = temp;
    p = strtok(NULL, ",");
}

String str_1 = parsedStrings[0];
String str_2 = parsedStrings[1];
String str_3 = parsedStrings[2];
String str_4 = parsedStrings[3];
String str_5 = parsedStrings[4];
String str_6 = parsedStrings[5];
String str_7 = parsedStrings[6];

str_1.replace("[", "");
str_7.replace("]", "");

int int_2 = str_2.toInt();
int int_4 = str_4.toInt();

Serial.print("Cell: ");
Serial.println(str_1);
Serial.print("status: ");
Serial.println(int_2);
Serial.print("product: ");
Serial.println(str_3);
Serial.print("amount: ");
Serial.println(int_4);
Serial.print("order#: ");
Serial.println(str_5);
Serial.print("Order#: ");
Serial.println(str_6);
Serial.print("color: ");
Serial.println(str_7);

    if (int_2 == 1 && str_3 == "<ProductName>") { //insert "<ProductName>" as declared on the pick list on google sheets
      tft.setCursor(0, 0, 2);
      tft.fillScreen(TFT_BLACK);
      Serial.println("if triggered");
            if (str_7 == "BLUE"){
      tft.setTextColor(TFT_BLUE); tft.setTextFont(7);
      Serial.println("if blue");
      }
            if (str_7 == "RED"){
      tft.setTextColor(TFT_RED); tft.setTextFont(7);
      Serial.println("if red");
      }
            if (str_7 == "GREEN"){
      tft.setTextColor(TFT_GREEN); tft.setTextFont(7);
      Serial.println("if green");
      }
      tft.setTextSize(2);
      tft.println(int_4);

          while (buttonState == HIGH) {
          buttonState = digitalRead(buttonPin);
          // Turn off the LED                         
          //wait for pick/button press
          digitalWrite(ledPin, HIGH);
          }
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_YELLOW); tft.setTextFont(2);
        tft.println("product_1");

        Serial.println("\nClear spreadsheet values in range...");
        Serial.println("--------------------------------------------------------------");

        // For Google Sheet API ref doc, go to https://developers.google.com/sheets/api/reference/rest/v4/spreadsheets.values/clear

        success = GSheet.values.clear(&response /* returned response */, "<spreadsheetId>" /* spreadsheet Id to clear */, "Sheet1!" + str_1 /* range to clear */);
        response.toString(Serial, true);
        Serial.println();
    }
}

delay (2000); //time between spreadsheet checks. Device waiting for new orders

        if (success)
            response.toString(Serial, true);
        else
            Serial.println(GSheet.errorReason());
        Serial.println();

#if defined(ESP32) || defined(ESP8266)
        Serial.println(ESP.getFreeHeap());
#elif defined(PICO_RP2040)
        Serial.println(rp2040.getFreeHeap());
#endif
        return;
        taskComplete = true;
    }
}

void tokenStatusCallback(TokenInfo info)
{
    if (info.status == esp_signer_token_status_error)
    {
        Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
        Serial.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
    }
    else
    {
        Serial.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    }
}