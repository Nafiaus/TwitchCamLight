# TwitchCamLight
This is a my personal code (minus private data you will need to populate) for my NafiBot cam/light contoller for Twitch. If you have any questions add me on discord Nafiaus#4848 / Nafiaus and I'll answer them to the best of my ability

::::::::::::::::::::::::::::::::::::::::::::::::::

I used (all items from Adafruit.com):

2853 - Cool White NeoPixel Ring 12 GRBW LEDs, 

2471 - Adafruit HUZZAH ESP8266 Breakout, 

1967 - Mini Tilt Pan Servo Kit

::::::::::::::::::::::::::::::::::::::::::::::::::

You will have to populate: 

SSID with wifi name, 

PASS with wifi password, 

CHANNEL with the channel name you want to join, 

BOT_NAME with the name of your twitch bot, 

OAUTH with oauth token gained here: https://twitchapps.com/tmi/

::::::::::::::::::::::::::::::::::::::::::::::::::


You can keep variables the same or change for

BAUD  baud rate for serial communication, 

X_PIN  pin x-axis servo is attached to, 

Y_PIN  pin y-axis servo is attached to, 

MOVE_AMT  amount of movement per command call for servos, 

BRIGHTNESS_AMT  amount of bightness change per command call, 

LED_PIN  pin for led strip, 

LED_AMT  amount of leds on strip, 

RED_DEFAULT  default red amount, 

GREEN_DEFAULT  default green amount, 

BLUE_DEFAULT  default blue amount, 

WHITE_DEFAULT  default white amount, 

BRIGHTNESS_DEFAULT  default brightness

::::::::::::::::::::::::::::::::::::::::::::::::::


DO NOT CHANGE

Y_MIN,

Y_MAX,

X_MIN,

X_MAX,

HOST,

PORT 

:::::::::::::::::::::::::::::::::::::::::::::::::
