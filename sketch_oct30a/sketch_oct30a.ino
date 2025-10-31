// Arduino: envía lecturas simuladas de sensores por Serial
// Baudrate: 115200 (debe coincidir con tu programa C++)
// Formato: 
//   T;T-001;25.6
//   P;P-105;82

unsigned long lastSend = 0;
int presionFake = 80;
float tempFake = 25.0;

void setup() {
  Serial.begin(115200);
  // Espera a que se abra el puerto (en algunos Arduinos no es necesario, pero no estorba)
  while (!Serial) { ; }
}

void loop() {
  unsigned long now = millis();
  if (now - lastSend >= 2000) { // cada 2 segundos manda algo
    lastSend = now;

    // Alterna entre temperatura y presión
    static bool sendTemp = true;
    if (sendTemp) {
      tempFake += 0.3; // variar un poco
      Serial.print("T;T-001;");
      Serial.println(tempFake, 1); // 1 decimal
    } else {
      presionFake += 1;
      Serial.print("P;P-105;");
      Serial.println(presionFake);
    }
    sendTemp = !sendTemp;
  }
}
