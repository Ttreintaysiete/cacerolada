

#include <MIDI.h>
// MIDI_CREATE_DEFAULT_INSTANCE();

MIDI_Class MIDI;
bool NOTEOFFS[32];

#define LED 7   		// LED pin on Arduino board
int leds[12];
int pins[12];
int octave=0;

void setup () {
 pinMode(LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI); 
  // MIDI.setHandleNoteOn(noteOn);
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleAfterTouchChannel(HandleAfterTouchChannel);
  MIDI.setHandleAfterTouchPoly(HandleAfterTouchPoly);
  // MIDI.setHandleNoteOff(noteOff);

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
	 // noteOn();
	 // for (int i = 0; i < 12; ++i)
	 // {
	 // 	openLed(pins[i]);
	 // }
	 // delay(500);
	 // for (int i = 0; i < 12; ++i)
	 // {
	 // 	turnOneOff(pins[i]);
	 // }
	 // noteOff();
	 // delay(500);

	 //turnOff();
}
void noteOn(){
   digitalWrite(13,HIGH);
   
}

void noteOff(){
   digitalWrite(13,LOW);

}

void turnOneOff(int nLed){
	digitalWrite(nLed, LOW);
}

void openLed(int nPin){
	digitalWrite(nPin, HIGH);
}


void HandleAfterTouchChannel(byte channel, byte pressure){
	if (pressure<50)
	{
		for (int i = 0; i < 12; ++i)
		{
			turnOneOff(pins[i]);
		}
	}

}

void HandleAfterTouchPoly (byte channel, byte note, byte pressure){
	//openLed(LED);

  // Do whatever you want when you receive a Note On.
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[0]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[1]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[2]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[3]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[4]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[5]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[6]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[7]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[8]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[9]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[10]);
		}
	}
	if(note==((12*octave))){
		if (pressure<10)
		{
			turnOneOff(pins[11]);
		}
	}
	
  
}

void HandleNoteOn(byte channel, byte pitch, byte velocity) { 
	//openLed(LED);

  // Do whatever you want when you receive a Note On.
	if(pitch==(48+(12*octave))){
		if (velocity>0)
		{
			openLed(pins[0]);
		}else{
			turnOneOff(pins[0]);
		}
		

	}
	if(pitch==(49+(12*octave))){
		if (velocity>0)
		{
			openLed(pins[1]);
		}else{
			turnOneOff(pins[1]);
		}
		

	}
  if(pitch==(50+(12*octave))){
  	if (velocity>0)
  	{
  			openLed(pins[2]);
  	}else{
  		turnOneOff(pins[2]);
  	}
		

	}
	if(pitch==(51+(12*octave))){
		if (velocity>0)
		{
			openLed(pins[3]);
		}else{
			turnOneOff(pins[3]);
		}
		

	}
  if(pitch==(52+(12*octave))){
  	if (velocity>0)
  	{
  			openLed(pins[4]);
  	}else{
  		turnOneOff(pins[4]);
  	}
		

	}
	if(pitch==(53+(12*octave))){
		if (velocity>0)
		{
			openLed(pins[5]);
		}else{
			turnOneOff(pins[5]);
		}
		

	}
  if(pitch==(54+(12*octave))){
  	if (velocity>0)
  	{
  			openLed(pins[6]);
  	}else{
  		turnOneOff(pins[6]);
  	}
		

	}
	if(pitch==(55+(12*octave))){
		if (velocity>0)
		{
			openLed(pins[7]);
		}else{
			turnOneOff(pins[7]);
		}
		

	}
	if(pitch==(56+(12*octave))){
		if (velocity>0)
		{
			openLed(pins[8]);
		}else{
			turnOneOff(pins[8]);
		}
		

	}
	if(pitch==(57+(12*octave))){
		if (velocity>0)
		{
			openLed(pins[9]);
		}else{
			turnOneOff(pins[9]);
		}
		

	}
  if(pitch==(58+(12*octave))){
  	if (velocity>0)
  	{
  			openLed(pins[10]);
  	}else{
  			turnOneOff(pins[10]);
  	}
		

	}
	if(pitch==(59+(12*octave))){
		if (velocity>0)
		{
			openLed(pins[11]);
		}else{
			turnOneOff(pins[11]);
		}
		

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


