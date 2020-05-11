//  244117 Wojciech Serewis PTM PT 09:15

/////////////////////////////////////////////////////////
/////////////////THREE BUTTON CALCULATOR/////////////////
/////////////////////////////////////////////////////////

///////////////////////MANUAL////////////////////////////
//B1 = BUTTON 1 - INCREASING VALUE OF CURRENT DIGIT(0-9)/
//B2 = BUTTON 2 - CHANGING ACTION/SIGN///////////////////
//B3 = BUTTON 3 - APPLY DATA FROM BUTTON 1 & 2///////////
//AVAIBLE ACTIONS ON BUTTON 2:///////////////////////////
//- " " NEXT DIGIT IN NUMBER/////////////////////////////
//- "=" PRINTS RESULT////////////////////////////////////
//- "+" SUM OF NUMBERS///////////////////////////////////
//- "-" DIFFERENCE OF NUMBERS////////////////////////////
//- "*" MULTIPLY NUMBERS/////////////////////////////////
//- "/" DIVIDE NUMBERS///////////////////////////////////
//- "." ----------------/////////////////////////////////
//-"RESET" RESTARTING CALCULATOR/////////////////////////
/////////////////////////////////////////////////////////
//1. TO APPLY DIGIT DONT USE "=" IT CAN COUSE ///////////
///////////////////////////////////UNNATURAL BEHAVIOUR///
//2. TO USE "=" MAKE SURE THERE IS NO BLINKING DIGIT/////
//3. LCD ONLY SHOWS DIGITS TO 1/100, IF U WANT //////////
////TO GET FURTHER DIGIT SIMPLY MULTYPLY BY POWER OF 10//
/////////////////////////////////////////////////////////



#include <LiquidCrystal.h>
const int rs = 3, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Buttons Service
long int debounce = 20;

const int  B1PIN = 14;
int B1Counter = 0;
int B1State = 0;
int lastB1State = 0;
long int dt1 = 0;

const int  B2PIN = 15;
int B2Counter = 0;
int B2State = 0;
int lastB2State = 0;
long int dt2 = 0;

const int  B3PIN = 16;
int B3Counter = 0;
int B3State = 0;
int lastB3State = 0;
long int dt3 = 0;

//Calculator Variables
long int dtblink = 0;
int *Count[3] = {&B1Counter, &B2Counter, &B3Counter};
double num1 = 0;    //Number 2
double num2 = 0;    //Number 1
int numi1 = 0;      // num1 integer to display integers
int numi2 = 0;      //num2 integer to display integers
int current = 0;    //accepted digits
int counter = 0;    //current changing digit
int ms = 0;         //Last used sign sign(doesnt count =,"","RESET")
double result = 0;  //Result
String Sign[8] = { " ", "=", "+", "-", "*", "/", ".", "RESET"};

/////////////////////////////////////////////////////////////
//////////////////////SETUP & MAIN LOOP//////////////////////
/////////////////////////////////////////////////////////////
void setup() {
  // Buttons
  pinMode(B1PIN, INPUT);
  pinMode(B2PIN, INPUT);
  pinMode(B3PIN, INPUT);

  lcd.begin(16, 2);
  lcd.print("Bush did 9/11");

  Serial.begin(9600);// debuggin purpose

}

void loop() {

  buttons_Service();

  calc_debug();

  Print();

}

/////////////////////////////////////////////////////////////
//////////////////////MAIN FUNCTIONS/////////////////////////
/////////////////////////////////////////////////////////////

//PRINT
void Print() {
  lcd.clear();
  lcd.setCursor(0, 0);
  numi1 = num1;
  numi2 = num2;
  if (num1 == numi1) { // Code unfortunately is doubled becouse of printing additional zeros when they are not needed.
    if (numi1 == 0)blinknum(numi1);
    else lcd.print(numi1);
    if (ms == 0)lcd.print(Sign[*Count[1] % 8]);
    else lcd.print(Sign[ms]);
  } else {
    if (num1 == 0)blinknum(num1);
    else lcd.print(num1);
    if (ms == 0)lcd.print(Sign[*Count[1] % 8]);
    else lcd.print(Sign[ms]);
  }
  if (numi2 == num2) {// Same as above
    if (num1 != 0 || num2 != 0) {
      if (num1 != 0 && num2 == 0)blinknum(numi2);
      else lcd.print(numi2);
    }
  } else {
    if (num1 != 0 || num2 != 0) {
      if (num1 != 0 && num2 == 0)blinknum(num2);
      else lcd.print(num2);
    }
  }
                      //NOW IT'S OK, THERE'S NO MORE UNECESSARY ZEROS
  if (ms != 0 )lcd.print(Sign[*Count[1] % 8]);
  if (result != 0 &&  current == 0 && counter == 0) {
    lcd.setCursor(0, 1);
    lcd.print("=");
    lcd.print(result);
  }
  else if (result != 0) {
    lcd.setCursor(0, 1);
    if (current != 0)lcd.print(current);
    lcd.print(counter);
  } else {
    lcd.setCursor(0, 1);
    if (current != 0)lcd.print(current);
    lcd.print(counter);
  }
}

