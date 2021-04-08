/*
  DigitalReadSerial

  Reads a digital input on pin 2, prints the result to the Serial Monitor

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/DigitalReadSerial
*/

// digital pin 2 has a pushbutton attached to it. Give it a name:
int ir_input1 = 2;
int ir_input2 = 3;
int fish = 0;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  // make the pushbutton's pin an input:
  pinMode(ir_input1, INPUT);
  pinMode(ir_input2, INPUT);
  
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input pin:
  int IR1_State = digitalRead(ir_input1);
  // print out the state of the IR:
  Serial.println(IR1_State);
  ///////////////////////
  if (ir_input1==1 && ir_input2==0){
    if (ir_input1==1 && ir_input2==1){
      if (ir_input1==0 && ir_input2==1){
        if(ir_input1==0 && ir_input2==1){
          fish=1;
        }
        else {
          fish=0;
          }
        }
        else {
          fish=0;
          }
        }
        else {
          fish=0;
          }
        }
        
          else {
          fish=0;
          }
          
        
  delay(1);        // delay in between reads for stability
}
