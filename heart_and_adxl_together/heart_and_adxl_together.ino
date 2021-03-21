#include <SIM800L.h>



//pulse
#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h> 
//adxl
#include <Wire.h>
#include <ADXL345.h>



//gsm2

#include <SoftwareSerial.h>


#define SIM800_RST_PIN 2


//
char temp1[100];
char temp2[100];
char temp3[100];
//gsm
String response="";
const char APN[] = "airtelgprs.com";
String url = "https://lit-refuge-49573.herokuapp.com/device/";
String tapandf="https://lit-refuge-49573.herokuapp.com/device/emergency",
       dataUrl="https://lit-refuge-49573.herokuapp.com/device/data",
       location="https://lit-refuge-49573.herokuapp.com/device/location";
String tapData="{\"deviceToken\": \"cee28af31726\", \"option\": \"tap\"}";
String fallData="{\"deviceToken\": \"cee28af31726\", \"option\": \"freefall\"}";
String data = "";
//String data="{\"deviceToken\": \"cee28af31726\"} \"footsteps\": ?, \"bpm\": # }";
String loc="{\"deviceToken\": \"cee28af31726\"} \"lat\": ?, \"lng\": # }";
const char CONTENT_TYPE[] = "application/json";
//const char PAYLOAD[] = "{\"bpm\": 95, \"deviceToken\": \"cee28af31726\"}";

SIM800L* sim800l;

//adxl
ADXL345 adxl; //variable adxl is an instance of the ADXL345 library


#define DEVICE (0x53)    //ADXL345 device address
#define TO_READ (6)        //num of bytes we are going to read each time (two bytes for each axis)

#define offsetX   -10.5       // OFFSET values
#define offsetY   -2.5
#define offsetZ   -4.5

#define gainX     257.5   
#define gainY     254.5
#define gainZ     248.5



byte buff[TO_READ] ;    //6 bytes buffer for saving data read from the device
char str[512];                      //string buffer to transform data before sending it to the serial port

int x,y,z;
  int a,b,c;
  int regAddress = 0x32; 

int xavg, yavg,zavg, steps=0, flag=0;
int xval[15]={0}, yval[15]={0}, zval[15]={0};
int threshhold = 60.0;


const int PulseWire = 0;       
const int LED13 = 13;          
int Threshold = 500; 
 int average =0;
 int sum=0;         
 int k=0;
  int myBPM =0; 

 PulseSensorPlayground pulseSensor;  


void setup() {
  Serial.begin(115200);      
   Serial.println("*****************");
  Wire.begin();        // join i2c bus (address optional for master)  // start serial for output
  adxl.powerOn();
  
   
  //Turning on the ADXL345
  writeTo(DEVICE, 0x2D, 0);      
  writeTo(DEVICE, 0x2D, 16);
  writeTo(DEVICE, 0x2D, 8);
  Serial.println("*****************");
 adxl.setTapDetectionOnX(0);
 adxl.setTapDetectionOnY(0);
 adxl.setTapDetectionOnZ(1);
  Serial.println("*****************");
 adxl.setTapThreshold(60); //62.5mg per increment
 adxl.setTapDuration(30); //625μs per increment
  Serial.println("*****************");
 adxl.setFreeFallThreshold(5); //(5 - 9) recommended - 62.5mg per increment
 adxl.setFreeFallDuration(19); //(20 - 70) recommended - 5ms per increment
  Serial.println("*****************");
 adxl.setInterruptMapping( ADXL345_INT_SINGLE_TAP_BIT,  ADXL345_INT1_PIN );
 adxl.setInterruptMapping( ADXL345_INT_FREE_FALL_BIT,  ADXL345_INT1_PIN );
  Serial.println("*****************");
 adxl.setInterrupt( ADXL345_INT_SINGLE_TAP_BIT, 1);
 adxl.setInterrupt( ADXL345_INT_DOUBLE_TAP_BIT, 1);
 adxl.setInterrupt( ADXL345_INT_FREE_FALL_BIT, 1);
  Serial.println("*****************");
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       
  pulseSensor.setThreshold(Threshold); 
  pulseSensor.begin(); 
  Serial.println("*****************");
  // Initialize a SoftwareSerial
  SoftwareSerial* serial = new SoftwareSerial(11, 10);
  serial->begin(9600);
   
  // Initialize SIM800L driver with an internal buffer of 200 bytes and a reception buffer of 512 bytes, debug disabled
  sim800l = new SIM800L((Stream *)serial, SIM800_RST_PIN, 200, 1024);

  // Equivalent line with the debug enabled on the Serial
  // sim800l = new SIM800L((Stream *)&Serial1, SIM800_RST_PIN, 200, 512, (Stream *)&Serial);

  // Setup module for GPRS communication
  setupModule();




}

