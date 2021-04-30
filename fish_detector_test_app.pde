import processing.serial.*;
Serial dataPort; 
static int PORTSPEED = 115200;
static int PORT_TIMEOUT = 5000;
int tx,ty;
String txt = "start";
int sensors[] = new int[44];
  String gate1Status = "NO FISH";
  String gate2Status = "NO FISH";
void setup(){
  size(800,400);
  for(int i = 0; i < 44; i++) sensors[i] = 0;
  connect();
  smooth(4);
  noStroke();
}
void draw(){
  background(240);
  translate((width-520)/2,(height-240)/2);
  text(gate1Status,0,-20);
  text(gate2Status,12*20+40,-20);
  fill(15);
  rect(0,0,12*20,12*20);
  rect (12*20+40,0,12*20,12*20);
  for(int i = 0; i < 11; i++){
    stroke(220);
    if(sensors[i] == 0) line(20+i*20,0,20+i*20,12*20);
    if(sensors[i+11] == 0) line(0,20+i*20,12*20,20+i*20);
    int offs =12*20+40;
    if(sensors[i+22] == 0) line(offs+20+i*20,0,20+i*20+offs,12*20);
    if(sensors[i+33] == 0) line(offs,20+i*20,12*20+offs,20+i*20);
  }
  //for(int i = 0; i < 44; i++){
  //  if(sensors[i] == 1) fill(130);
  //  else fill(15);
  //  ellipse((i/11)*20,(i%11)*20,10,10);
  //}
}

void serialEvent (Serial myPort) {
  try {
    String inString = myPort.readStringUntil('\n');
    parse(inString);
  }
  catch(RuntimeException e) {
    e.printStackTrace();
  }
}
void parse(String str) {
  str = str.trim();
  if (str.startsWith("FSH") && str.endsWith("END") ) {
    str = readBetween(str, "FSH", "END");
    String[] dataset = split(str, ',');  
    if (dataset.length == 44) {
      for(int i = 0; i < 44; i++){
        sensors[i] = int(dataset[i]);
      }
      
    }
    
  }

  else if(str.startsWith("FISH1") && str.endsWith(";")){
    if(int(readBetween(str, "FISH1=", ";")) == 1) gate1Status = "FISH DETECTED";
    else gate1Status = "NO FISH";
  }
  else if(str.startsWith("FISH2") && str.endsWith(";")){
    if(int(readBetween(str, "FISH2=", ";")) == 1) gate2Status = "FISH DETECTED";
    else gate2Status = "NO FISH";
  }
  else println(str);
}

void connect() {
  println("CONNECTING...");
  println(Serial.list());
  String port  = Serial.list()[0];//findPort();
  if (port != null) {
    dataPort = new Serial(this, port, PORTSPEED);
    dataPort.bufferUntil('\n');
    dataPort.write("FSH"+'\n');
    println("CONNECTED TO " +port) ;
  } else {
    println("NO DEVICES FOUND");
  }
}
String findPort() {
  printArray(Serial.list());
  Serial tempPort;
  for (int i = 0; i< Serial.list().length; i++) {
    tempPort = new Serial(this, Serial.list()[i], PORTSPEED);
    long timer = millis();
    while (millis()-timer < PORT_TIMEOUT) {
      delay(100);
      if (tempPort.available()>0) {
        String inString = tempPort.readStringUntil('\n');
        if (inString != null) {
          inString = inString.trim();
          if (inString.startsWith("FSH") ) {
            tempPort.stop();
            return Serial.list()[i];
          }
        }
      }
    }
  }
  return null;
}
String readBetween(String str, String Start, String End) {
  int st = str.indexOf(Start)+Start.length();
  int en = str.indexOf(End, st);
  if (st != -1 && en != -1) {
    return str.substring(st, en);
  } else return "";
}
