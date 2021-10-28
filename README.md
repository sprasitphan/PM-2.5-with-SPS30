# PM 2.5 with SPS30
Sensirion PM SPS30 for ESP32 & ESP8266 that operate and communicate through the I2C interface. I use this code work with **Blynk**, **NODE-RED** and **ThingsBoard**. Using **Bridge** function of **Blynk** for recieving data of PM 2.5 from sensor and send to **NODE-RED** that work similarly to MQTT and UDP.
Using **NODE-RED** for recieving data of PM 2.5 from **Blynk** and send to **ThingsBoard** for display on dashboard. **You can edit and change code following to your usage**

# Installation in Arduino IDE
**1.** Click "File", "Preferences" and add boards manager URLs.

```
http://arduino.esp8266.com/stable/package_esp8266com_index.json,https://dl.espressif.com/dl/package_esp32_index.json
```

**2.** Click **"Tools"**, **"Board"**, **"Manager Boards"** and search **"esp32 by Espressif Systems"** and **"esp8266 by ESP8266 Community"** to install.

**3.** Click **"Sketch"**, **"Include Library"**, **"Manage Libraries"** and search **"sensirion-sps by Johannes Winkelmann"** and **"Blynk by Volodymyr Shymanskyy"** to install. **[If you don't use blynk, you don't need to install Blynk's library]**

# Getting Started with Blynk App 
You can learn from [docs.blynk ](https://docs.blynk.cc/)
[If you don't use blynk, you can skip this step]

# Usage
**1.** Create 2 projects and choose main and other project

**2.** Add your Wi-Fi name in **"YOUR WI-FI NAME"**, add your auth token of main project in **"YOUR AUTH TOKEN"**, choose virtual pin for bridge widget and add your auth token of other project in **"OTHER AUTH TOKEN"**.   
```
char ssid[] = "YOUR WI-FI NAME"; // YOUR WIFI CREDENTIALS
char pass[] = ""; // SET PASSWORD WIFI TO "" FOR OPEN NETWORKS
char auth[] = "YOUR AUTH TOKEN"; //AUTH TOKEN IN THE BLYNK APP
```
```
WidgetBridge bridge1(V1); // BRIDGE WIDGET ON VIRTUAL PIN 1 (YOU CAN CHANGE ANOTHER VIRTUAL PIN)
```
```
BLYNK_CONNECTED() { //PLACE THE AUTH TOKEN OF THE SECOND HARDWARE HERE
  bridge1.setAuthToken("OTHER AUTH TOKEN"); 
}
```
**3.** Connect board with Sensirion PM SPS30 and upload code

**4.** In **NODE-RED**, Install node pallete of Blynk **"node-red-contrib-blynk-ws"** and use **Blynk write event** node. Then, connect your other project and choose virtual pin. **[If you choose virtual pin 1 of your main project for sending data , you have to choose virtual pin 1 of your other project]**

**5.** In **NODE-RED**, Sending data to Thingsboard with using **http request** node. You can learn how to connect thingsboard from [thingsboard.io ](https://thingsboard.io/docs/)
