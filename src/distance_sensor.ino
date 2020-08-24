int Trigger = 2;   //Pin digital 2 para el Trigger del sensor
int Echo = 3;   //Pin digital 3 para el Echo del sensor
int cc = 0xB0;

int controlChange = 0x0B;


void setup() {
  //Serial.begin(9600);//iniciailzamos la comunicación
  Serial.begin(31250);
  pinMode(Trigger, OUTPUT); //pin como salida
  pinMode(Echo, INPUT);  //pin como entrada
  digitalWrite(Trigger, LOW);//Inicializamos el pin con 0
}

void loop()
{
  readDistance(Trigger, Echo, cc);

}


void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

void CCOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

void sendeMIDI(int statusByte, int dataByte1, int dataByte2) {
Serial.write(statusByte);
Serial.write(dataByte1);
Serial.write(dataByte2);
}

void readDistance(int Trigger, int Echo, int ctrlNummer) {
  //Trigger Measurement
  digitalWrite(Trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger, LOW);


  long sensorWert = pulseIn(Echo, HIGH);

  if (sensorWert <=2500) {
    int sensorMIDI = map(sensorWert,2500,0,0,127);
    sendeMIDI(controlChange, ctrlNummer, sensorMIDI);
  }

}
