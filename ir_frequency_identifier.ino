/*
 * Author: Jenver I.
 * 
 * Sketch for using the IR receiver sensor present on
 * IR remote controlled appliances, used to determine
 * the frequency of the signal by measuring the high and low
 * period of 1 cycle
 */

struct signalData {
  bool state;
  unsigned long microSec;
};

#define maxDataLength 4
#define signalInputPin 2

volatile signalData signalSpecimen[maxDataLength];          //container of IR signal data
volatile short dataLength = 0;
volatile bool isProbingComplete = false;

bool signalState = HIGH;
unsigned long interruptMicro = 0;

void setup() {
  Serial.begin(9600);
  pinMode(signalInputPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(signalInputPin), signalHandler, CHANGE);
  Serial.println("Watching for IR signal");
}

void loop() {

  //If the probing was completed display the reading
  if (isProbingComplete){
    unsigned long highTime = signalSpecimen[1].microSec - signalSpecimen[0].microSec;
    unsigned long lowTime = signalSpecimen[2].microSec - signalSpecimen[1].microSec;
    Serial.print("High time ");
    Serial.println(highTime);
    Serial.print("Low time ");
    Serial.println(lowTime);
    isProbingComplete = false;
    dataLength = 0;
  }
}

void signalHandler(){
  
  //Listener for the IR signal
  //stop listening after probing 1 cycle
  if (dataLength < maxDataLength - 1){
    interruptMicro = micros();
    signalState = !signalState;
    signalSpecimen[dataLength].state = signalState;
    signalSpecimen[dataLength].microSec = interruptMicro;
    dataLength++;
  }else{
    isProbingComplete = true;
  }
}
