#include <Arduino.h>
#include <M5Core2.h>
#include <SCServo.h>

// M5Stack Core2 PortA
const int rx_servo = 32;
const int tx_servo = 33;
static const int ids[] = {41, 42, 43};

const int cmd_max_sweep = 4095;
unsigned short speed = 3400;
unsigned char acc = 50;

static const int n_servo = sizeof(ids) / sizeof(int);
int angled_cmd[n_servo] = {0};
double angled_deg[n_servo] = {0};

SMS_STS st;

s16 conv_deg_cmd(double deg){
  s16 cmd = (deg+180)*cmd_max_sweep/360.0;
  return cmd;
}

double conv_cmd_deg(int cmd){
  double deg = -999;
  if(cmd>=0){
    deg = ((double)cmd / (cmd_max_sweep/360.0)) - 180;
  }
  return deg;
}

void setup(){

  M5.begin();
    
  M5.IMU.Init();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN , BLACK);
  M5.Lcd.setTextSize(2);

  Serial.begin(115200); // serial for debugging
  Serial2.begin(1000000, SERIAL_8N1, rx_servo, tx_servo);
  st.pSerial = &Serial2;

  delay(10);
}


void loop(){

  for(int deg=-180; deg<=180; deg+=90){
    
    M5.Lcd.setCursor(0, 0);

    // Send angles
    for(int i=0; i<n_servo; i++){
      st.RegWritePosEx(ids[i], conv_deg_cmd(deg), speed, acc);
    }
    st.RegWriteAction();
    delay(2000);

    // Read angles
    for(int i=0; i<n_servo; i++){
      angled_cmd[i] = st.ReadPos(ids[i]);
      angled_deg[i] = conv_cmd_deg(angled_cmd[i]);
      M5.Lcd.print("id");
      M5.Lcd.printf(String(ids[i]).c_str());
      M5.Lcd.print(": ");
      M5.Lcd.println(String(angled_deg[i]).c_str());
    }
    M5.update();

    delay(500);
  }

}