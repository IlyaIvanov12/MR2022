#include <Servo.h>
//#include <Ultrasonik.h>


#define ECHOPIN 19  // приемник
#define TRIGPIN 18  // источник

#define BUTTONPIN 8  // кнопка включения и отключения
#define NOTEPIN 10  // оповещение о приближении

#define SPEED_R 5  // скорость правых моторов
#define SPEED_L 6  // скорость левых моторов
#define DIR_R 4  // направление правых моторов
#define DIR_L 7  // направление левых моторов

enum MODE {move_towards, detour_object};


int current_mode = move_towards;
bool is_on = false;  // для кнопки включения
bool button_state = false;
float time = 0.0;
int sound_time = 0;  // время оповещения  
float filt_param = 0.2;  // параметр для фильтрации
float dist_filt = 0;  // отфильтрованный выход
float dur_param = 29.1;


void setup() {
  Serial.begin(9600);
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(BUTTONPIN, INPUT);

  analogWrite(NOTEPIN, 255);
  delay(300);
  analogWrite(NOTEPIN, 0);
}


void set_mode (int new_mode) {
  switch(new_mode) {
    case move_towards:
      move();
      break;

    case detour_object:
      detour();
      break;

    default:
      stop();
      break;

    }
    current_mode = new_mode;
    Serial.print(new_mode);
}


void move() {
  int value_right = 1;
  int value_left = 1;

  start_motors(value_right, value_left);
}


void detour() {
  int value_right = 1;
  int value_left = 1;

  start_motors(value_right, value_left);
}


void stop() {
  int value_right = 0;
  int value_left = 0;
  start_motors(value_right, value_left);
}


void start_motors (int value_right, int value_left) {
  int direction_left;
  int direction_right;
  if (value_right > 0) {
    direction_right = 1;
  }
  else {
    direction_right = 0;
  }

  if (value_left > 0) {
    direction_left = 1;
  }
  else {
    direction_left = 0;
  }
  digitalWrite(DIR_R, direction_right);
  digitalWrite(DIR_L, direction_left);
  analogWrite(SPEED_R, value_right);
  analogWrite(SPEED_L, value_left);

}


void program() {
  int mode = 0;
  mode = find_object;
  set_mode(mode);
  delay(300);
}


void find_object() {

}


int get_distance() {
  long dur, sm;
  digitalWrite(TRIGPIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGPIN, LOW);
  dur = pulseIn(ECHOPIN, HIGH);
  sm = (dur / 2) / dur_param;  // перевод показаний датчика в сантиметры
  Serial.print("Distance is ");
  Serial.print(sm);
  Serial.print(" sm");
  delay(100);
  sm = map(sm, 0, 350, 0, 255);
  //sound_time = map(sm, 0, 255, 700, 100);
  return (sm);
}


int dist_filtered() {
  int dist = get_distance();
  dist_filt += (dist - dist_filt) * filt_param;
  return (dist_filt);
}


int peep_time() {
  int sound = dist_filtered();
  int sound_time = map(sound, 0, 255, 700, 100);
  analogWrite(NOTEPIN, sound_time);
  return (sound_time);
}


void sound() {
  int time_s = peep_time();
  analogWrite(NOTEPIN, 200);
  delay(time_s);
  analogWrite(NOTEPIN, 0);
  delay(time_s);
}


bool check_button() {
  if (digitalRead (BUTTONPIN) == HIGH) {
    button_state = not button_state;
    current_mode = 0;
    delay(500);
  }
  return button_state;
}


void loop() {
  bool is_on = check_button();

  if (is_on) {
    program();
  }
  
  else {
    digitalWrite(SPEED_R, LOW);
    digitalWrite(SPEED_L, LOW);
    digitalWrite(DIR_R, LOW);
    digitalWrite(DIR_L, LOW);
    }
}