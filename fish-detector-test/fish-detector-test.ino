//#define FLIP_LOGIC_LEVEL
#include "setup.h"
float xc, yc;
bool start = false;

void setup() {
  Serial.begin(115200);
  //  pinMode(LED_PIN, OUTPUT);
  //  Ethernet.begin(mac, serverAddress, gateway, subnet);
  //  Serial.print("TCP Server IP address: ");
  //  Serial.println(Ethernet.localIP());
  //  TCPserver.begin();
//  while (!start) {
//    Serial.println("FSH");
//    if (Serial.available() > 0) {
//      Serial.println("incoming");
//      String inc = Serial.readStringUntil('\n');
//      Serial.print(">>>"); Serial.println(inc);
//      if (inc.startsWith("FSH")) start = true;
//    }
//    delay(500);
//  }
  Serial.println("fish detector test");
}
void loop() {
  updateSensors();

  switch (mode) {
    case IDLE:
      //wait for the fish at any of the gates
      if (gate1_fish_detected()) {
        mode = GATE_1_TRIGGERED;
        enter_ingate_timer = millis();
        Serial.println("Fish entered gate one");
        //        Sensor *ingate = gate1;
        //        fish_detected(ingate);
        //        if(ingate == gate1) Serial.println("XXX");
        //        if(ingate == gate2) Serial.println("YYY");
      }
      else if (gate2_fish_detected()) {
        mode = GATE_2_TRIGGERED;
        enter_ingate_timer = millis();
        Serial.println("Fish entered gate two");
      }
      break;

    case GATE_1_TRIGGERED:
      //wait for fish to swim through and measure time

      if (!gate1_fish_detected() && !left_ingate) {
        ingate_swim_time = millis() - enter_ingate_timer;
        left_ingate = true;
        Serial.print("Fish left gate one after "); Serial.print(float(ingate_swim_time) / 1000.0); Serial.println(" s");
      }
      //wait for fish to swim into another gate and measure time
      if (gate2_fish_detected() && !entered_outgate) {
        enter_outgate_timer = millis();
        intergate_swim_time_1 = enter_outgate_timer - enter_ingate_timer;
        entered_outgate = true;
        Serial.print("Fish eneterd gate two after "); Serial.print(float(intergate_swim_time_1) / 1000.0); Serial.println(" s");
      }
      if (!gate2_fish_detected() && entered_outgate) {
        outgate_swim_time = millis() - enter_outgate_timer;
        intergate_swim_time_2 = millis() - (enter_ingate_timer + ingate_swim_time);
        Serial.print("Fish left gate two after "); Serial.print(float(outgate_swim_time) / 1000.0); Serial.println(" s");
        //check data validity or add speed adjustment model
        //calculate(avg_pass_time, avg_swim_time,)
        reset();
      }

      //timeout
      if ( millis() - enter_ingate_timer > GATE_TIMEOUT_S * 1000) {
        Serial.println("timeout");
        reset();
      }

      break;

    case GATE_2_TRIGGERED:
      //won't be needed
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
      if(i != 21) Serial.print(",");
    }
    Serial.print("END");
    Serial.println();
    String str = "FISH1=";
    str+=isFish(gate1);
    str+=";";
    Serial.println(str);
    str = "FISH2=";
    str+=isFish(gate2);
    str+=";";
    Serial.println(str);
  }

  delay(20);
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
  mode = IDLE;
}

// Wait for a TCP client from Arduino #1:
//  EthernetClient client = TCPserver.available();
//  Serial.println("Waiting for a client...");
//  if (client) {
//    // Read the command from the TCP client:
//    char command = client.read();
//    Serial.print("- Received command: ");
//    Serial.println(command);
//
//    /*if (command == '1')
//      digitalWrite(LED_PIN, HIGH); // Turn LED on
//    else if (command == '0')
//      digitalWrite(LED_PIN, LOW);  // Turn LED off*/
//
//    //Ethernet.maintain();
//  }
