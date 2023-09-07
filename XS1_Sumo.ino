/* JSumo Micro Sumo Robot Code
* Model Number: XS1
* Board Model: XMotion Micro
* JS40F Opponent sensors give Logic 1 When Opponent Sensor Seen
* ML1 Line Sensor gives Logic 0 when white Line seen.
* Buttons give Logic 0 when button is pushed
* Leds Turn on with High (Logic 1)
*/
// #define DEBUG
//// Opponent Sensors Decleration ////
#define Right_Op_Sensor A0
#define Front_Op_Sensor A1
#define Left_Op_Sensor A2
//// Line Sensor Decleration ////
#define Line_Sensor A3
//// User Buttons & User Leds Decleration ////
#define Right_Button 8
#define Left_Button 12
#define Led1 4
#define Led2 6
//// Motor Control Pins Decleration ////
#define Right_Motor_Direction 10
#define Right_Motor_Speed 9
#define Left_Motor_Direction 13
#define Left_Motor_Speed 5
//// Speed & Last Remembered Sensor Variables ////
int Speed2=200;
int Last_Value=1;

//// Main Motor Control Function ////
void Motor(int LeftMotorValue, int RightMotorValue) {

  if (LeftMotorValue < 0) {
    LeftMotorValue = abs(LeftMotorValue);
    digitalWrite(Right_Motor_Direction, LOW);
    analogWrite(Right_Motor_Speed, LeftMotorValue);
  }
  else if (LeftMotorValue > 0){
    digitalWrite(Right_Motor_Direction, HIGH);
    analogWrite(Right_Motor_Speed, 255 -LeftMotorValue);
  }
  else
  {
    digitalWrite(Right_Motor_Direction, HIGH);
    analogWrite(Right_Motor_Speed, 255);
  }
  if (RightMotorValue < 0) {
    RightMotorValue = abs(RightMotorValue);
    digitalWrite(Left_Motor_Direction, LOW);
    analogWrite(Left_Motor_Speed, RightMotorValue);
  }
  else if (RightMotorValue > 0) {
    digitalWrite(Left_Motor_Direction, HIGH);
    analogWrite(Left_Motor_Speed, 255 - RightMotorValue);
  }
  else
  {
    digitalWrite(Left_Motor_Direction, HIGH);
    analogWrite(Left_Motor_Speed, 255);
  }
}

//// Motors Stopping Function ////
void MotorStop()
{
  digitalWrite(Left_Motor_Direction, HIGH);
  analogWrite(Left_Motor_Speed, 255);
  digitalWrite(Right_Motor_Direction, HIGH);
  analogWrite(Right_Motor_Speed, 255);
}


void traditionalTimerStart(){
  while(digitalRead(Right_Button)==1 && digitalRead(Left_Button)==1) // While loop for waiting start button push. (That push can be one of both button)
  {
  #ifdef DEBUG
    Serial.println(analogRead(Line_Sensor));
  #endif
  MotorStop();
  }
  // Depending on Button Push we give different Values to Last_Value Variable.
  // That Last_Value variables will be used later to turning the robot Left or Right
  if(digitalRead(Right_Button)==0)Last_Value=2;
  if(digitalRead(Left_Button)==0)Last_Value=0;

  //// 5 Seconde Delay Routine (500ms x 10 Times)
  for(int x=0;x<10;x++)
  {
    if(x%2==0){digitalWrite(Led1,HIGH); digitalWrite(Led2,LOW); }
    else{digitalWrite(Led1,LOW);digitalWrite(Led2,HIGH);}
    delay(500);
  }
}


//// Main Setup Function for Defining Input & Outputs ////
void setup() {
  pinMode(Left_Button, INPUT_PULLUP);
  pinMode(Right_Button, INPUT_PULLUP);
  pinMode(Left_Op_Sensor, INPUT_PULLUP);
  pinMode(Right_Op_Sensor, INPUT_PULLUP);
  pinMode(Left_Motor_Direction, OUTPUT);
  pinMode(Right_Motor_Direction, OUTPUT);
  pinMode(Left_Motor_Speed, OUTPUT);
  pinMode(Right_Motor_Speed, OUTPUT);
  #ifdef DEBUG
    Serial.begin(9600);
  #endif
  traditionalTimerStart();
}

//// Main Robot Code Routine ////

void loop() {
  Motor(Speed2,Speed2);

  while(digitalRead(Right_Button)==1 && digitalRead(Left_Button)==1){ // When no button is pushed.

  //// Line Sensor Control Function ////
  if(analogRead(Line_Sensor)<45) // Analog Reading from 45/1024 level.
  {
  delay(15); // Extra delay threshold for not seeing thin scratches!
  if(analogRead(Line_Sensor)<45) // Analog Reading from 45/1024 level.
  {
  Motor(-Speed2,-Speed2);
  delay(150); // 150mS Retreat
  Motor(-Speed2,Speed2);
  delay(200); // 200mS Turning
  Motor(Speed2,Speed2);
  }

  }
  ////////////////
  //// Opponent Sensor Control Functions, 3 sensor 3 If statement ////
  else if(digitalRead(Front_Op_Sensor)==1) // If Front sensor sees opponent
  {
  Motor(Speed2,Speed2); // Both Motors full speed forward.
  Last_Value=1;
  }
  else if(digitalRead(Left_Op_Sensor)==1&&digitalRead(Right_Op_Sensor)==0) // If Left sensor sees opponent
  {
  Motor(-Speed2,Speed2);
  Last_Value=0;
  }
  else if(digitalRead(Left_Op_Sensor)==0&&digitalRead(Right_Op_Sensor)==1) // If Right sensor sees opponent
  {
  Motor(Speed2,-Speed2);
  Last_Value=2;
  }

  //// If no data is received from any sensor, we finally look at the Last_Value variable.
  ////With this Last_Value variable, all we do is give a value to the Last_Value variable in that function,
  ///if the last data came from a sensor. We also enable it to remember the last sensor status with the following functions.

  else if(Last_Value==0)
  {
  Motor(-Speed2,Speed2);
  }
  else if(Last_Value==1)
  {
  Motor(Speed2,Speed2);
  }
  else if(Last_Value==2)
  {
  Motor(Speed2,-Speed2);
  }
  }
  // It will enter this part only when the while loop is broken. (When any button is pressed to stop the robot while it is running.)
  delay(100); // That delay is passed when again button is pressed main routine break from while. Used for debouncing button.
  while(digitalRead(Right_Button)==0 || digitalRead(Left_Button)==0);
  delay(100);
}