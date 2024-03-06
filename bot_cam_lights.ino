#include <ESP8266WiFi.h> // Allow us use of the esp8266 huzzah
#include <Servo.h> // Controlling servos with the board
#include <IRCClient.h> // Lets us connect to twitch
#include <Adafruit_NeoPixel.h> // Lets us control the leds

// Declare your wifi connection information
#define SSID "" // wifi 2.4GHz name
#define PASS "" // wifi 2.4GHz password

// Declare channel information
String CHANNEL = ""; // use all lowercase
#define BOT_NAME "" // capitalization allowed
#define OAUTH "" // use oauth token for account used as bot

// Declare arduino information
#define BAUD 115200
#define X_PIN 12
#define Y_PIN 13
#define LED_PIN 14
#define LED_AMT 12

// Declare servo min and max rotations
#define Y_MIN 75
#define Y_MAX 150
#define X_MIN 0
#define X_MAX 180
#define X_DEFAULT 60
#define Y_DEFAULT 110
#define MOVE_AMT 5
#define BRIGHTNESS_AMT 10

// Declare default colors and brightness
#define RED_DEFAULT 0
#define GREEN_DEFAULT 0
#define BLUE_DEFAULT 0
#define WHITE_DEFAULT 255
#define BRIGHTNESS_DEFAULT 150
#define BRIGHTNESS_MAX 250
#define BRIGHTNESS_MIN 10

// Declare servos/position, leds, and irc channel
int xpos = X_DEFAULT;
int ypos = Y_DEFAULT;
int red = RED_DEFAULT;
int green = GREEN_DEFAULT;
int blue = BLUE_DEFAULT;
int white = WHITE_DEFAULT;
int brightness = BRIGHTNESS_DEFAULT;
Servo xservo;
Servo yservo;
String ircChannel = "";
WiFiClient wifi;
IRCClient client("irc.chat.twitch.tv", 6667, wifi);
Adafruit_NeoPixel ring(LED_AMT, LED_PIN, NEO_GRBW + NEO_KHZ800);

// Runs once so setup and initalize all variables and processes.
void setup() {
  Serial.begin(BAUD);
  xservo.attach(X_PIN);
  yservo.attach(Y_PIN);
  xservo.write(xpos);
  yservo.write(ypos);
  ring.begin();
  ring.setBrightness(BRIGHTNESS_DEFAULT);
  for(int i = 0; i < LED_AMT; i++) {
    changeColor(RED_DEFAULT, GREEN_DEFAULT, BLUE_DEFAULT, WHITE_DEFAULT);
  }
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Wifi Connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  ircChannel = "#" + CHANNEL;

  client.setCallback(callback);
}

