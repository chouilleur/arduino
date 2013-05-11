#define MOTOR_LEFT		0
#define MOTOR_RIGHT		1
#define PIN_MOTOR_LEFT		5
#define PIN_MOTOR_RIGHT		6
#define MAX_SPEED		255
#define MIN_SPEED		40
#define ACCELERATION_DELAY	10
#define CMD_LEFT_START		'a'
#define CMD_RIGHT_START		'b'
#define CMD_LEFT_STOP		'c'
#define CMD_RIGHT_STOP		'd'
#define CMD_STOP		'e'

#define ENABLE_A  5
#define ENABLE_B  6
#define INA_1     10
#define INA_2     11
#define INB_1     12
#define INB_2     13


//--------------------------------------------------------
// TYPE DEFINITION
//--------------------------------------------------------
struct Direction
{
  int ina_1;
  int ina_2;
  int inb_1;
  int inb_2;
};

struct DirectionMap
{
  char      key;
  Direction dir;
};
//-------------------------------------
// Motor control

typedef struct {
  bool	inAcceleration;
  int	speed;
  int	pin;
} t_motor;

t_motor motor[] = {
  { false, 0, PIN_MOTOR_LEFT  },
  { false, 0, PIN_MOTOR_RIGHT }
};

void run() {
  acceleration(MOTOR_LEFT);
  acceleration(MOTOR_RIGHT);
}

void startMotor(int motorNum) {
  if (motorNum == MOTOR_LEFT || motorNum == MOTOR_RIGHT) {
    motor[motorNum].inAcceleration = true;
    motor[motorNum].speed = MIN_SPEED;
  }
}

void acceleration(int motorNum) {
  if (motorNum != MOTOR_LEFT && motorNum != MOTOR_RIGHT)
    return;

  for (int i = 0; i < 2; ++i) {
    if (motor[i].inAcceleration) {
      motor[i].speed++;
      analogWrite(motor[motorNum].pin, motor[motorNum].speed);
      if (motor[i].speed >= MAX_SPEED)
	motor[i].inAcceleration = false;
    }
  }

  delay(ACCELERATION_DELAY);
}

//-------------------------------------
//
void setup() {
  Serial.begin(9600);
  pinMode(PIN_MOTOR_LEFT, OUTPUT);
  pinMode(PIN_MOTOR_RIGHT, OUTPUT);
}

void loop() {
  int buffer;

  if (Serial.available() > 0) {
    buffer = Serial.read();

    switch(buffer) {
    case CMD_LEFT_START:
      startMotor(MOTOR_LEFT);
      break;
    case CMD_RIGHT_START:
      startMotor(MOTOR_RIGHT);
      break;
    case CMD_LEFT_STOP:
      analogWrite(PIN_MOTOR_LEFT, 0);
      break;
    case CMD_RIGHT_STOP:
      analogWrite(PIN_MOTOR_RIGHT, 0);
      break;
    case CMD_STOP:
      analogWrite(PIN_MOTOR_LEFT, 0);
      analogWrite(PIN_MOTOR_RIGHT, 0);
      break;
    }
  }
  run();
}
