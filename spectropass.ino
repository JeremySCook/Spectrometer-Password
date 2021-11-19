#include <Bounce.h>
Bounce button11 = Bounce(11, 10);
const int ledPin = 13;
const int value50a = 200;
const int value50b = 200;
const int value100a = 200;
const int value100b = 200;
const int value150a = 200;
const int value150b = 200;

/*
 * Macro Definitions
 */
#define SPEC_TRG         A0
#define SPEC_ST          A1
#define SPEC_CLK         A2
#define SPEC_VIDEO       A3
#define WHITE_LED        A4
#define LASER_404        A5

#define SPEC_CHANNELS    288 // New Spec Channel
uint16_t data[SPEC_CHANNELS]; //will want to look at certain points in this array to verify ID - perhaps 45?

void setup(){

  //Set desired pins to OUTPUT
  pinMode(SPEC_CLK, OUTPUT);
  pinMode(SPEC_ST, OUTPUT);
  pinMode(LASER_404, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);

  digitalWrite(SPEC_CLK, HIGH); // Set SPEC_CLK High
  digitalWrite(SPEC_ST, LOW); // Set SPEC_ST Low

  Serial.begin(115200); // Baud Rate set to 115200


  pinMode(11, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(22, OUTPUT);
}

/*
 * This functions reads spectrometer data from SPEC_VIDEO
 * Look at the Timing Chart in the Datasheet for more info
 */
void loop(){

//
  button11.update();  
  if (button11.fallingEdge()){
  digitalWrite(WHITE_LED, HIGH);
  for (int i = 0; i < 5; i++){
  delay(10);
  readSpectrometer();
  if(i == 4) printData();
}
  digitalWrite(WHITE_LED, LOW);
  delay(1000);
  Serial.println("");
  }

/*
digitalWrite(22, HIGH); 
readSpectrometer();
printData();
digitalWrite(22, LOW);
delay(50);
*/

}

void readSpectrometer(){

  int delayTime = 1; // delay time

  // Start clock cycle and set start pulse to signal start
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  digitalWrite(SPEC_ST, HIGH);
  delayMicroseconds(delayTime);

  //Sample for a period of time
  for(int i = 0; i < 15; i++){

      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime); 
 
  }

  //Set SPEC_ST to low
  digitalWrite(SPEC_ST, LOW);

  //Sample for a period of time
  for(int i = 0; i < 85; i++){

      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime); 
      
  }

  //One more clock pulse before the actual read
  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  digitalWrite(SPEC_CLK, LOW);
  delayMicroseconds(delayTime);

  //Read from SPEC_VIDEO
  for(int i = 0; i < SPEC_CHANNELS; i++){

      data[i] = analogRead(SPEC_VIDEO);
      
      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime);
        
  }

  //Set SPEC_ST to high
  digitalWrite(SPEC_ST, HIGH);

  //Sample for a small amount of time
  for(int i = 0; i < 7; i++){
    
      digitalWrite(SPEC_CLK, HIGH);
      delayMicroseconds(delayTime);
      digitalWrite(SPEC_CLK, LOW);
      delayMicroseconds(delayTime);
    
  }

  digitalWrite(SPEC_CLK, HIGH);
  delayMicroseconds(delayTime);
  
}

/*
 * The function below prints out data to the terminal or 
 * processing plot
 */
void printData(){
  
  for (int i = 0; i < SPEC_CHANNELS; i++){
    
    //Serial.print(data[i]);
    //Serial.print(',');
    
  }
  int avg50 = (data[50] + data[51] + data[52] + data[53] + data[54] + data[55])/6;
  int avg150 = (data[150] + data[151] + data[152] + data[153] + data[154] + data[155])/6;
  //Serial.print("\n");
  //Serial.print("50-55 average "); Serial.println(avg50);
  //Serial.print("150-155 average "); Serial.println(avg150);
  if((avg50 > 475) && (avg50 < 515) && (avg150 > 295) && (avg150 < 325)){ //nominal avg50 = 495 nom avg150 = 315
    //Serial.println("password correct");
    Keyboard.println("YourPassword##1234");
    delay(10);
  }
}

void unlock(){

  if(data[50] < 500){
    Serial.println("reading 50 < 500");
  }
}
