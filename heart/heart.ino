#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>        

const int PulseWire = 0;       
const int LED13 = 13;          
int Threshold = 500; 
 int average =0;
 int sum=0;         
 int k=0;

 

                               
PulseSensorPlayground pulseSensor;  

 

void setup() {   

 

  Serial.begin(9600);          

 

 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       
  pulseSensor.setThreshold(Threshold);   

 

  
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  
  }
  
}

 

 

void loop() {

 

  while(k<=30){
  int myBPM = pulseSensor.getBeatsPerMinute();  
   if (pulseSensor.sawStartOfBeat()) {            
// Serial.println("♥  A HeartBeat Happened ! "); 
 Serial.print("BPM: ");                        
 Serial.println(myBPM);   
 sum=sum+myBPM;
 k=k+1;
                   
}
delay(50);     
//Serial.println(i);
                   
}
average=sum/k;
Serial.println("****************************");
Serial.println(k);
Serial.println(sum);
Serial.println(average);
Serial.println("****************************");
delay(5000);

 

sum=0;
k=0;
average=0;

 

}
