

#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();


#define LED 7   		// LED pin on Arduino board
int leds[12];
int pins[12];

void setup () {
 pinMode(LED, OUTPUT);
  MIDI.begin(); 
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);
	 for (int i = 0; i < 12; ++i)
	 {
	 	pins[i]=i+2;
	 	pinMode(pins[i], OUTPUT);
	 	
	 }
             	// Launch MIDI with default options
				// input channel is set to 4
}


void loop() {
	 MIDI.read();
	 //turnOff();
}

void turnOneOff(int nLed){
	digitalWrite(nLed, LOW);
}


void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
	//openLed(LED);

  // Do whatever you want when you receive a Note On.
	if(pitch==48){
		openLed(pins[0]);
	}
	if(pitch==49){
		openLed(pins[1]);
	}
  if(pitch==50){
		openLed(pins[2]);
	}
	if(pitch==51){
		openLed(pins[3]);
	}
  if(pitch==52){
		openLed(pins[4]);
	}
	if(pitch==53){
		openLed(pins[5]);
	}
  if(pitch==54){
		openLed(pins[6]);
	}
	if(pitch==55){
		openLed(pins[7]);
	}
	if(pitch==56){
		openLed(pins[8]);
	}
	if(pitch==57){
		openLed(pins[9]);
	}
  if(pitch==58){
		openLed(pins[10]);
	}
	if(pitch==59){
		openLed(pins[11]);
	}
  
}

void HandleNoteOff(byte channel, byte pitch, byte velocity) { 
	//openLed(LED);

  // Do whatever you want when you receive a Note On.
	if(pitch==48){
		turnOneOff(pins[0]);
	}
	if(pitch==49){
		turnOneOff(pins[1]);
	}
  if(pitch==50){
		turnOneOff(pins[2]);
	}
	if(pitch==51){
		turnOneOff(pins[3]);
	}
  if(pitch==52){
		turnOneOff(pins[4]);
	}
	if(pitch==53){
		turnOneOff(pins[5]);
	}
  if(pitch==54){
		turnOneOff(pins[6]);
	}
	if(pitch==55){
		turnOneOff(pins[7]);
	}
	if(pitch==56){
		turnOneOff(pins[8]);
	}
	if(pitch==57){
		turnOneOff(pins[9]);
	}
  if(pitch==58){
		turnOneOff(pins[10]);
	}
	if(pitch==59){
		turnOneOff(pins[11]);
	}
  
}

void openLed(int nPin){
	digitalWrite(nPin, HIGH);
}
