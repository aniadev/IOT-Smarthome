 
//============================================================= Serial Software define
  #include <SoftwareSerial.h>
  SoftwareSerial swSerial(D3, D2); // TX RX
//============================================================= MF58 Constant
#define R_25     100.0 //Ki Ohms
#define Beta    3950.0
#define T25     298.15 // Kelvin
#define alpha   0.0033540164346805  // 1/T25
// T = 1/ (log(R/R25)/Beta + 1/T25 )

//============================================================= Button Variables
int Q1 = 1, Q1p = 1, Q2 = 1, Q2p = 1, Q3 = 1, Q3p = 1 , Q4 = 1, Q4p = 1;
int state1, state2, state3, state4; // 0: noclick; 1: falling; 2: holding; 3: rising

//============================================================= Pinout Controller
#define livingRoomBtn        A0
#define bedRoomBtn           A1
#define bathRoomBtn          A2
#define hallwayBtn           A3

#define livingRoomLight     D4
#define bedRoomLight        D7
#define bathRoomLight       D6
#define hallwayLight        D5
#define gardenLight         D8

#define lightSensor         A6
#define tempSensor          A5

uint8_t livingRoomLight_status  = 0;
uint8_t bedRoomLight_status     = 0;
uint8_t bathRoomLight_status    = 0;
uint8_t hallwayLight_status     = 0;
uint8_t gardenLight_status      = 0;
uint8_t gardenLight_status_prv      = 0;

int lightValue;

//============================================================= Timer variable
unsigned long timer1;



