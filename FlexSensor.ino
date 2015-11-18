const int DELAY = 100;
const int DELAY_KEEP_LIGHT_ON = 1500;
const int TESTS = 200;
const int ITERATIONS = 50;

//Pins for flex sensors
const int PIN_RIGHT_IN = A0;
const int PIN_RIGHT_OUT = A1;
const int PIN_LEFT_IN = A2;
const int PIN_LEFT_OUT = A3;

//Pins for EL tape
const int PIN_RIGHT_TURN_1 = 2;
const int PIN_RIGHT_TURN_2 = 3;
const int PIN_RIGHT_TURN_3 = 4;
const int PIN_LEFT_TURN_1 = 5;
const int PIN_LEFT_TURN_2 = 6;
const int PIN_LEFT_TURN_3 = 7;

//which sensor to write to serial 1-RightIN 2-RightOut 3-LeftIn 4-LeftOut
const int SERIAL_WRITE = 3;

//Bounds for turning
const int SEN_RIGHT_IN = 480;//bounds right arm inside
const int SEN_RIGHT_OUT = 470;//bounds right arm outside
const int SEN_LEFT_IN = 445; //480;//bounds left arm inside
const int SEN_LEFT_OUT = 445; //470;//bounds left arm outside

//Bounds for stopping
const int SEN_STOP_OUT = 480;
const int SEN_STOP_IN_LOW = 480;
const int SEN_STOP_IN_HIGH = 530;

int isLeft; // # of iterations user maintains leftturn gesture
int notLeft; // 
int isRight; // # of iterations user maintains rightturn gesture
int notRight; // 


void setup() {
  // put your setup code here, to run once
  Serial.begin(9600);
  pinMode(PIN_RIGHT_TURN_1, OUTPUT);
  pinMode(PIN_RIGHT_TURN_2, OUTPUT);
  pinMode(PIN_RIGHT_TURN_3, OUTPUT);
  pinMode(PIN_LEFT_TURN_1, OUTPUT);
  pinMode(PIN_LEFT_TURN_2, OUTPUT);
  pinMode(PIN_LEFT_TURN_3, OUTPUT);
}

void leftSignalLight(int state)
{
  if(state) {
    digitalWrite(2, HIGH);
    Serial.println("ON");
  }
  else {
    Serial.println("OFF");
    digitalWrite(2, LOW);
  } 
}

void rightSignalLight(int state)
{
  if(state)
    digitalWrite(4, HIGH);
  else
    digitalWrite(4, LOW);
}

void stopSignalLight(int state)
{
  if(state)
    digitalWrite(3, HIGH);
  else
    digitalWrite(3, LOW);
}

void testLeft(int sensLeftIn, int sensLeftOut)
{
//  Serial.println(isLeft);
//  Serial.println(notLeft);
  if(sensLeftIn < SEN_LEFT_IN && sensLeftOut > SEN_LEFT_OUT) {
    isLeft++;
    notLeft = 0;
  }
  else {
    notLeft++;
    isLeft = 0;
  }

  if (isLeft > ITERATIONS)
    leftSignalLight(1);
  if (notLeft > ITERATIONS)
    leftSignalLight(0);    
}

void testRight(int sensRightIn, int sensRightOut)
{
  //Right turn light on
  if(sensRightIn < SEN_RIGHT_IN && sensRightOut > SEN_RIGHT_OUT)
    rightSignalLight(1);
  else
    rightSignalLight(0);

}

void testStop(int sensLeftIn, int sensLeftOut)
{
  if (sensLeftOut > SEN_STOP_OUT && sensLeftIn < SEN_STOP_IN_HIGH && sensLeftIn > SEN_STOP_IN_LOW)
    stopSignalLight(1);
  else
    stopSignalLight(0);
}

void serialWrite(int select, int sensRightIn, int sensRightOut, int sensLeftIn, int sensLeftOut)
{
  switch(select) {
    case 1:
      Serial.println(sensRightIn);
      break;
    case 2:
      Serial.println(sensRightOut);
      break;
    case 3:
      Serial.println(sensLeftIn);
      break;
    case 4:
      Serial.println(sensLeftOut);
      break;
     default:
      Serial.println("What??");
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  int sensRightIn;
  int sensRightOut;
  int sensLeftIn;
  int sensLeftOut;

  digitalWrite(5, HIGH);

  
  while(1)
  {
    sensRightIn = analogRead(PIN_RIGHT_IN);
    sensRightOut = analogRead(PIN_RIGHT_OUT);
    sensLeftIn = analogRead(PIN_LEFT_IN);
    sensLeftOut = analogRead(PIN_LEFT_OUT);

    serialWrite(SERIAL_WRITE, sensRightIn, sensRightOut, sensLeftIn, sensLeftOut);
    delay(DELAY);

    //testRight(sensRightIn, sensRightOut);
    testLeft(sensLeftIn, sensLeftOut);
    //testStop(sensLeftIn, sensLeftOut);
  }
}