///BUTTONS SERVICE
void buttons_Service() {
  buttons_Background();
  buttons_Foreground();
}
void buttons_Foreground() {
  counter = *Count[0] % 10;
  if ((*Count[2]) % 2 == 1) { //WHEN DATA IS APPLIED
    *Count[2] = 0;
    switch (*Count[1] % 8) {

      case 0:   //NEXT DIGIT
        current = current * 10 + counter;
        break;

      case 1:   //A&B=R
        calculate();
        if (current != 0 || counter != 0) {
          num1 = result;
          num2 = current * 10 + counter;
          calculate();
        }
        current = 0;
        break;

      case 2:   //A+B

        if (num1 == 0) {
          num1 = current * 10 + counter;
        } else if (num2 == 0) {
          num2 = current * 10 + counter;
        } else if (num1 != 0 && num2 != 0) {
          calculate();
          num1 = result;
          if (counter != 0 || current != 0)num2 = current * 10 + counter;
        }
        result = 0;
        current = 0;
        ms = *Count[1] % 8;
        break;

      case 3:   //A-B

        if (num1 == 0) {
          num1 = current * 10 + counter;
        } else if (num2 == 0) {
          num2 = current * 10 + counter;
        } else if (num1 != 0 && num2 != 0) {

          calculate();
          num1 = result;
          num2 =  current * 10 + counter;
        }
        result = 0;
        current = 0;
        ms = *Count[1] % 8;
        break;

      case 4:   //A*B
        if (num1 == 0) {
          num1 = current * 10 + counter;
        } else if (num2 == 0) {
          num2 = current * 10 + counter;
        } else if (num1 != 0 && num2 != 0) {

          calculate();
          num1 = result;
          num2 =  current * 10 + counter;
        }
        result = 0;
        current = 0;
        ms = *Count[1] % 8;
        break;

      case 5:   // A/B
        if (num1 == 0) {
          num1 = current * 10 + counter;
        } else if (num2 == 0) {
          num2 = current * 10 + counter;
        } else if (num1 != 0 && num2 != 0) {

          calculate();
          num1 = result;
          num2 =  current * 10 + counter;
        }
        result = 0;
        current = 0;
        ms = *Count[1] % 8;
        break;

      case 6://. (IN THE FURUTE) MAYBE
        //current = current +counter/100 UNAVAIBLE
        break;

      case 7://RESET CALC
        restart();
        break;

    }

    counters_reset();
  }
}


void buttons_Background() {
  //SPRAWDZANIE PRZYCISKU 1
  B1State = digitalRead(B1PIN);
  if (B1State != lastB1State && (millis() - dt1) > debounce) {
    dt1 = millis();
    if (B1State == HIGH) B1Counter++;
  }
  lastB1State = B1State;


  //SPRAWDZANIE PRZYCISKU 2
  B2State = digitalRead(B2PIN);
  if (B2State != lastB2State && (millis() - dt2) > debounce) {
    dt2 = millis();
    if (B2State == HIGH) B2Counter++;
  }
  lastB2State = B2State;


  //SPRAWDZANIE PRZYCISKU 3
  B3State = digitalRead(B3PIN);
  if (B3State != lastB3State && (millis() - dt3) > debounce) {
    dt3 = millis();
    if (B3State == HIGH) B3Counter++;
  }
  lastB3State = B3State;
}



////////////////////////SIDE FUNCTIONS/////////////////////////


void counters_reset() {
  for (auto i : Count)
    *i = 0;
}

void Buttons_debug() {
  for (int i = 0; i < 3; i++) {
    Serial.print("*Count[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(*Count[i]);
  }
}
void calc_debug() {
  Serial.print("num1: ");
  Serial.println(num1);
  Serial.print("num2: ");
  Serial.println(num2);
  Serial.print("result ");
  Serial.println(result);
}

void blinknum(int num) {
  if ((millis() - dtblink) < 1000) {

    lcd.print(num);
  } else if ((millis() - dtblink) < 2000) {
    lcd.print(" ");
  } else {
    dtblink = millis();
  }
}

void restart() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("RESTARTING...");
  delay(1000);
  lcd.noDisplay();
  delay(500);
  lcd.display();
  //deleting all data
  num1 = 0;
  num2 = 0;
  current = 0;
  counter = 0;
  ms = 0;
  result = 0;
  counters_reset();
}

void calculate() {
  switch (ms) {
    case 0://??
      break;

    case 1:
      result = result;
      break;

    case 2:
      result = num1 + num2;

      break;

    case 3:
      result = num1 - num2;
      break;

    case 4:
      result = num1 * num2;
      break;

    case 5:
      result = num1 / num2;
      break;

    case 6://IN THE FUTURE

      break;

    case 7://RESET ?
      break;
  }
}