void swSerialTransfer(String code, String value) {
    swSerial.print(code);
    swSerial.print(":");
    swSerial.print(value);
    swSerial.print("#");
}
void swSerialTransfer(String code, double value) {
    swSerial.print(code);
    swSerial.print(":");
    swSerial.print(value);
    swSerial.print("#");
}
void setup() {
  // Serial initializing:
  Serial.begin(115200);
  swSerial.begin(9600);
  // Pinmode for butotn
  pinMode(livingRoomBtn,  INPUT_PULLUP);
  pinMode(bedRoomBtn,     INPUT_PULLUP);
  pinMode(bathRoomBtn,    INPUT_PULLUP);
  pinMode(hallwayBtn,     INPUT_PULLUP);
  // Pinmode for led
  pinMode(livingRoomLight,  OUTPUT);
  pinMode(bedRoomLight,     OUTPUT);
  pinMode(bathRoomLight,    OUTPUT);
  pinMode(hallwayLight,     OUTPUT);
  pinMode(gardenLight,      OUTPUT);
  // Set external for analog REF
  analogReference(EXTERNAL);
  swSerialTransfer("R1", "OFF");
  swSerialTransfer("R2", "OFF");
  swSerialTransfer("R3", "OFF");
  swSerialTransfer("G1", "OFF");
  swSerialTransfer("H1", "OFF");
}
void loop() {
  // Button Read
                                                    // Read button 1
  Q1 = digitalRead(livingRoomBtn);
  if ((Q1==1)&&(Q1p==1))       state1 = 0; // no click
  else if ((Q1==0)&&(Q1p==1)) {state1 = 1;} // falling
  else if ((Q1==0)&&(Q1p==0)) {state1 = 2;} // holding
  else if ((Q1==1)&&(Q1p==0)) {state1 = 3;} // rising 
  Q1p = Q1;
  //-------------------------------------------------------------------------------
                                                    // Read button 2
  Q2 = digitalRead(bedRoomBtn);
  if ((Q2==1)&&(Q2p==1))       state2 = 0; // no click
  else if ((Q2==0)&&(Q2p==1)) {state2 = 1;} // falling
  else if ((Q2==0)&&(Q2p==0)) {state2 = 2;} // holding
  else if ((Q2==1)&&(Q2p==0)) {state2 = 3;} // rising 
  Q2p = Q2;
  //-------------------------------------------------------------------------------
                                                    // Read button 3
  Q3 = digitalRead(bathRoomBtn);
  if ((Q3==1)&&(Q3p==1))       state3 = 0; // no click
  else if ((Q3==0)&&(Q3p==1)) {state3 = 1;} // falling
  else if ((Q3==0)&&(Q3p==0)) {state3 = 2;} // holding
  else if ((Q3==1)&&(Q3p==0)) {state3 = 3;} // rising 
  Q3p = Q3;
  //-------------------------------------------------------------------------------
                                                    // Read button 4
  Q4 = digitalRead(hallwayBtn);
  if ((Q4==1)&&(Q4p==1))       state4 = 0; // no click
  else if ((Q4==0)&&(Q4p==1)) {state4 = 1;} // falling
  else if ((Q4==0)&&(Q4p==0)) {state4 = 2;} // holding
  else if ((Q4==1)&&(Q4p==0)) {state4 = 3;} // rising 
  Q4p = Q4;
 //-------------------------------------------------------------------------------
//================================================================== transfer led data
  if (state1 == 1) {
    livingRoomLight_status = !livingRoomLight_status;
    swSerialTransfer("R1", livingRoomLight_status ? "ON" : "OFF");
  }
  if (state2 == 1) {
    bedRoomLight_status = !bedRoomLight_status;
    swSerialTransfer("R2", bedRoomLight_status ? "ON" : "OFF");
  }
  if (state3 == 1) {
    bathRoomLight_status = !bathRoomLight_status;
    swSerialTransfer("R3", bathRoomLight_status ? "ON" : "OFF");
  }
  if (state4 == 1) {
    hallwayLight_status = !hallwayLight_status;
    swSerialTransfer("H1", hallwayLight_status ? "ON" : "OFF");
  }

//=================================================================debug
//  Serial.print("Q1: "); Serial.print(state1);
//  Serial.print(", Q2: "); Serial.print(state2);
//  Serial.print(", Q3: "); Serial.print(state3);
//  Serial.print(", Q4: "); Serial.print(state4);
//  Serial.print(", A6: "); Serial.print(analogRead(lightSensor));
//  Serial.print(", A5: "); Serial.print(analogRead(A5));
//  Serial.println();
//============================================================ OUTPUT digital control led
  digitalWrite(livingRoomLight, livingRoomLight_status);
  digitalWrite(bedRoomLight,    bedRoomLight_status);
  digitalWrite(bathRoomLight,   bathRoomLight_status);
  digitalWrite(hallwayLight,    hallwayLight_status);
  digitalWrite(gardenLight,     gardenLight_status);

//============================================================  Read LDR sensor
  lightValue = analogRead(lightSensor);
  if (lightValue <= 150) {
    gardenLight_status_prv = gardenLight_status;
    gardenLight_status = 1;
  }
  if (lightValue >= 160) {
    gardenLight_status_prv = gardenLight_status;
    gardenLight_status = 0;
  }
//  Serial.println(lightValue);
  if (gardenLight_status_prv != gardenLight_status) {
    Serial.print(lightValue);
    Serial.println(": garden light - changed");
    swSerialTransfer("L1", lightValue);
    swSerialTransfer("G1", gardenLight_status ? "ON" : "OFF");
  }
//====================================================== Calculate Temp, light, and transfer
  if ((unsigned long)(millis() - timer1 > 10000)) {
    timer1 = millis();
    unsigned int   tempValue = analogRead(tempSensor);
    double   R1 = 108.4 * (4064.0/(double)tempValue - 1);
    double   T1 = 1 / (log(R1 / R_25)/Beta + alpha) - 273.0;
    swSerialTransfer("T1", T1);
    swSerialTransfer("L1", lightValue);
  }
//  ===================================================== Read data from NodeMCU to change data
  while (swSerial.available()) {
    String code = swSerial.readStringUntil(':');
    String value = swSerial.readStringUntil('#');
    Serial.print(code);
    Serial.print("/");
    Serial.print(value);
    Serial.println();
    if (code == "R1"){
      if (value == "ON") livingRoomLight_status = 1;
      if (value == "OFF") livingRoomLight_status = 0;
    }
    if (code == "R2"){
      if (value == "ON") bedRoomLight_status = 1;
      if (value == "OFF") bedRoomLight_status = 0;
    }
    if (code == "R3"){
      if (value == "ON") bathRoomLight_status = 1;
      if (value == "OFF") bathRoomLight_status = 0;
    }
    if (code == "H1"){
      if (value == "ON") hallwayLight_status = 1;
      if (value == "OFF") hallwayLight_status = 0;
    }
//    Serial.print("R1: "); Serial.print(livingRoomLight_status);
//    Serial.print(", R2: "); Serial.print(bedRoomLight_status);
//    Serial.print(", R3: "); Serial.print(bathRoomLight_status);
//    Serial.print(", H1: "); Serial.print(hallwayLight_status);
//    Serial.println();
  }
}
