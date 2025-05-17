

#include <LCDI2C_Multilingual.h>

LCDI2C_Symbols lcd(0x3F, 16, 2);
const int an = A0;  // Analog input pin that the potentiometer is attached to
const int led = 13; // Analog output pin that the LED
const int voltpin = A1;
const int temppin = A2;
const int presspin = A3;
//const int voltpin = A1;

float R1 = 100000.0;
float R2 = 10000.0;
float k = R2/(R1+R2);


unsigned long lastflash = 0; // Час останнього обрахунку
unsigned long currentTime = 0; // Поточний час
int RPM = 0;                  // Обчислені оберти
int divider = 130;            //130 Кількість зубів для обчислення
int toothCounter = 0;         // Лічильник зубів


void sens() {
  toothCounter++; // Збільшити лічильник зубів
  if (toothCounter >= divider) { // Якщо досягнуто потрібної кількості зубів
    currentTime = micros(); // Зчитати поточний час
    // Обчислення RPM
    RPM = 60 / ((float)(currentTime - lastflash) / 1000000) ;
    lastflash = currentTime; // Оновити час останнього обрахунку
    toothCounter = 0;        // Скинути лічильник зубів
  }
}

void setup() {
  Serial.begin(9600);

  pinMode(voltpin, INPUT);
  pinMode(presspin, INPUT);
  pinMode(temppin, INPUT);
  pinMode(led, OUTPUT);
  
  attachInterrupt(0,sens,RISING); //прерывание на 2 пин 
  
  lcd.init();
  lcd.backlight();

  lcd.print("Hello! ");
  delay(1000);

  lcd.clear();
}

void loop() {
     if ((micros()-lastflash)>1000000){ //если сигнала нет больше секунды
        RPM=0;  //считаем что RPM 0
     }
    
 {
//========================================== tacho
    char buffer[8]; // збереження відформатованого тексту
    sprintf(buffer, "%04d RPM", RPM);
    lcd.setCursor(0, 0);
    lcd.print(buffer);

//===========================================voltage
    float voltage = analogRead(voltpin) * (5.0 / 1023.0)/k;
    Serial.println(voltage);
    buffer[4]; // Масив для збереження відформатованого тексту
    dtostrf(voltage, 3, 1, buffer);
    lcd.setCursor(10, 0);
    lcd.print("     V");
    lcd.setCursor(10, 0);
    lcd.print(buffer);
    if (voltage > 29){
    digitalWrite(led, HIGH);
//    lcd.noBacklight();
    delay(50);}

//============================================press
    float p = analogRead(presspin)/2.0; 
    buffer[8]; // Масив для збереження відформатованого тексту
    dtostrf(p, 3, 1, buffer);
    lcd.setCursor(0, 1);
    lcd.print("     BAR");
    lcd.setCursor(0, 1);
    lcd.print(buffer);

//===========================================temp
    int t =  analogRead(temppin);
    lcd.setCursor(10, 1);
    lcd.print("    °C");
    lcd.setCursor(10, 1);
    lcd.print(t);
    if (t > 105){
    digitalWrite(led, HIGH);
//    lcd.noBacklight();
    delay(50);}
}

    digitalWrite(led, LOW);
    lcd.backlight();
    delay(300);


    }
