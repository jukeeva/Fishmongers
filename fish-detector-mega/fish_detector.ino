
// looking for 2+ adjacent crossed points
bool fish_detected(Sensor *s_array) {
  xc = -1;
  yc = -1;
  bool twoPerSide = false;
  bool pointFoundY = false;
  bool twoPointFoundY = false;
  int cnt = 0;
  // X axis
  for (int i = 0; i < SENSORS_PER_SIDE; i++) {

    if (s_array[i].enabled && s_array[i].isCrossed()) cnt++;
    else cnt = 0;
    
    if (cnt == 1 && !twoPerSide) {
      if ( xc < 0) xc = float(i);
      else return false; // multiple isolated points, ambiguity
    }
    
    if (cnt == 2 && !twoPerSide) {
      twoPerSide = true;
      xc = float(i) - 0.5;
    }
    else if (cnt == 2 && twoPerSide) return false; // multiple isolated patches, ambiguity   
  }
  if(xc < 0) return false; //no points detected at all, no need to proceed
  
  //Y axis:
  cnt = 0;
  for (int i = 0; i < SENSORS_PER_SIDE; i++) {

    if (s_array[i+SENSORS_PER_SIDE].enabled && s_array[i+SENSORS_PER_SIDE].isCrossed()) cnt++;
    else cnt = 0;
    
    //2 points at X:
    if (twoPerSide && cnt == 1) {
      if (!pointFoundY) {
        yc = float(i);
        pointFoundY = true;
      }
      else return false; // multiple isolated points, ambiguity  
    }
    
    //1 point at X:
    if (cnt == 2) {
      if (!twoPointFoundY) {
        yc = float(i) - 0.5;
        twoPointFoundY = true;
      }
      else return false; // multiple isolated patches, ambiguity  
    }

  }
  if ((twoPerSide && pointFoundY) || twoPointFoundY ) {
    //detecting which one of the beams was triggered first and recording triggering time
    int x,y,z;
    if( twoPerSide ) {
      x = int(xc);
      y = x+1;
      z = int(yc)+SENSORS_PER_SIDE;
    }
    else{
      x = int(yc)+SENSORS_PER_SIDE;
      y = int(yc)+1+SENSORS_PER_SIDE;
      z = int(xc);
    }
    fish_detection_time = min_long(s_array[x].state_changed_timer,s_array[y].state_changed_timer,s_array[z].state_changed_timer);

    return true;
  }
  return false;
}
unsigned long min_long(unsigned long x, unsigned long y, unsigned long z){
  unsigned long minval = x;
  if(y < minval) minval = y;
  if(z < minval) minval = z;
  return minval;
}
