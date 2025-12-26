#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

WebServer server(80);

// Simple chat storage
#define MAX_MSG 100
String chatMessages[MAX_MSG];
int msgCount = 0;

// Button states
bool btn1State = false;
bool btn2State = false;

void addMessage(String msg) {
  if (msgCount < MAX_MSG) {
    chatMessages[msgCount++] = msg;
  } else {
    // shift all messages left
    for (int i = 1; i < MAX_MSG; i++) chatMessages[i-1] = chatMessages[i];
    chatMessages[MAX_MSG-1] = msg;
  }
}

String getChatHTML() {
  String html = "<div style='border:1px solid #ccc;padding:10px;height:200px;overflow:auto'>";
  for (int i = 0; i < msgCount; i++) {
    html += chatMessages[i] + "<br>";
  }
  html += "</div>";
  return html;
}

String getHTML() {
  String html = "<!DOCTYPE html><html><head><title>ESP32 Chat & Buttons</title></head><body>";
  
  html += "<h2>Buttons</h2>";
  html += "<form method='GET'>";
  html += "<button name='btn' value='1'>Button 1</button>";
  html += "<button name='btn' value='2'>Button 2</button>";
  html += "</form>";
  
  html += "<h2>Chat</h2>";
  html += getChatHTML();
  
  html += "<form method='GET'>";
  html += "Message: <input name='msg'><input type='submit' value='Send'>";
  html += "</form>";
  
  html += "</body></html>";
  return html;
}

void handleRoot() {
  if (server.hasArg("btn")) {
    if (server.arg("btn") == "1") btn1State = !btn1State;
    if (server.arg("btn") == "2") btn2State = !btn2State;
    addMessage("Button " + server.arg("btn") + (server.arg("btn")=="1"? (btn1State?" ON":" OFF") : (btn2State?" ON":" OFF")));
  }
  
  if (server.hasArg("msg")) {
    String msg = server.arg("msg");
    if (msg.length() > 0) addMessage(msg);
  }
  
  server.send(200, "text/html", getHTML());
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP: "); Serial.println(WiFi.localIP());
  
  server.on("/", handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
}
