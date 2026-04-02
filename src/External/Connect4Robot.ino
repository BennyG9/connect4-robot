#include <Encoder.h>
#include <Servo.h>
#include <string.h>


class Motor{
  private: 

    int pinA;
    int pinB;
    int pinPWM;

  public:

    //pinA - cw
    Motor(int A, int B, int PWM){
      pinA = A;
      pinB = B;
      pinPWM = PWM;

      pinMode(pinA, OUTPUT);
      pinMode(pinB, OUTPUT);
      pinMode(pinPWM, OUTPUT);
    }

    void setSpeed(int pwm){
      
      if(pwm == 0){
        stop();
        return;
      }

      if(pwm > 0){
        digitalWrite(pinA, HIGH);
        digitalWrite(pinB, LOW);
      }else{
        digitalWrite(pinB, HIGH);
        digitalWrite(pinA, LOW);
      }
      analogWrite(pinPWM, abs(pwm));
    }

    void stop(){
      digitalWrite(pinA, LOW);
      digitalWrite(pinB, LOW);
      digitalWrite(pinPWM, LOW);
    }

};

class Sensor{
  private: 
    int pin; 
    float threshold;
    float alpha = 0.5;
    float prevEMA = 0;

  public:
    Sensor(int analogPin, int thresh){
      pin = analogPin;
      threshold = thresh; 
    }

    int digRead(){
      return analogRead(pin) < threshold;
    }

    int anRead(){
      return analogRead(pin);
    }

    float EMARead(){
      int sensorReading = anRead();
      float ema = alpha * sensorReading + (1 - alpha) * prevEMA;
      prevEMA = ema;
      return ema;
    }

    int digEMARead(){
      return EMARead() < threshold;
    }

    void setThreshold(){
      float minReading = 256;
      for(int i = 0; i < 500; i++){
        float reading = EMARead();
        if(i >= 50){
          if(reading < minReading){
            minReading = reading;
          }
        }
      }
      // int minReading_int = (int)minReading;
      // if(minReading_int < minReading){
      //   threshold = minReading_int - 2;
      // }else{
      //   threshold = minReading_int - 3;
      // }
      threshold = minReading - 4.0;
      Serial.print("Threshold: ");
      Serial.println(threshold);
    }
};

//encoder vars
int encY = 2;
int encB = 3;
Encoder myEnc(encY, encB);
long encCount = 0;

//PID vars
int dt_min = 30000UL;
double Kp = 5.0;
double Ki = 0.0;
double Kd = 0.5;
int targetCount = 0;
int err = 0;
int pos_tol = 5;
double v_tol = 0.0; 
long unsigned lastTick = 0;
double integral;

//servo vars
int cartServoPin = 7;
Servo cartServo;

//DC motor vars
int mtrA = 4;
int mtrB = 6;
int mtrPWM = 5;
Motor cartMtr(mtrA, mtrB, mtrPWM);

//UART communications variables 
const byte START = 0xAA;
enum Command {
  STARTUP,
  MOVE_TRAY,
  DROP_TRAY,
  REGISTER_MOVE,
  CAL_SENSORS,
  
};

//Sensors 
Sensor sensorList[7] = {Sensor(A0,26), Sensor(A1,30), Sensor(A2,27), Sensor(A3,26), Sensor(A4,30), Sensor(A5,30), Sensor(A6,25)};

//control state
enum State {
  IDLE,
  MOVE_CART,
  DROP_PIECE,
  USER_MOVE,
  COMPUTER_MOVE,
};
State currentState = IDLE;

void sendPacket(int cmd, int arg){
  byte checksum = (cmd+arg) % 256;
  Serial.write((byte)START);
  Serial.write((byte)cmd);
  Serial.write((byte)arg);
  Serial.write((byte)checksum);
  Serial.println();
}


void setup() {
  Serial.begin(9600);
  Serial.println("STARTED");
  cartServo.attach(cartServoPin);
  cartServo.write(10);

  delay(1);
  for(int s = 0; s < 7; s++){
    sensorList[s].setThreshold();
  }

  currentState = IDLE;
}


