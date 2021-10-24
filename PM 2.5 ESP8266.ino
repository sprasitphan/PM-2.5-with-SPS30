#define BLYNK_PRINT Serial //COMMENT THIS OUT TO DISABLE PRINTS AND SAVE SPACE

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <sps30.h>

char ssid[] = "YOUR WI-FI NAME"; // YOUR WIFI CREDENTIALS
char pass[] = ""; // SET PASSWORD WIFI TO "" FOR OPEN NETWORKS
char auth[] = "YOUR AUTH TOKEN"; //AUTH TOKEN IN THE BLYNK APP

BlynkTimer timer;

WidgetBridge bridge1(V1); // BRIDGE WIDGET ON VIRTUAL PIN 1 (YOU CAN CHANGE ANOTHER VIRTUAL PIN)

BLYNK_CONNECTED() { //PLACE THE AUTH TOKEN OF THE SECOND HARDWARE HERE
  bridge1.setAuthToken("OTHER AUTH TOKEN"); 
}

void sendSensor()
{ struct sps30_measurement m;
  char serial[SPS30_MAX_SERIAL_LEN];
  uint16_t data_ready;
  int16_t ret;

  do {
    ret = sps30_read_data_ready(&data_ready);
    if (ret < 0) {
      Serial.print("ERROR READING DATA-READY FLAG: ");
      Serial.println(ret);
    } else if (!data_ready)
      Serial.print("DATA NOT READY, NO NEW MEASUREMENT AVAILABLE\n");
    else
      break;
    delay(1000); // RETRY IN 1000 ms 
  } while (1);
  ret = sps30_read_measurement(&m);
  if (ret < 0) {
    Serial.print("ERROR READING MEASUREMENT\n");
  } 
  else {
    float pm2p5;
    if (m.mc_2p5 < 10) {
      pm2p5 = m.mc_2p5*2;
    } else {
      pm2p5 = m.mc_2p5*(1+(1/log10(m.mc_2p5)));
    }

    if(!Blynk.connected()){    
      Blynk.disconnect();
      delay(1000);
      if(WiFi.status() == WL_CONNECTED){
        Blynk.begin(auth, ssid, pass, "blynk.cloud", 8080);
        Blynk.connect();  
        delay(10000);
      }
    }
    else{    
    bridge1.virtualWrite(V1, pm2p5); // SEND VALUE OF PM 2.5 TO BLYNK_WRITE(V1) HANDLER ON RECEIVING SIDE
    Blynk.virtualWrite(V1, pm2p5); // SEND VALUE OF PM 2.5 TO BLYNK APP
    }
    Serial.print("PM 2.5 : ");
    Serial.println(pm2p5);
  }
}

void setup(){
  int16_t ret;
  uint8_t auto_clean_days = 4;
  uint32_t auto_clean;

  Serial.begin(115200); // DEBUG CONSOLE

  Blynk.begin(auth, ssid, pass, "blynk.clound", 8080); 
  // YOU CAN ALSO SPECIFY SERVER:
  // Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
  
  Serial.print("SETTING UP.");
  
  delay(2000);
  
  sensirion_i2c_init();

  while (sps30_probe() != 0) {
    Serial.print("SPS SENSOR PROBING FAILED\n");
    delay(500);
  }

  delay(30000);
  
  ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
  if (ret) {
    Serial.print("error setting the auto-clean interval: ");
    Serial.println(ret);
  }

  ret = sps30_start_measurement();
  if (ret < 0) {
    Serial.print("ERROR STARTING MEASUREMENT\n");
  }
  timer.setInterval(1000L, sendSensor);   // SETUP A FUNCTION TO BE CALLED EVERY SECOND
}

void loop(){
      Blynk.run();
      timer.run();
}

