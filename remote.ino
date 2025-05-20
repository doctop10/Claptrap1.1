#define    STX          0x02
#define    ETX          0x03

byte cmd[8] = {0, 0, 0, 0, 0, 0, 0, 0};
byte buttonStatus = 0; 

void readControlParameters() {
  if (SerialBT.available())  {                           
    cmd[0] =  SerialBT.read();  
    if(cmd[0] == STX)  {
      int i = 1; 

      while (SerialBT.available())  {
        cmd[i] = SerialBT.read();
        if(cmd[i] > 127 || i > 7)                 
        break;  
        if((cmd[i] == ETX) && (i == 2 || i == 7)) 
        break;     
        i++;
      }

      if (i == 2) getButtonState(cmd[1]);                  
      else if (i == 7) getJoystickState(cmd);              
    }
  } 
}

void getJoystickState (byte data[8])    {
  int joyX = (data[1] - 48) * 100 + (data[2] - 48) * 10 + (data[3] - 48);   
  int joyY = (data[4] - 48) * 100 + (data[5] - 48) * 10 + (data[6] - 48);
  joyX = joyX - 200;                                                  
  joyY = joyY - 200;                                                  

  if (joyX < -100 || joyX > 100 || joyY < -100 || joyY > 100) return; 
  if (joyX < - 10 || joyX > 10)  { 
    if (joyX > 0) 
      steering_remote = (-joyX * joyX + 0.1 * joyX) / 100.0;
    else   
      steering_remote = (joyX * joyX + 0.1 * joyX) / 100.0;
  } else 
      steering_remote = 0;
  steering_remote = map(steering_remote, -100, 100, -STEERING_MAX, STEERING_MAX);
  if (joyY < - 10 || joyY > 10)   
     speed_remote = map(joyY, 100, -100, SPEED_MAX, -SPEED_MAX);
  else
     speed_remote = 0;        
}

void getButtonState (int bStatus)  {
  switch (bStatus) {

   case 'A':
      buttonStatus |= B000001;        
      break;
    case 'B':
      buttonStatus &= B111110;        
      break;
   case 'C':
      buttonStatus |= B000010;        
      break;
    case 'D':
      buttonStatus &= B111101;        
      break;
  }
}

String getButtonStatusString()  {
  String bStatus = "";
  for (int i = 0; i < 6; i++)  {
    if (buttonStatus & (B100000 >> i))      bStatus += "1";
    else                                    bStatus += "0";
  }
  return bStatus;
}