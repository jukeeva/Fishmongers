/*User defined parameters*/
const float CELL_SIZE = 30.0; // in mm
const float DISTANCE_BETWEEN_GATES = 600.0; // in mm
const int SENSORS_PER_SIDE  = 11;

const int DEBOUNCE_TIME = 10; //filtering impulses less then X ms on sensors
#define GATE_TIMEOUT_S  20 //if another gate is not triggered in X s, the detector is reset
#define SENSOR_FAULT_TIMEOUT_S 60 // if sensors is crossed for more than X s, sensor readings are ignored 
/***********************/

const int SENSORS_TOTAL = SENSORS_PER_SIDE*4;
#include <Bounce2.h>
enum {IDLE, GATE_TRIGGERED};
int mode = IDLE;

unsigned long enter_ingate_timer = 0;
unsigned long enter_outgate_timer = 0;
unsigned long ingate_swim_time = 0;
unsigned long outgate_swim_time = 0;
bool left_ingate = false;
bool entered_outgate = false;
unsigned long intergate_swim_time_1 = 0;
unsigned long intergate_swim_time_2 = 0;
int defective_sensors_counter = 0;
float xc, yc, inX, inY, outX, outY;
unsigned long fish_detection_time = 0;

bool fishWasMeasured = false;
String fishData = "";

class Sensor{
  public:
  Bounce pin = Bounce();
  bool enabled = true;
  unsigned long state_changed_timer = millis();
  Sensor(int pinNum){
    pin.attach(pinNum,INPUT_PULLUP);
    pin.interval(DEBOUNCE_TIME);
  }
  bool isCrossed(){
    #ifdef FLIP_LOGIC_LEVEL
      return !pin.read();
    #else
      return pin.read();
    #endif
  }
};
//Sensor pins are declared here:
Sensor gate1[2*SENSORS_PER_SIDE] = {
  Sensor(28),Sensor(29),Sensor(30),Sensor(31),Sensor(32),Sensor(33),Sensor(34),Sensor(35),Sensor(36),Sensor(37),Sensor(38), //X
  Sensor(39),Sensor(40),Sensor(41),Sensor(42),Sensor(43),Sensor(44),Sensor(45),Sensor(46),Sensor(47),Sensor(48),Sensor(49)  //Y
};

Sensor gate2[2*SENSORS_PER_SIDE] = {
  Sensor(A1),Sensor(A2),Sensor(A3),Sensor(A4),Sensor(A5),Sensor(A6),Sensor(A7),Sensor(A8),Sensor(A9),Sensor(A10),Sensor(A11), //X
  Sensor(17),Sensor(18),Sensor(19),Sensor(20),Sensor(21),Sensor(22),Sensor(23),Sensor(24),Sensor(25),Sensor(26),Sensor(27)    //Y
};
Sensor *ingate, *outgate;
//updating sensors and disabling sensors that were triggered for too long
void updateSensors(){
  for(int i = 0; i< SENSORS_PER_SIDE*2; i++){
    gate1[i].pin.update();
    if( gate1[i].pin.changed() ) gate1[i].state_changed_timer = millis();
    if( gate1[i].enabled && gate1[i].isCrossed() && millis() - gate1[i].state_changed_timer > SENSOR_FAULT_TIMEOUT_S*1000) {
      gate1[i].enabled = false;
      defective_sensors_counter++;
      Serial.print(i);
      Serial.println(" sensor defective, gate 1");
    }
    gate2[i].pin.update();
    if( gate2[i].pin.changed() ) gate2[i].state_changed_timer = millis();
    if( gate2[i].enabled && gate2[i].isCrossed() && millis() - gate2[i].state_changed_timer > SENSOR_FAULT_TIMEOUT_S*1000) {
      gate2[i].enabled = false;
      defective_sensors_counter++;
      Serial.print(i);
      Serial.println(" sensor defective, gate 2");
    }
  }
}
