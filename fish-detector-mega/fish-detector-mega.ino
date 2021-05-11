//#define FLIP_LOGIC_LEVEL
//#define Serial3 Serial
#include "setup.h"

void setup() {
  Serial.begin(115200);
  Serial.println("fish detector mega");
  Serial3.begin(115200);
}

void loop() {
  
  if(Serial3.available() > 0){
    String incoming = Serial3.readStringUntil('\n');
    if(incoming.startsWith("POLL")){
      if(fishWasMeasured){
        fishWasMeasured = false;
        Serial3.println(fishData);
        fishData = "";
      }
      else{
        Serial3.print("SENSORS=");
        Serial3.print(defective_sensors_counter);
        Serial3.println(";");
      }
    }
  }
  updateSensors();

  switch (mode) {
    case IDLE:
      //wait for the fish at any of the gates

      if ( fish_detected(gate1) ) {
        ingate = gate1;
        outgate = gate2;
        mode = GATE_TRIGGERED;
        enter_ingate_timer = fish_detection_time; //millis();
        inX = xc;
        inY = yc;
        Serial.print(enter_ingate_timer);
        Serial.println("Fish entered gate one");
      }
      else if ( fish_detected(gate2) ) {
        ingate = gate2;
        outgate = gate1;
        mode = GATE_TRIGGERED;
        enter_ingate_timer = fish_detection_time; //millis();
        inX = xc;
        inY = yc;
        Serial.println("Fish entered gate two");

      }
      break;

    case GATE_TRIGGERED:
      //wait for fish to swim through and measure time
      if (!fish_detected(ingate) && !left_ingate) {
        ingate_swim_time = millis() - enter_ingate_timer;
        left_ingate = true;
        Serial.print("Fish left ingate after "); Serial.print(float(ingate_swim_time) / 1000.0); Serial.println(" s");
      }
      //wait for fish to swim into another gate and measure time
      if (fish_detected(outgate) && !entered_outgate) {
        enter_outgate_timer = fish_detection_time;//millis();
        intergate_swim_time_1 = enter_outgate_timer - enter_ingate_timer;
        entered_outgate = true;
        outX = xc; 
        outY = yc;
        Serial.print("Fish eneterd outgate after "); Serial.print(float(intergate_swim_time_1) / 1000.0); Serial.println(" s");
      }
      if (!fish_detected(outgate) && entered_outgate) {
        outgate_swim_time = millis() - enter_outgate_timer;
        intergate_swim_time_2 = millis() - (enter_ingate_timer + ingate_swim_time);
        Serial.print("Fish left outgate after "); Serial.print(float(outgate_swim_time) / 1000.0); Serial.println(" s");
        
        //calculating fish size and error
        unsigned long avg_pass_time = (ingate_swim_time + outgate_swim_time) / 2;
        unsigned long pass_delta = delta(ingate_swim_time, outgate_swim_time);
        unsigned long avg_swim_time = (intergate_swim_time_1 + intergate_swim_time_2) / 2 ;
        unsigned long swim_delta = delta(intergate_swim_time_1,intergate_swim_time_2);
        float error = (float(pass_delta)/float(avg_pass_time)) + (float(swim_delta)/float(avg_swim_time));
//        Serial.println(pass_delta);
//        Serial.println(avg_pass_time);
//        Serial.println();
//        Serial.println(swim_delta);
//        Serial.println(avg_swim_time);
        
        float size  = get_size(float(avg_pass_time)/1000.0, float(avg_swim_time)/1000.0);
        bool dir = 1; // IN 
        if( ingate == gate2) dir = 0; //OUT
        fishData = "FISH!SIZE=";
        fishData+=size;
        fishData+=";DIR=";
        fishData+=dir;
        fishData+=";ERROR=";
        fishData+=error;
        fishData+=";";
        Serial.println(fishData);
        fishWasMeasured = true;
        reset();
        break;
      }

      //timeout
      if ( millis() - enter_ingate_timer > GATE_TIMEOUT_S * 1000) {
        Serial.println("timeout");
        reset();
      }

      break;

  }

  static unsigned long timer = millis();
  if (millis() - timer > 50) {
    timer = millis();
    Serial.print("FSH");
    for (int i = 0; i < SENSORS_PER_SIDE * 2; i++) {
      Serial.print(gate1[i].isCrossed()); Serial.print(",");
    }
    for (int i = 0; i < SENSORS_PER_SIDE * 2; i++) {
      Serial.print(gate2[i].isCrossed());
      if (i != 21) Serial.print(",");
    }
    Serial.print("END");
    Serial.println();
    String str = "FISH1=";
    str += fish_detected(gate1);
    str += ";";
    Serial.println(str);
    str = "FISH2=";
    str += fish_detected(gate2);
    str += ";";
    Serial.println(str);
  }

  delay(10);
}
unsigned long delta(unsigned long a,unsigned long b){
  unsigned long diff = a>b?a-b:b-a;
  return diff/2;
}
void reset() {
  enter_ingate_timer = 0;
  enter_outgate_timer = 0;
  ingate_swim_time = 0;
  outgate_swim_time = 0;
  left_ingate = false;
  entered_outgate = false;
  intergate_swim_time_1 = 0;
  intergate_swim_time_2 = 0;
  inX = 0;
  inY = 0;
  outX = 0;
  outY = 0;
  mode = IDLE;
}
float get_size(float pass_time, float swim_time){
  float xy_distance = distance((inX+1)*CELL_SIZE ,(inY+1)*CELL_SIZE,(outX+1)*CELL_SIZE,(outY+1)*CELL_SIZE); //mm
  float swim_distance = sqrt( sq(DISTANCE_BETWEEN_GATES) + sq(xy_distance) ); // mm
  float v = swim_distance/swim_time; // mm/s
//  Serial.println(xy_distance);
//  Serial.println(swim_distance);
//  Serial.println(v);
  return pass_time*v; //mm
}
float distance(float x1,float y1,float x2,float y2){
  return sqrt( sq(x1-x2)+ sq(y1-y2) );
}
