#include <SPI.h>
#include <Ethernet.h>

const int LED_PIN = 13;
const int serverPort = 4080;

byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0xFF};
EthernetServer TCPserver = EthernetServer(serverPort);
byte serverAddress[4] = {192, 168, 0, 180};
byte gateway[4] = {192, 168, 0, 190};
byte subnet[4] = {255, 255, 255, 0};

#include <Bounce2.h>

const float CELL_SIZE = 30.0;
const float DISTANCE_BETWEEN_GATES = 600.0;
const int SENSORS_PER_SIDE  = 11;
const int SENSORS_TOTAL = SENSORS_PER_SIDE*4;
const int DEBOUNCE_TIME = 10;
const int GATE_TIMEOUT_S = 20; // 


enum {IDLE, GATE_1_TRIGGERED, GATE_2_TRIGGERED};
int mode = IDLE;
//ADD global for fish coords
// inX, inY; outX, outY
unsigned long enter_ingate_timer = 0;
unsigned long enter_outgate_timer = 0;
unsigned long ingate_swim_time = 0;
unsigned long outgate_swim_time = 0;
bool left_ingate = false;
bool entered_outgate = false;
unsigned long intergate_swim_time_1 = 0;
unsigned long intergate_swim_time_2 = 0;

class Sensor{
  public:
  Bounce pin = Bounce();
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

Sensor gate1[2*SENSORS_PER_SIDE] = {
  Sensor(28),Sensor(29),Sensor(30),Sensor(31),Sensor(32),Sensor(33),Sensor(34),Sensor(35),Sensor(36),Sensor(37),Sensor(38), //X
  Sensor(39),Sensor(40),Sensor(41),Sensor(42),Sensor(43),Sensor(44),Sensor(45),Sensor(46),Sensor(47),Sensor(48),Sensor(49)  //Y
};

Sensor gate2[2*SENSORS_PER_SIDE] = {
  Sensor(A1),Sensor(A2),Sensor(A3),Sensor(A4),Sensor(A5),Sensor(A6),Sensor(A7),Sensor(A8),Sensor(A9),Sensor(A10),Sensor(A11), //X
  Sensor(17),Sensor(18),Sensor(19),Sensor(20),Sensor(21),Sensor(22),Sensor(23),Sensor(24),Sensor(25),Sensor(26),Sensor(27)    //Y
};

void updateSensors(){
  for(int i = 0; i< SENSORS_PER_SIDE*2; i++){
    gate1[i].pin.update();
  }
  for(int i = 0; i< SENSORS_PER_SIDE*2; i++){
    gate2[i].pin.update();
  }
}
