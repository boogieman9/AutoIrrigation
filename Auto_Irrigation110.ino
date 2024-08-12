#include "time.h"
#include <WiFi.h>
// Using code from Rui Santos
// Replace with your network credentials
const char* ssid = "";
const char* password = "";

// Set web server port number to 80
WiFiServer server(80);
IPAddress local_IP(192, 168, 197, 36);
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 255, 0);
time_t inittime=1723154010;

// Variable to store the HTTP request
String header;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

time_t curtime;
timeval curtime2;
//time(&curtime);
int usertime=0;
bool evalv1=0;
bool evalv2=0;
bool evalv3=0;
bool evalv1U=0;
bool evalv2U=0;
bool evalv3U=0;

class simple_prog{
  public:
  int interval=1;
  int duration=10;
  int starttm=0;
  bool enable=0;
  String label;
  String valvbool="111";
};


simple_prog Alpha;


void confOutput(){ 
  evalv1=0;
  evalv2=0;
  evalv3=0;
  if(Alpha.enable){
    if((curtime2.tv_sec+usertime)%(86400*Alpha.interval)>Alpha.starttm&&(curtime2.tv_sec+usertime)%(86400*Alpha.interval)<Alpha.starttm + Alpha.duration){
      if(Alpha.valvbool[0]=='1'){evalv1=1;}
      if(Alpha.valvbool[1]=='1'){evalv2=1;}
      if(Alpha.valvbool[2]=='1'){evalv3=1;}
      }
    }
  if(evalv1U){evalv1=1;}
  if(evalv2U){evalv2=1;}
  if(evalv3U){evalv3=1;}
  digitalWrite(26,evalv1);
  digitalWrite(33,evalv2);
  digitalWrite(25,evalv3);

}

String calend(int rsec){
  String day="";
  switch ((rsec/86400)%7+1){
    case 1: day="Monday"; break;
    case 2: day="Tuesday"; break;
    case 3: day="Wednesday"; break;
    case 4: day="Thursday"; break;
    case 5: day="Friday"; break;
    case 6: day="Saturday"; break;
    case 7: day="Sunday"; break;
  }
  day= day+"  "+(rsec%86400)/36000+(rsec%86400/3600)%10+":"+rsec%3600/600+(rsec%3600/60)%10+":"+rsec%60/10+rsec%60%10; 
  return day;
}

void hour_set(String input){ // expected input format hh:mm (24h)

 int secs=0;
 String asdasd="";
 asdasd=asdasd+input[0]+input[1];
  secs=asdasd.toInt()*3600;
  asdasd="";
  asdasd=asdasd+input[3]+input[4]; 
  secs=secs+asdasd.toInt()*60;
 usertime=usertime+secs-usertime%86400; }



void day_set(String input){
  int day=0;
  int cday=0;
   if(input[1]=='u'){if(input[0]=='t'||input[0]=='T'){ day=1;} else{day=6;}}
   if(input[1]=='e'){day=2;}
   if(input[1]=='h'){day=3;}
   if(input[1]=='r'){day=4;}
   if(input[1]=='a'){day=5;}
  // Serial.print("day io :");
 //  Serial.println(day);
  String Cday=calend(curtime2.tv_sec+usertime);
   if(Cday[1]=='u'){if(Cday[0]=='T'){ cday=1;} else{cday=6;}}
   if(Cday[1]=='e'){cday=2;}
   if(Cday[1]=='h'){cday=3;}
   if(Cday[1]=='r'){cday=4;}
   if(Cday[1]=='a'){cday=5;}
   if(cday>day){day=day+7;}
   usertime= usertime%604800+(day-cday)*86400;
 // 86400
}

void setup() {
  day_set("Sundey");

  hour_set("15:58");
  inittime=inittime+10800;
  pinMode(26,OUTPUT);
  pinMode(33,OUTPUT);
  pinMode(25,OUTPUT);
  confOutput();
  Serial.begin(115200);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}




void loop(){


  confOutput();
  gettimeofday(&curtime2,NULL);
  curtime=inittime+curtime2.tv_sec;


  /*************************************************************************************************
  *************************************************************************************************/
  
  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
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
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /evalv1ON") >= 0) {
              evalv1U=1;
              Serial.println("evalv1 override on");
            } 
            else if (header.indexOf("GET /evalv1OFF") >= 0) {
              Serial.println("evalv1 override off");
              evalv1U=0;
            } 
            else if (header.indexOf("GET /evalv2ON") >= 0) {
              Serial.println("evalv2 override on");
             evalv2U=1;
            } 
            else if (header.indexOf("GET /evalv2OFF") >= 0) {
              Serial.println("evalv2 override off");
             evalv2U=0;
            }
             else if (header.indexOf("GET /evalv3ON") >= 0) {
              Serial.println("evalv3 override on");
             evalv3U=1;
             }
            else if (header.indexOf("GET /evalv3OFF") >= 0) {
              Serial.println("evalv3 override off");
             evalv3U=0;
            }
              
            confOutput();


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 26px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>ESP32 Web Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println(calend(curtime2.tv_sec+usertime));
            client.println("<p>");
            // If the output26State is off, it displays the ON button       
            if (evalv1) {
              client.println("<a href=\"/evalv1OFF\"><button class=\"button\">ON</button></a>");
            } else {
              client.println("<a href=\"/evalv1ON\"><button class=\"button button2\">OFF</button></a>");
            } 
            if (evalv2) {
              client.println("<a href=\"/evalv2OFF\"><button class=\"button\">ON</button></a>");
            } else {
              client.println("<a href=\"/evalv2ON\"><button class=\"button button2\">OFF</button></a>");
            } 
            if (evalv3) {
              client.println("<a href=\"/evalv3OFF\"><button class=\"button\">ON</button></a>");
            } else {
              client.println("<a href=\"/evalv3ON\"><button class=\"button button2\">OFF</button></a>");
            } 
              client.println("</p>");
            // Display current state, and ON/OFF buttons for GPIO 27  
            client.println("<p>GPIO 27 - State placeholder </p>");
            // If the output27State is off, it displays the ON button       
            
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
}