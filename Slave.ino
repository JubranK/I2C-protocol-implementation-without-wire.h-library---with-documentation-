// Jubran Khoury

#define SDA_PIN  A4
#define SCL_PIN  A5

void setup() {
  Serial.begin(9600);
  pinMode(SDA_PIN, INPUT);
  pinMode(SCL_PIN, INPUT);
}

void loop() {
  while (digitalRead(SDA_PIN) == HIGH || digitalRead(SCL_PIN) == HIGH);

  // Read address and direction bit
  byte address = Read();
  byte direction = address & 1;
  address >>= 1;

  if (address == 0x04) {
    if (direction == 0) {
      
      char data = Read();
      Serial.print("Slave: Data received: ");
      Serial.println(data);
    } else {
      
      Acknowledge();
      Write('k'); // Send character 'k' back to master
      Serial.println("Slave: 'k' sent.");
    }
  }
}

byte Read() {
  byte data = 0;
  pinMode(SDA_PIN, INPUT);
  for (byte i = 0; i < 8; i++) {
    while (digitalRead(SCL_PIN) == LOW);
    data <<= 1;
    if (digitalRead(SDA_PIN)) {
      data |= 1;
    }
    while (digitalRead(SCL_PIN) == HIGH);
  }
  return data;
}

void Acknowledge() {
  pinMode(SDA_PIN, OUTPUT);
  digitalWrite(SDA_PIN, LOW);
  delayMicroseconds(4);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(4);
  digitalWrite(SCL_PIN, LOW);
  pinMode(SDA_PIN, INPUT);
}

void Write(byte data) {
  for (byte i = 0; i < 8; i++) {
    digitalWrite(SDA_PIN, (data & 0x80) != 0);
    data <<= 1;
    digitalWrite(SCL_PIN, HIGH);
    delayMicroseconds(4);
    digitalWrite(SCL_PIN, LOW);
    delayMicroseconds(4);
  }
  
  // Acknowledge bit
  pinMode(SDA_PIN, INPUT);
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(4);
  digitalWrite(SCL_PIN, LOW);
  pinMode(SDA_PIN, OUTPUT);
}