void loop() {

  bool connected = false;
  for(uint8_t i = 0; i < 70 && !connected; i++) {
    connected = sim800l->connectGPRS();
  }
    if(connected) {
    Serial.println("GPRS connected !");
  } else{
    //Serial.println(F("GPRS not connected !"));
  }
 /////////////////////////////////////////////////task 2 adxl sensing
  adxl.readXYZ(&a, &b, &c);
  byte interrupts = adxl.getInterruptSource();

 ///////////////////////////////////////////////////task 3 free fall
  if(adxl.triggered(interrupts, ADXL345_FREE_FALL)){
  Serial.println("freefall");
//  resonse = sendRequest(tapandf,fallData,2500,5000);
  
  //add code here to do when freefall is sensed
 } 
  ////////////////////////////////////////////////task 4 tap
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP)){
  Serial.println("tap");
// response = sendRequest(tapandf,tapData,2500,5000);
 
  //add code here to do when a tap is sensed
 } 

  ////////////////////////////////////////////////task 1 heart sensing
   myBPM= pulseSensor.getBeatsPerMinute();  
  if (pulseSensor.sawStartOfBeat()) {                               
     sum=sum+myBPM;
     k=k+1;
//     z(myBPM));
  }



x = ArduinoPedometer();
 Serial.println("*****************");
 Serial.println(x);

                  
if(k==10){
  average=sum/k;
//Serial.println(F("****************************"));
//Serial.println(F(k));
//Serial.println(F(sum));
Serial.println(average);
//Serial.println(F("****************************"));
//Serial.println(F(x));
data="{\"deviceToken\": \"cee28af31726\"} \"footsteps\":"+String(x)+",\"bpm\": "+String(average)+" }";

response = sendRequest(dataUrl,data,2500,5000 );
Serial.println(response);
x=0;
steps=0;

//Serial.println(F("****************************"));

k=0;
sum=0;
average=0;

delay(5000);}

}

//Get pedometer.

int ArduinoPedometer(){
    int acc=0;
    int totvect[15]={0};
    int totave[15]={0};
    int xaccl[15]={0};
    int yaccl[15]={0};
    int zaccl[15]={0};
    for (int i=0;i<15;i++)
    {
      xaccl[i]= x;
      delay(1);
      yaccl[i]= y;
      delay(1);
      zaccl[i]= z;
      delay(1);
      totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
      totave[i] = (totvect[i] + totvect[i-1]) / 2 ;
      delay(150);
  
      //cal steps 
      if (totave[i]>threshhold && flag==0)
      {
         steps=steps+1;
         flag=1;
      }
      else if (totave[i] > threshhold && flag==1)
      {
          //do nothing 
      }
      if (totave[i] <threshhold  && flag==1)
      {
        flag=0;
      }
     // Serial.print("steps=");
     // Serial.println(steps);
     return(steps);
    }
  delay(100); 
 }

String sendRequest(String url,String payload,int r,int w){
     url.toCharArray(temp1,100);
     payload.toCharArray(temp2,100);
     uint16_t rc = sim800l->doPost(temp1, CONTENT_TYPE,temp2 , r, w);
     if(rc==200) return sim800l->getDataReceived();
     else return "error";
}
void writeTo(int device, byte address, byte val) {
   Wire.beginTransmission(device); //start transmission to device 
   Wire.write(address);        // send register address
   Wire.write(val);        // send value to write
   Wire.endTransmission(); //end transmission
}

//reads num bytes starting from address register on device in to buff array
void readFrom(int device, byte address, int num, byte buff[]) {
  Wire.beginTransmission(device); //start transmission to device 
  Wire.write(address);        //sends address to read from
  Wire.endTransmission(); //end transmission
  
  Wire.beginTransmission(device); //start transmission to device
  Wire.requestFrom(device, num);    // request 6 bytes from device
  
  int i = 0;
  while(Wire.available())    //device may send less than requested (abnormal)
  { 
    buff[i] = Wire.read(); // receive a byte
    i++;
  }
  Wire.endTransmission(); //end transmission
}


void setupModule() {
    // Wait until the module is ready to accept AT commands
  while(!sim800l->isReady()) {
    Serial.println("Problem to initialize AT command, retry in 1 sec");
    delay(1000);
  }
  Serial.println("Setup Complete!");

  // Wait for the GSM signal
  uint8_t signal = sim800l->getSignal();
  while(signal <= 0) {
    delay(1000);
    signal = sim800l->getSignal();
  }
  Serial.print("Signal OK (strenght: ");
  Serial.print(signal);
  Serial.println(")");
  delay(1000);

  // Wait for operator network registration (national or roaming network)
  NetworkRegistration network = sim800l->getRegistrationStatus();
  while(network != REGISTERED_HOME && network != REGISTERED_ROAMING) {
    delay(1000);
    network = sim800l->getRegistrationStatus();
  }
 // Serial.println("Network registration OK");
  delay(1000);

  // Setup APN for GPRS configuration
  bool success = sim800l->setupGPRS(APN);
  while(!success) {
    success = sim800l->setupGPRS(APN);
    delay(5000);
  }
 // Serial.println("GPRS config OK");
}