// Main loop code, all the juicy happens here
void loop() {
  // Try to connect to chat. If it loses connection try again
  if (!client.connected()) {
    Serial.println("Attempting to connect to " + ircChannel );
    // Attempt to connect
    // Second param is not needed by Twtich
    if (client.connect(BOT_NAME, "", OAUTH)) {
      client.sendRaw("JOIN " + ircChannel);
      Serial.print("Connected to www.twitch.tv/");
      Serial.println(CHANNEL);
      sendTwitchMessage("Camera Servos have been connected!");
    } else {
      Serial.println("failed... try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
    return;
  }
  client.loop();
}

// INTERNET CONNECTION CODE

void sendTwitchMessage(String message) {
  client.sendMessage(ircChannel, message);
}

void callback(IRCMessage ircMessage) {
  Serial.println("In CallBack");

  // Get username and text then print to serial line
  if (ircMessage.command == "PRIVMSG" && ircMessage.text[0] != '\001') {
    Serial.println("Passed private message.");
    ircMessage.nick.toUpperCase();
    String message("<" + ircMessage.nick + "> " + ircMessage.text);
    Serial.println(message);

    // Start of "main loop"
    // Parse message look for keywords and run commands if found
    if(ircMessage.text == "!up") {
      moveUp(MOVE_AMT);
      sendTwitchMessage("Camera moved up!");
      Serial.println("Moved up");
    } else if(ircMessage.text == "!down") {
      moveDown(MOVE_AMT);
      sendTwitchMessage("Camera moved down!");
      Serial.println("Moved down");
    } else if(ircMessage.text == "!left") {
      moveLeft(MOVE_AMT);
      sendTwitchMessage("Camera moved left!");
      Serial.println("Moved left");
    } else if(ircMessage.text == "!right") {
      moveRight(MOVE_AMT);
      sendTwitchMessage("Camera moved right!");
      Serial.println("Moved right");
    } else if(ircMessage.text == "!camreset") {
      xpos = X_DEFAULT;
      ypos = Y_DEFAULT;
      xservo.write(xpos);
      yservo.write(ypos);
      printPos();
      changeColor(RED_DEFAULT, GREEN_DEFAULT, BLUE_DEFAULT, WHITE_DEFAULT);
    } else if(ircMessage.text == "!lightwhite") {
      changeColor(0,0,0,255);
      sendTwitchMessage("Light turned color to white.");
      Serial.println("Light turned to white");
    } else if(ircMessage.text == "!lightred") {
      changeColor(0,255,0,0);
      sendTwitchMessage("Light turned color to red.");
      Serial.println("Light turned to red");
    } else if(ircMessage.text == "!lightyellow") {
      changeColor(255,255,0,0);
      sendTwitchMessage("Light turned color to yellow.");
      Serial.println("Light turned to yellow");
    } else if(ircMessage.text == "!lightgreen") {
      changeColor(255,0,0,0);
      sendTwitchMessage("Light turned color to green.");
      Serial.println("Light turned to green");
    } else if(ircMessage.text == "!lightblue") {
      changeColor(0,0,255,0);
      sendTwitchMessage("Light turned color to blue.");
      Serial.println("Light turned to blue");
    } else if(ircMessage.text == "!lightcyan") {
      changeColor(255,0,255,0);
      
      sendTwitchMessage("Light turned color to cyan.");
      Serial.println("Light turned to cyan");
    } else if(ircMessage.text == "!lightmagenta") {
      changeColor(0,255,255,0);
      sendTwitchMessage("Light turned color to magenta.");
      Serial.println("Light turned to magenta");
    } else if(ircMessage.text == "!lightoff") {
      changeColor(0,0,0,0);
      ring.setBrightness(0);
      
      sendTwitchMessage("Light has turned off.");
      Serial.println("Light turned off");
    } else if(ircMessage.text == "!lighton") {
      changeColor(RED_DEFAULT, GREEN_DEFAULT, BLUE_DEFAULT, WHITE_DEFAULT);
      ring.setBrightness(BRIGHTNESS_DEFAULT);
      sendTwitchMessage("Light turned on and color to white.");
      Serial.println("Light turned on");
    } else if(ircMessage.text == "!brightnessup") {
      changeBrightness(true, 10);
      sendTwitchMessage("Brightness has turned up.");
      Serial.println("Brightness up");
    } else if(ircMessage.text == "!brightnessdown") {
      changeBrightness(false, 10);
      sendTwitchMessage("Brightness has turned down.");
      Serial.println("Brightness down");
    } else if(ircMessage.text == "!flashbang") {
      flashbang(xpos, ypos, brightness, red, green, blue, white);
      sendTwitchMessage("BOOM FLASHBANG!");
      Serial.println("Flashbang done.");
    } else {
      Serial.println("cam did nothing");
    }
    // End of "main loop"
    return;
  }
}

// LED RING COMMANDS

void changeColor(int r, int g, int b, int w) {
  for(int i = 0; i < LED_AMT; i++) {
    ring.setPixelColor(i, g, r, b, w);
    ring.show();
    delay(10);
  }
}

void flashbang(int x_pos, int y_pos, int bright, int r, int g, int b, int w) {
  int oldx = x_pos;
  int oldy = y_pos;
  int oldbright = bright;
  int oldr = r;
  int oldg = g;
  int oldb = b;
  int oldw = w;
  xservo.write(X_DEFAULT);
  yservo.write(Y_DEFAULT);
  for(int i = 0; i < LED_AMT; i++) {
    ring.setPixelColor(i, 0, 0, 0, 255);
    ring.show();
    delay(10);
  }
  ring.setBrightness(255);
  ring.show();
  delay(3000);
  xservo.write(oldx);
  yservo.write(oldy);
  for(int i = 0; i < LED_AMT; i++) {
    ring.setPixelColor(i, oldr, oldg, oldb, oldw);
    ring.show();
    delay(10);
  }
  ring.setBrightness(oldbright);
  ring.show();
}

void changeBrightness(bool up, int amt) {
  if(up == true) {
    if(brightness == BRIGHTNESS_MAX) {
      sendTwitchMessage("Cannot go brighter than MAX BRIGHTNESS...");
    } else {
      brightness += amt;
    }
  } else if(up == false) {
    if(brightness == BRIGHTNESS_MIN) {
      sendTwitchMessage("Cannot go dimmer...");
    } else {
      brightness -= amt;
    }
  }
  ring.setBrightness(brightness);
  ring.show();
}

// MOVEMENT CODE FOR SERVOS

void moveDown(int amt) {
if(ypos > Y_MIN) {
    ypos -= amt;
    yservo.write(ypos);
    Serial.println(ypos);
    printPos();
  } else {
    Serial.println("Cannot move down further...");
    sendTwitchMessage("Cannot move down further...");
  }
}

void moveUp(int amt){
if(ypos < Y_MAX) {
    ypos += amt;
    yservo.write(ypos);
    Serial.println(ypos);
    printPos();
  } else {
    Serial.println("Cannot move up further..");
    sendTwitchMessage("Cannot move up further..");
  }
}

void moveLeft(int amt){
  if(xpos > X_MIN) {
    xpos -= amt;
    xservo.write(xpos);
    Serial.println(xpos);
    printPos();
  } else {
    Serial.println("Cannot move left further...");
    sendTwitchMessage("Cannot move left further...");
  }
}

void moveRight(int amt) {
  if(xpos < X_MAX) {
    xpos += amt;
    xservo.write(xpos);
    Serial.println(xpos);
    printPos();
  } else {
    Serial.println("Cannot move right further...");
    sendTwitchMessage("Cannot move right further...");
  }
}

void printPos() {
  Serial.print("x position: ");
  Serial.println(xpos);
  Serial.print("y position: ");
  Serial.println(ypos);
}
