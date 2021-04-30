bool gate1_fish_detected() {
  return isFish(gate1);
}
bool gate2_fish_detected() {
  return isFish(gate2);
}

bool fish_detected(Sensor *s_array) {
  return isFish(s_array);
}

bool isFish(Sensor *s_array) {
  xc = -1;
  yc = -1;
  bool twoPerSide = false;
  bool threePerSide = false;
  bool multiplePointsX = false;
  bool pointFoundY = false;
  bool twoPointFoundY = false;
  int cnt = 0;
//  bool gateX[] = {0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0};
//  bool gateY[] = {0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0};
  // looking for 3+ adjacent points
  // X axis
  for (int i = 0; i < SENSORS_PER_SIDE; i++) {

    if (/*gateX[i]*/ s_array[i].isCrossed()) cnt++;
    else cnt = 0;
    if (cnt == 1 && !twoPerSide) {
      if ( xc < 0) xc = float(i);
      else multiplePointsX = true;
    }
    if (cnt == 2 && !twoPerSide) {
      twoPerSide = true;
      xc = float(i) - 0.5;
    }
    else if (cnt == 2 && twoPerSide) {
      Serial.println("ERROR");
      return false;
    }
    if (cnt == 3) {
      threePerSide = true;
      xc = float(i - 1);
    }
  }
  //Y axis:
  cnt = 0;
  for (int i = 0; i < SENSORS_PER_SIDE; i++) {

    if (s_array[i+SENSORS_PER_SIDE].isCrossed()/*gateY[i]*/) cnt++;
    else cnt = 0;
    if (threePerSide && cnt == 1) {
      if (!pointFoundY) {
        yc = float(i);
        pointFoundY = true;
      }
      else {
        //multiple points Y
        //Serial.println("ERROR");
        return false;
      }
    }
    if (twoPerSide && cnt == 2) {
      if (!twoPointFoundY) {
        yc = float(i) - 0.5;
        twoPointFoundY = true;
      }
      else {
        //Serial.println("ERROR");
        return false;
      }
    }
    if (cnt == 3 && !multiplePointsX) {
      yc = float(i - 1);
      return true;
    }
  }
  if (threePerSide && pointFoundY) return true;
  if (twoPerSide && twoPointFoundY) return true;
  return false;
}


//int neighbours(bool *aX, bool *aY, int x, int y){ //can be replaced with looking for three cons ones in one row, or 2 cons 1 in both rows
//  int n = 0;
//  //int map[] = {x-1,y-1,x-1,y,x-1,y+1,x,y-1,x,y+1,x+1,y-1,x+1,y,x+1,y+1};
//  int map[] = {x-1,y,x,y-1,x,y+1,x+1,y};
//  for(int i = 0; i < 7; i+=2){
//    if(map[i] > 0 && map[i+1] > 0 && map[i] < SENSORS_PER_SIDE && map[i+1] < SENSORS_PER_SIDE) n+= (aX[map[i]] && aY[map[i+1]]);
//  }
//  return n;
//}
