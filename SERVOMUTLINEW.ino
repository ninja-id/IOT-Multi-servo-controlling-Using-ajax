#include <Wire.h>
#include <ESP8266WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>


Servo myservo;
Servo myservo1;
LiquidCrystal_I2C lcd(0x27, 16, 2);// create servo object to control a servo
// twelve servo objects can be created on most boards

// GPIO the servo is attached to
static const int servoPin = D8;
static const int servoPin1 = D7;
static const int ledPin = D4;
// Replace with your network credentials
const char* ssid     = "tes";
const char* password = "qwerty234";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString = String(5);
String valueString1 = String(5);
int pos1 = 0;
int pos2 = 0;

int pos3, pos4 = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Wire.begin(D1,D2);
  lcd.begin();
  lcd.setCursor(0,0); 
  lcd.print("inisialisasi");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("connecting");
  lcd.setCursor(0,1);
  lcd.print(ssid);
  lcd.clear();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  myservo.attach(servoPin);  // attaches the servo on the servoPin to the servo object
  myservo1.attach(servoPin1);
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  lcd.print("Connected");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
            client.println(".slider { width: 300px; }</style>");
            client.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
                     
            // Web Page
            client.println("</head><body><h1>PENGENDALI SERVO</h1>");
            //slider1
            client.println("<p>TILTING: <span id=\"servoPos\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\" value=\""+valueString+"\"/>");
            //slider2
            client.println("<p>AZIMUTH: <span id=\"servoPos1\"></span></p>");          
            client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider1\" onchange=\"servo1(this.value)\" value1=\""+valueString1+"\"/>");
            
            
            client.println("<script>var slider = document.getElementById(\"servoSlider\");");
            client.println("var servoP = document.getElementById(\"servoPos\"); servoP.innerHTML = slider.value;");
            client.println("slider.oninput = function() { slider.value = this.value; servoP.innerHTML = this.value; }");
           
            client.println("var slider1 = document.getElementById(\"servoSlider1\");");
            client.println("var servoP1 = document.getElementById(\"servoPos1\"); servoP1.innerHTML = slider1.value;");
            client.println("slider1.oninput = function() { slider1.value = this.value; servoP1.innerHTML = this.value; }");
            
            client.println("$.ajaxSetup({timeout:1000}); function servo(pos) { ");
            client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}");

            client.println("$.ajaxSetup({timeout:1000}); function servo1(pos) { ");
            client.println("$.get(\"/?value1=\" + pos + \"&\"); {Connection: close};}</script>");

           
            client.println("</body></html>");     
            
            //GET /?value=180& HTTP/1.1
            if(header.indexOf("GET /?value=")>=0) {
              pos1 = header.indexOf('=');
              pos2 = header.indexOf('&');
              valueString = header.substring(pos1+1, pos2);
              
              //Rotate the servo
  
              myservo.write(valueString.toInt());
              Serial.println(valueString);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(WiFi.localIP());
              lcd.setCursor(0,1);
              lcd.print("TILTING =");
              lcd.print(valueString);
              lcd.print(" DER");
              digitalWrite(ledPin,HIGH);
              delay(100);
              digitalWrite(ledPin,LOW); 
            }         

              if(header.indexOf("GET /?value1=")>=0) {
              pos3 = header.indexOf('=');
              pos4 = header.indexOf('&');
              valueString1 = header.substring(pos3+1, pos4);
              
              //Rotate the servo
  
              myservo1.write(valueString1.toInt());
              Serial.println(valueString1); 
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print(WiFi.localIP());
              lcd.setCursor(0,1);
              lcd.print("AZIMUTH =");
              lcd.print(valueString1);
              lcd.print(" DER");
              digitalWrite(ledPin,HIGH); 
              delay(100);
              digitalWrite(ledPin,LOW);
            }
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}
