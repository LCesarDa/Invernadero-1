


#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 29         // Pin digital donde está conectado el sensor DHT11
#define DHTTYPE DHT11     // Tipo de sensor DHT (en este caso, DHT11)
DHT dht(DHTPIN, DHTTYPE); // Inicializa el sensor DHT

#define SOIL_MOISTURE_PIN A0    // Pin analógico donde está conectado el sensor de humedad de tierra
#define RELAY_PIN 33             // Pin digital donde está conectado el módulo relé
#define FAN_PIN 25               // Pin digital donde está conectado el ventilador

#define LCD_RS 2         // Pin RS del display LCD
#define LCD_E 3         // Pin E del display LCD
#define LCD_D4 4        // Pin D4 del display LCD
#define LCD_D5 5       // Pin D5 del display LCD
#define LCD_D6 11        // Pin D6 del display LCD
#define LCD_D7 12        // Pin D7 del display LCD
LiquidCrystal lcd(LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7); // Inicializa el display LCD

void setup() {
  dht.begin();     // Inicializa el sensor DHT
  lcd.begin(16, 2); // Inicializa el display LCD
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
}

void loop() {
  // Lee la temperatura y humedad del ambiente
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Lee la humedad de la tierra
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  // Si la temperatura es mayor que 30°C y la humedad es menor que 60%, activa el ventilador
  if (temperature > 30 && humidity < 60) {
    digitalWrite(FAN_PIN, HIGH);
  } else {
    digitalWrite(FAN_PIN, LOW);
  }

  // Si la humedad de la tierra es menor que 400 y la temperatura es menor que 30°C, activa la bomba de agua
  if (soilMoisture < 400 && temperature < 30) {
    digitalWrite(RELAY_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, LOW);
  }

  // Muestra los valores de temperatura, humedad y humedad de tierra en el display LCD
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Hum: ");
  lcd.print(humidity);
  lcd.print("%  ");

  lcd.setCursor(9, 1);
  lcd.print("Moist: ");
  lcd.print(soilMoisture);
  lcd.print("  ");

  delay(1000);
}
