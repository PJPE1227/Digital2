void setup() {
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() > 0) {
    // Lee datos
    int receivedData = Serial.parseInt();
    
    // Datos recibidos
    Serial.print("Received data: ");
    Serial.println(receivedData);
  }
}
