#include <LiquidCrystal_I2C.h>
#define Mode_Button 5
#define Sensor1 6
#define Sensor2 7
#define ControlLed 8

#define ON LOW
#define OFF HIGH

#define Mode_1 true
#define Mode_2 false

LiquidCrystal_I2C lcd(0x3F, 16, 2);
static unsigned int counter;
bool Mode_Check = Mode_1;

void Init_LCD(){
  lcd.init();       // Initialize the lcd
  lcd.backlight();  // Turn on LCD's led
  lcd.clear();      // Clear LCD screen
}

void Init_Sensor(){
  pinMode(Sensor1,INPUT_PULLUP); // Using internal resistor PULLUP
  pinMode(Sensor2,INPUT_PULLUP); // Using internal resistor PULLUP
}

void Init_Control(){
  pinMode(ControlLed,OUTPUT); // OUPUT to control light
  digitalWrite(ControlLed, OFF); // Set off light when init the system
}

void Init_Button(){
  pinMode(Mode_Button,INPUT_PULLUP); // Using internal resistor PULLUP
}

void LCD_display(bool Test_case){ 
  // Display information
  lcd.setCursor(0, 0);   
  lcd.print("Control:");
  lcd.setCursor(10, 0);
  
  if (Test_case == Mode_1)
    lcd.print("OFF");
  else if (Test_case == Mode_2)
    lcd.print("ON ");
   
  lcd.setCursor(4, 1);
  lcd.print("Count: ");
  lcd.print(counter);  
}

void Mode_Select(){
  if (!digitalRead(Mode_Button)){
    while (!digitalRead(Mode_Button));
    
    if (Mode_Check == Mode_1)
      Mode_Check = Mode_2;
    else 
      Mode_Check = Mode_1;
  }
}

void Control(){
  if (Mode_Check == Mode_2){
    if (counter > 0)
      digitalWrite(ControlLed, ON);
    else 
      digitalWrite(ControlLed, OFF);
  }
}

void setup() {
  Serial.begin(9600); 

  // Init the system
  Init_LCD();
  Init_Sensor();
  Init_Control();
  Init_Button();
}

void loop() {
  Serial.println(Mode_Check);
  LCD_display(Mode_Check);
  Mode_Select();
  Control();

  // Check sensor 1, if sensor 1 is covered
  if(digitalRead(Sensor1) == 0)
  {
    while(digitalRead(Sensor1) == 0); // Loop until sensor 1 is uncovered 
    //delay(100); //We can put the delay here to avoid people doesn't cover the sensor2 right-after sensor 1 is uncovered
      if(digitalRead(Sensor2) == 0) // If sensor 2 is covered right-after sensor 1 is uncovered
      {
        while(digitalRead(Sensor2)==0);// Loop until sensor 2 is uncovered
        counter++; // Increase counter
      }
    }

  // Decrease also like that
  else if (digitalRead(Sensor2) == 0)
  {
     while(digitalRead(Sensor2) == 0);
      if(digitalRead(Sensor1) == 0)
      {
        while(digitalRead(Sensor1)==0);
          if(counter != 0)
            {
              counter--;  
            }
      }
  }

    
}
