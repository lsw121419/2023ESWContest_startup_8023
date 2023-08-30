#include <Stepper.h>
#include <pm2008_i2c.h>
#include <DFRobot_DHT11.h>

#define DHT11_PIN A1

#define rotate_loop_cnt 10
#define loop_cnt 2048
int is_open = 0;
const int stepsPerRevolution = 64;

DFRobot_DHT11 DHT;
PM2008_I2C pm2008_i2c;
Stepper myStepper(stepsPerRevolution, 11,9,10,8);



void open_window(int loopcnt);
void close_window(int loopcnt);

void setup() {
  myStepper.setSpeed(500);
  // put your setup code here, to run once:
  Serial.begin(9600);
  pm2008_i2c.begin();
  pm2008_i2c.command();

  delay(1000);
}

void loop() {
  DHT.read(DHT11_PIN);
  uint8_t ret = pm2008_i2c.read();
  
  if(DHT.humidity>=80 && is_open == 0){
    open_window(5);
    is_open = 1;
  }
  else if(DHT.humidity<80 && is_open == 1){
    close_window(5);
    is_open = 0;
  }
  

  
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);

  Serial.print("PM 2.5 (GRIMM) :  ");
  Serial.println(pm2008_i2c.pm2p5_grimm);
  Serial.print("PM 10 (GRIMM) :  ");
  Serial.println(pm2008_i2c.pm10_grimm);

}

void open_window(int loopcnt){
  for(int _ = 0;_<loopcnt;_++){
    for(int i = 0; i<32; i++){
      myStepper.step(-stepsPerRevolution);
    }  
  }
}
void close_window(int loopcnt){
  for(int _ = 0;_<loopcnt;_++){
    for(int i = 0; i<32; i++){
      myStepper.step(stepsPerRevolution);
    }
  }
}
