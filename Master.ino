// Jubran Khoury 

#define SDA_PIN  A4
#define SCL_PIN  A5

// Slave address 
#define SLAVE_ADDRESS 0x04

void setup() {
  Serial.begin(9600);
  pinMode(SDA_PIN, OUTPUT);
  pinMode(SCL_PIN, OUTPUT);
  
  // At first both are high 
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, HIGH);
}

void loop() {
  Start();
  Write(SLAVE_ADDRESS << 1);
  Write('j'); // Send char 'j'
  Stop(); // stop 
  Serial.println("Master: 'j' sent.");  // now... wait for ACK from the slave
  
  delay(2000);
  
  Start();
  Write((SLAVE_ADDRESS << 1) | 1); // Send slave address with read bit
  char data = Read();
  Stop();
  Serial.print("Master received: ");
  Serial.println(data);
  
  delay(5000);
}

// Start condition
void Start() {
  digitalWrite(SDA_PIN, LOW);
  delayMicroseconds(5);
  digitalWrite(SCL_PIN, LOW);
}

// Stop condition 
void Stop() {
  digitalWrite(SDA_PIN, LOW);
  delayMicroseconds(4);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(4);
  digitalWrite(SDA_PIN, HIGH);
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
  
  // Ack bit
  pinMode(SDA_PIN, INPUT);
  digitalWrite(SDA_PIN, HIGH);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(4);
  digitalWrite(SCL_PIN, LOW);
  pinMode(SDA_PIN, OUTPUT);
}

char Read() {
  char data = 0;
  pinMode(SDA_PIN, INPUT);
  for (byte i = 0; i < 8; i++) {
    digitalWrite(SCL_PIN, HIGH);
    delayMicroseconds(4);
    data <<= 1;
    if (digitalRead(SDA_PIN)) {
      data |= 1;
    }
    digitalWrite(SCL_PIN, LOW);
    delayMicroseconds(4);
  }
  
  // Send ACK
  pinMode(SDA_PIN, OUTPUT);
  delayMicroseconds(4);
  digitalWrite(SDA_PIN, LOW);
  digitalWrite(SCL_PIN, HIGH);
  delayMicroseconds(4);
  digitalWrite(SCL_PIN, LOW);
  pinMode(SDA_PIN, INPUT);
  
  return data;
}
