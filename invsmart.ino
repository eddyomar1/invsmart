#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd1(0x27, 16, 2);  // LCD 1, Dirección I2C 0x27, 16 columnas y 2 filas
LiquidCrystal_I2C lcd2(0x25, 16, 2);  // LCD 2, Dirección I2C 0x28, 16 columnas y 2 filas

const int panelSolarPin = 34;   // Pin analógico para el sensor de paneles solares
const int bateriaPin = 35;      // Pin analógico para el sensor de baterías
const int EDEPin = 32;          // Pin analógico para el sensor de EDE
const int carga1Pin = 33;       // Pin analógico para el sensor de carga 1
const int carga2Pin = 36;       // Pin analógico para el sensor de carga 2
const int carga3Pin = 39;       // Pin analógico para el sensor de carga 3
const int carga4Pin = 25;       // Pin analógico para el sensor de carga 4
const int carga5Pin = 26;       // Pin analógico para el sensor de carga 5
const int activacionBateriasPin = 27; // Pin para el relé de activación de baterías
const int activacionEDEPin = 14; // Pin para el relé de activación de EDE

const float voltaje = 120.0;  // Voltaje de la red eléctrica en V

void setup() {
   
  Wire.begin();
    
  lcd1.begin(16, 2);  // Inicializar LCD 1  
// lcd1.init();

  lcd2.begin(16, 2);  // Inicializar LCD 2

lcd1.backlight();  
lcd2.backlight();

  lcd1.print("PS:0W   Bat:0W");
  lcd1.setCursor(0, 1);
  lcd1.print("EDE:0W");

  lcd2.print("C1: 0W C2: 0W C3: 0W");
  lcd2.setCursor(0, 1);
  lcd2.print("C4: 0W C5: 0W");

  pinMode(activacionBateriasPin, OUTPUT);
  pinMode(activacionEDEPin, OUTPUT);
  digitalWrite(activacionBateriasPin, LOW); // Inicialmente, el relé está apagado
  digitalWrite(activacionEDEPin, LOW);
}

void loop() {
  // Leer valores de los sensores de corriente
  float corrienteSolar = analogRead(panelSolarPin) * (voltaje / 4095.0);
  float corrienteBateria = analogRead(bateriaPin) * (voltaje / 4095.0);
  float corrienteEDE = analogRead(EDEPin) * (voltaje / 4095.0);
  float corrienteCarga1 = analogRead(carga1Pin) * (voltaje / 4095.0);
  float corrienteCarga2 = analogRead(carga2Pin) * (voltaje / 4095.0);
  float corrienteCarga3 = analogRead(carga3Pin) * (voltaje / 4095.0);
  float corrienteCarga4 = analogRead(carga4Pin) * (voltaje / 4095.0);
  float corrienteCarga5 = analogRead(carga5Pin) * (voltaje / 4095.0);

  // Calcular potencia (P = V * I)
  float potenciaSolar = voltaje * corrienteSolar;
  float potenciaBateria = voltaje * corrienteBateria;
  float potenciaEDE = voltaje * corrienteEDE;
  float potenciaCarga1 = voltaje * corrienteCarga1;
  float potenciaCarga2 = voltaje * corrienteCarga2;
  float potenciaCarga3 = voltaje * corrienteCarga3;
  float potenciaCarga4 = voltaje * corrienteCarga4;
  float potenciaCarga5 = voltaje * corrienteCarga5;

  // Mostrar valores en LCD 1
  lcd1.setCursor(4, 0);
  lcd1.print(String(potenciaSolar, 1) + "W");
  lcd1.setCursor(12, 0);
  lcd1.print(String(potenciaBateria, 1) + "W");
  lcd1.setCursor(0, 1);
  lcd1.print("EDE:" + String(potenciaEDE, 1) + "W");

  // Mostrar valores en LCD 2
  lcd2.setCursor(0, 0);
  lcd2.print("C1:" + String(potenciaCarga1, 1) + "W");
  lcd2.setCursor(6, 0);
  lcd2.print("C2:" + String(potenciaCarga2, 1) + "W");
  lcd2.setCursor(12, 0);
  lcd2.print("C3:" + String(potenciaCarga3, 1) + "W");
  lcd2.setCursor(0, 1);
  lcd2.print("C4:" + String(potenciaCarga4, 1) + "W");
  lcd2.setCursor(6, 1);
  lcd2.print("C5:" + String(potenciaCarga5, 1) + "W");

float sumaPotenciasCargas = (potenciaCarga1 + potenciaCarga2 + potenciaCarga3 + potenciaCarga4 + potenciaCarga5);

  // Activar relé de activación de baterías si la suma de potencias de las cargas supera el umbral
  if (sumaPotenciasCargas >= potenciaSolar) {
    digitalWrite(activacionBateriasPin, HIGH); // Activar el relé de activación de baterías
  } else {
    digitalWrite(activacionBateriasPin, LOW); // Desactivar el relé de activación de baterías
  }

  // Activar relé de activación de EDE si la potencia EDE es mayor que la potencia solar
  if (potenciaEDE >= potenciaBateria) {
    digitalWrite(activacionEDEPin, HIGH); // Activar el relé de activación de EDE
  } else {
    digitalWrite(activacionEDEPin, LOW); // Desactivar el relé de activación de EDE
  }

  delay(50);  // Esperar antes de la próxima lectura
}
