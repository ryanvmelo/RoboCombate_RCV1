// Arduino RC Car using Flysky Transmitter and Receiver
// Subscribe the Channel: https://www.YouTube.com/@AKElectroDIY
// Video Link: https://youtu.be/q-Clw0m3E18

int enA = 5;
int in1 = 2;
int in2 = 3;
int enB = 6;
int in3 = 4;
int in4 = 7;

int receiver_pins[] = {A0, A1, A2, A3, A4, A5};
int receiver_values[] = {0, 0, 0, 0, 0, 0};
int res_min = 950;
int res_max = 2020;

int working_range = 255; // motor driver range

boolean prt = true;

int mode = 0;
//-1 = transmitter not connected or out of range
// 0 = transmitter connected and ready
// 1 = slow speed mode
// 2 = high speed mode

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(115200);
}

void loop() {
  receive();

  int m1 = 0;
  int m2 = 0;

  int rot = receiver_values[0];

  if (mode == 1) {
    m1 = receiver_values[1] / 2 + (rot)/1.5;
    m2 = receiver_values[1] / 2 - (rot)/1.5;

  } else if (mode == 2) {

    m1 = receiver_values[1] + rot / 1.75;
    m2 = receiver_values[1] - rot / 1.75;
  }

  mpower(1,  m1);
  mpower(2,  m2);
}

int rp = 0;

void receive() {
  receiver_values[rp] = map(pulseIn (receiver_pins[rp], HIGH), res_min, res_max, -1 * working_range, working_range);
  rp++;
  if (rp == 6){
    rp = 0;
  }
  boolean activevalues = true;
  for (int i = 0; i < 6; i++) {
    if (prt) {
      Serial.print("CH");
      Serial.print(i);
      Serial.print(" : ");
      Serial.print(receiver_values[i]);
      Serial.print(",\t");
    }
    if (receiver_values[i] < -500) {
      activevalues = false;
    }
  }
  mode = 0;
  if (!activevalues) {
    mode = -1;
  } else if (receiver_values[4] > -100) {
    mode = 2;
  } else if (receiver_values[5] > -100) {
    mode = 1;
  }
  if (prt) {
    Serial.println("");
  }
}

void mpower(int motor,  int spd) {
  int rotation = 0;
  if (spd > 0) {
    rotation = 1;
  } else if (spd < 0) {
    rotation = -1;
    spd *= -1;
  }
  if (spd > 255) {
    spd = 255;
  }
  int pwm;
  int pA;
  int pB;
  if (motor == 1) {
    pwm = enA;
    pA = in1;
    pB = in2;
  } else if (motor == 2) {
    pwm = enB;
    pA = in3;
    pB = in4;
  } else {
    return;
  }
  if (rotation == 0) {
    digitalWrite(pA, LOW);
    digitalWrite(pB, LOW);
  } else if (rotation == 1) {
    digitalWrite(pA, HIGH);
    digitalWrite(pB, LOW);
  } else if (rotation == -1) {
    digitalWrite(pA, LOW);
    digitalWrite(pB, HIGH);
  }
  analogWrite(pwm, spd);
}