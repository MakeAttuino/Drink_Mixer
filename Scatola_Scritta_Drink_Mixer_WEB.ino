// Load Wi-Fi library
#include <ESP8266WiFi.h>
// Credenziali
const char* ssid     = "SSID";
const char* password = "PSW";
// Set web server port number to 80
WiFiServer server(80);
// Variable HTTP request
String header;
String output_stringa_1 = "off";
String output_stringa_2 = "off";
String output_stringa_3 = "off";
// definizone display a matrice di led 8x8
#include <MD_MAX72xx.h>
#define CLK_PIN    D5
#define DATA_PIN   D6
#define CS_PIN     D8
#define MAX_DEVICES  8
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW//se il display non è corretto, sostituire FC16_HW con PAROLA_HW o GENERIC_HW
MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
#define ARRAYSIZE 4 // numero delle stringhe da visualizzare (dimensione array)
// definizione delle stringhe che si possono visualizzare
char* results[ARRAYSIZE] = {
  "------------                     ",
  "Benvenuti al FAB-LAB 121                     ",
  "Preparatore di cocktail                     ",
  "Dimostrazione Drink Mixer                     "
};
byte num_string = 0;

#define pulsante D7

// time
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;

void setup() {
  // Initializes serial monitor
  Serial.begin(115200);
  Serial.println("Sistema Drinx Mixer");
  pinMode(pulsante,INPUT_PULLUP);
  // Connessione al Wi-Fi con SSID e PSW
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  // init display matrix 8x8
  mx.begin();
  Serial.println("Drink-Mixer pronto");
  delay(3000);
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients
  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
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

            output_stringa_3 = "off";output_stringa_2 = "off";output_stringa_1 = "off";
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /1/on") >= 0) {
              Serial.println("Stringa 1 on");
              output_stringa_1 = "on";
              num_string = 1;
            } else if (header.indexOf("GET /1/off") >= 0) {
              Serial.println("Stringa 1 off");
              
              num_string = 0;
            }else if (header.indexOf("GET /2/on") >= 0) {
              Serial.println("Stringa 2 on");
              output_stringa_2 = "on";
              num_string = 2;
            } else if (header.indexOf("GET /2/off") >= 0) {
              Serial.println("Stringa 2 off");
              
              num_string = 0;
            }else if (header.indexOf("GET /3/on") >= 0) {
              Serial.println("Stringa 3 on");
              output_stringa_3 = "on";
              num_string = 3;
            } else if (header.indexOf("GET /3/off") >= 0) {
              Serial.println("Stringa 3 off");
              
              num_string = 0;
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>DRINK_MIXER Wi-Fi</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>Stringa 1 = " + output_stringa_1 + "</p>");
            // If the output5State is off, it displays the ON button       
            if (output_stringa_1=="off") {
              client.println("<p><a href=\"/1/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/1/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 

            client.println("<p>Stringa 2 = " + output_stringa_2 + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output_stringa_2=="off") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/2/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("<p>Stringa 3 = " + output_stringa_3 + "</p>");
            // If the output4State is off, it displays the ON button       
            if (output_stringa_3=="off") {
              client.println("<p><a href=\"/3/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/3/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");
            
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
  mx.control(MD_MAX72XX::INTENSITY, 8);
  if(num_string == 0){
    scrollText(results[0]);
//    num_string = 2;
    }else if(num_string == 1){
      scrollText(results[1]);
//      num_string = 3;
      }else if(num_string == 2){
        scrollText(results[2]);
//        num_string = 1;
        }else if(num_string == 3){
        scrollText(results[3]);
//        num_string = 1;
          }
//  delay(1000);
}

// scrolling dela stringa
void scrollText(const char *p){
  uint8_t charWidth;
  uint8_t cBuf[8];
  mx.clear();
  while (*p != '\0'){
    charWidth = mx.getChar(*p++, sizeof(cBuf) / sizeof(cBuf[0]), cBuf);
    for (uint8_t i = 0; i <= charWidth; i++){
      mx.transform(MD_MAX72XX::TSL);
      if(digitalRead(pulsante) == LOW){
      Serial.println("AA");delay(100);
      if(digitalRead(pulsante) == LOW){
//      num_string+=1;
//      if(num_string == 4)num_string = 1;
    }
  }
      if (i < charWidth)
        mx.setColumn(0, cBuf[i]);
      delay(50);
    }
  }
}