void loop() {
  // put your main code here, to run repeatedly:
  
  // noInterrupts();
  // long newCount = myEnc.read();
  // interrupts();
  // if(newCount != encCount){
  //   if(newCount - encCount > 0){
  //     //Serial.println("-1");
  //   }
  //   encCount = newCount;
  //   double pos = encCount * 1.25 / 4.0;
  //   //Serial.println(encCount);
  // }
  //Serial.println(analogRead(A6));
  // if(digitalRead(8)){
  //   Serial.println(1);
  // }
  //Serial.println(digitalRead(8));
  // Serial.print(digitalRead(8));
  // Serial.print("  ");
  // Serial.println(digitalRead(9));

  //Serial.println("loop");
  // for(int s = 0; s < 7; s++){
  //  //Serial.print(sensorList[s].EMARead());
  //  //Serial.print("\t");
  //  if(sensorList[s].digEMARead()){
  //   Serial.println(s);
  //   delay(500);
  //  }
  // }
  //Serial.println("\n");

  switch(currentState){
    case IDLE:

      // check for incoming packets over serial
      //Serial.println(Serial.available());
      if(Serial.available()){
        //immediately read first byte
        int b = Serial.read();
        //Serial.println(b);
        

        if(b != START){
          Serial.println("ERROR: INVALID PACKET START");
          break; 
        }

        while(Serial.available() < 3){}

        int cmd = Serial.read();
        int arg = Serial.read();
        int checksum = Serial.read();

        if(checksum != cmd + arg){
          Serial.println("WARNING: PACKET CHECKSUM FAILED");
        }

        switch(cmd){
          case STARTUP:
            Serial.println("STARTUP");
            break;

          case MOVE_TRAY:
            Serial.println("MOVE_TRAY");
            if(arg < 0 || arg > 6){
              Serial.println("ERROR: COLUMN OUT OF BOUNDS");
              break;
            }
            targetCount = -1 * ((136+2)/2) * arg;
            err = targetCount - myEnc.read();
            currentState = MOVE_CART;
            break;

          case DROP_TRAY:
            Serial.println("DROP_TRAY");
            currentState = DROP_PIECE;
            break;

          case REGISTER_MOVE:
            Serial.println("WARNING: REGISTER_MOVE COMMAND NOT EXECUTABLE ON ARDUINO");
            break;

          case CAL_SENSORS:
            for(int i = 0; i < 7; i++){
              sensorList[i].setThreshold();
            }
            break;

          default: 
            Serial.println("ERROR: COMMAND DOES NOT EXIST");
            break;
        }

      }

      // check sensors 
      for(int s = 0; s < 7; s++){
        int sensorReading = sensorList[s].digEMARead();
        
        if(sensorReading){
          //Serial.println(sensorList[s].EMARead());
          sendPacket(REGISTER_MOVE, s);
          delay(500);
        }
      }

      break; 

    case DROP_PIECE:
      //Serial.println("DROPPING PIECE");
      cartServo.write(110);
      delay(750);
      cartServo.write(10);
      currentState = IDLE;
      break;

    case USER_MOVE:
      break;

    case COMPUTER_MOVE:
      break;

    case MOVE_CART:
      //Serial.println("MOVE CART");
      unsigned long now = micros();
      if(now - lastTick >= dt_min){

        noInterrupts();
        long newCount = myEnc.read();
        interrupts(); 

        int newErr = targetCount - newCount;
        double dt = (now - lastTick) / 1e6;

        integral += newErr * dt;
        double derivative = (double)(newErr - err) / dt;
        err = newErr;

        int pwm = (int)(Kp*err + Ki*integral + Kd*derivative);        

        if(pwm < -255){
          pwm = -255;
        }else if(pwm > 255){
          pwm = 255;
        }else if(pwm != 0 && derivative == 0.0){
          int s = pwm / abs(pwm);
          pwm = s * 120;
        }
        // else if(abs(pwm) < 150){
        //   //int s = pwm / abs(pwm);
        //   //pwm = s*100;
        // }

        cartMtr.setSpeed(pwm);

        if(abs(err) <= pos_tol && abs(derivative) <= v_tol){
          integral = 0.0;

          cartMtr.stop();

          //currentState = DROP_PIECE;
          currentState = IDLE;
          //Serial.print("TAR: ");
          //Serial.println(targetCount);
          //Serial.print("POS: ");
          //Serial.println(myEnc.read());
          //Serial.println("PID DONE");
        }
      }
      break;

    default:
      Serial.println("INVALID STATE");
      break;
  }

}
