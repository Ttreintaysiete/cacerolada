/*
--------------------------------------------------------------------------------------------------
Generative midi drum machine derived from the amazing Mutable instrument's Anushri drum sequencer.
--------------------------------------------------------------------------------------------------
I am not a programmer. I stiched this thing together because I wanted to build this and it didn't 
seem very hard. Surprisingly it worked almost immediately.
Feel free to clean up the code, tune it up or whatever.

-----------------------
Cluster and sector map:
-----------------------
(9 clusters, 4 sectors)

	7 ------- 8 ------- 9
	|         |         |
	|    2    |    3    |
	|         |         |
	4 ------- 5 ------- 6
	|         |         |
	|    1    |    4    |
	|         |         |
	1 ------- 2 ------- 3
	
______________________________  
HW connections:

Bass drum density pot  ---> A1
Snare drum density pot ---> A2
HiHat density pot      ---> A3

X coordinate           ---> A5
Y coordinate           ---> A4

Midi from TX obviously. Check the midi out connection from arduino online.
I don't even know which midi notes it's using right now - change it to whatever you need.

example of midi in and out connection circuit: http://mbed.org/media/uploads/Midimetric/midi.png
I did not use the optoisolation for this so far only breadboarded project but you should use it if you want to be perfectly safe.



Have fun!

Honza Hrubes /aka ashimoke
ashimoke@gmail.com
*/

#include <Shifter.h>
#include <MIDI.h>
#include "DRUMNBASS.h"
MIDI_Class MIDI;

#include <Bounce2.h>
#include <Shifter.h>
#define SER_Pin 10 //SER_IN
#define RCLK_Pin 12 //L_CLOCK
#define SRCLK_Pin 11 //CLOCK

#define NUM_REGISTERS 1 //how many registers are in the chain

Shifter shifter(SER_Pin, RCLK_Pin, SRCLK_Pin, NUM_REGISTERS); 

int nLed=0;;
int count=0;

byte midi_start = 0xfa;
byte midi_stop = 0xfc;
byte midi_clock = 0xf8;
byte midi_continue = 0xfb;
int play_flag = 0;
byte data;                               // ^ midi in stuff
byte led=7;
byte note=0;
byte octave=4;

boolean tickSent=false;
#define VOICE_COUNT 16
byte NOTE_ON[VOICE_COUNT];
byte NOTE_CHAN[VOICE_COUNT];
byte NOTE_VEL[VOICE_COUNT];
unsigned long NOTE_OFF[VOICE_COUNT];
long _clockCounter = -1;
long _posCounter = 0;
byte clockInState = LOW ;
long lastClock = millis();
byte debounce= 5; 
Bounce divdown= Bounce( ); 
Bounce divup= Bounce( ); 

Bounce notedown= Bounce( ); 
Bounce noteup= Bounce( ); 


int BDfreq = 0;
int SDfreq = 0;
int HHfreq = 0;                          // variables for storing drum density

int density=127;
int rage=0;
int tempo=12;

float Xmap = 0;
float Ymap = 0;                          // coordinates of the 2D pattern map

int clockcount = 0;

int lastposition = 0;
int Position = 0;
int Sector = 0;                          // I have no idea what I'm doing... but it works... in a way.
float BDrum = 0;
float SDrum = 0;
float HHats = 0;

int tempoLED = 7;

byte division=2;

long interval = 100;           // interval at which to blink (milliseconds)
int ledState = LOW; 

const int ledPin =  13;   
long previousMillis = 0;
byte currentRhythm=0;

void setup() {
	setupMidi();                                  // set baud rate to midi
	pinMode(tempoLED, OUTPUT);  
	noteup.attach(2);
	noteup.interval(5); // interval in ms
	divdown.attach(3);
	divdown.interval(5); // interval in ms
	divup.attach(4);
	divup.interval(5); // interval in ms
	notedown.attach(5);
	notedown.interval(5); // interval in ms
	shifter.setPin(0 , HIGH); //set pin 1 in the chain(second pin) HIGH
	shifter.write(); //send changes to the chain and display them

}

void control(){

	if (divup.update()) { 
		if (divup.read() == HIGH){
			division=(division+2)%16;
		}
	}
	if (divdown.update()) { 
		if (divdown.read() == HIGH){
			if(division>2){division-=2;}
		}
	}
	if (noteup.update()) { 
		if (noteup.read() == HIGH){
			note=(note+3)%64;
		}
	}
	if (notedown.update()) { 
		if (notedown.read() == HIGH){
			if(note>=3){note-=3;}
		}
	}
}

void loop() {
	// shifter.clear(); //set all pins on the shift register chain to LOW
	// shifter.write(); //send changes to the chain and display them
		//clockIn(8);
	MIDI.read();
	

	// count++;
	// delay(100);
	// nLed=nLed%6;
	// // shifter.setPin(nLed , HIGH); //set pin 1 in the chain(second pin) HIGH
	
	// // shifter.write(); 
	// if (count%6==0)
	// {
	// 	nLed++;
	// }
	unsigned long currentMillis = millis();

	if(currentMillis - previousMillis > tempo) {
		// save the last time you blinked the LED 
		previousMillis = currentMillis;   

		// if the LED is off turn it on and vice-versa:
		if (ledState == LOW)
			ledState = HIGH;
		else
			ledState = LOW;

		// set the LED with the ledState of the variable:
		// digitalWrite(ledPin, ledState);
		shifter.setPin(0 , ledState); //set pin 1 in the chain(second pin) HIGH
		shifter.write(); //send changes to the chain and display them
		HandleClock();
		// MIDI.sendNoteOn(36, 127, 1);
	}
	control();
	noteKill();

}

void midiLoop(){

	if(_clockCounter%(1)==0){
	//trace(56, _clockCounter);
	//trace(57, _posCounter);

		Play();
	//playNote(36, 127, 1000, 10);
		_posCounter++;
	}

	//trace(66, 12 ); 
	// noteKill();

		//partCheck();
}

void HandleClock() {
	midiLoop();
	// tickClockLed(3*division);
	_clockCounter++;
}

void reset(){
	_clockCounter=0;
	_posCounter=0;
}

// void tickClockLed(int divi){
// 	if ((_clockCounter % (divi)) == 0) {
// 		digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
// // trace(10, 123 );
// 	}

// 	// reset trigger after number+2 CLOCK_DIVIDER ticks
// 	if (((_clockCounter+2) % (divi)) == 0) {
// 		// wait for a second
// 		digitalWrite(led, LOW);   
// 	 // turn the LED off by making the voltage LOW

// 	}
// }

void noteKill(){

	unsigned long time=millis();
		
		for (int i=0;i<VOICE_COUNT;i++){

				if (NOTE_VEL[i]!=0 && NOTE_OFF[i]<time){  // if note value>0 and note off time < time

					shifter.setPin(1 , HIGH); //set pin 1 in the chain(second pin) HIGH
	 				shifter.write(); //send changes to the chain and display them
					byte pitch = NOTE_ON[i];
					byte velocity = 0;
					MIDI.sendNoteOn(pitch, velocity, NOTE_CHAN[i]);
					// NOTE_ON[i]=0; //wipe NOTE_ON entry to signal no active note
					// NOTE_VEL[i]=0; //wipe NOTE_ON entry to signal no active note
					NOTE_VEL[i]=0; //wipe NOTE_ON entry to signal no active note
					
					shifter.setPin(1 , LOW); //set pin 1 in the chain(second pin) HIGH
	 				shifter.write(); //send changes to the chain and display them
				}
		}
}

void playNote(byte pitch, byte velocity, int duration, byte chan) {
	unsigned long time = millis();
	// find an empty note slot 
	boolean found_slot = false;

	for (int i=0; i<VOICE_COUNT; i++){
		if (NOTE_VEL[i] == 0){

			NOTE_ON[i] = pitch;
			NOTE_VEL[i] = velocity;
			NOTE_OFF[i] = time+duration;
			NOTE_CHAN[i] = chan;
			found_slot = true;
			break;
		}
	}
		

// RED LIGHT SHOWS NOTE BUFFER IS OVERFULL 
	if (found_slot == false){
 			shifter.setPin(3 , HIGH); //set pin 1 in the chain(second pin) HIGH
	 		shifter.write(); //send changes to the chain and display them
	}
	else{
			shifter.setPin(5 , LOW); //set pin 1 in the chain(second pin) HIGH
	 		shifter.write(); //send changes to the chain and display them
		//-  digitalWrite (7, LOW);
		 MIDI.sendNoteOn(pitch, velocity, chan);
			// shifter.setPin(4 , HIGH); //set pin 1 in the chain(second pin) HIGH
			// shifter.write(); //send changes to the chain and display them
	};

}

void setupMidi(){
	MIDI.begin();
	MIDI.setHandleClock ( HandleClock );
	MIDI.setHandleStart(reset);
	MIDI.setHandleNoteOn(noteOn);
	MIDI.setHandleNoteOff(noteOff);
}

void noteOn(){
	 shifter.setPin(0 , HIGH); //set pin 1 in the chain(second pin) HIGH
	 shifter.write(); //send changes to the chain and display them
	}

void noteOff(){
	shifter.setPin(0 , LOW); //set pin 1 in the chain(second pin) HIGH
	shifter.write(); //send changes to the chain and display them
}

void clockIn(int dig){
	
	byte inClock=digitalRead(dig);
	if(inClock == LOW ){
		//clockInState = LOW ;
		digitalWrite(7, LOW);
		tickSent=false;
	}else{
		
		if(!tickSent){
			HandleClock();
			digitalWrite(7, HIGH);
			tickSent=true;
		}
		
		//clockInState == HIGH;
	}
	
}
void Play() {
	
	
	 Position=_posCounter%16;                     // loop after 16 steps

	 lastposition = Position;
	 
	 //BDfreq = analogRead(0);                              // read bass drum density pot
	 density = map(analogRead(4), 0, 1023, 0, 255);                // remap it to values similar to those in cluster configurations
	 note = map(analogRead(2), 0, 1023, 0, 127);                // remap it to values similar to those in cluster configurations
	 tempo = map(analogRead(0), 0, 1023, 1, 1000);                // remap it to values similar to those in cluster configurations
	 // SDfreq = analogRead(1);                              // read snare drum density pot
	 // SDfreq = map(SDfreq, 0, 1023, 0, 255);                // remap it to values similar to those in cluster configurations
	 // HHfreq = analogRead(2);                              // read hi hat density pot
	 // HHfreq = map(HHfreq, 0, 1023, 0, 255);                // remap it to values similar to those in cluster configurations

	 Xmap = analogRead(3);                                // read the X-pot for cluster navigation
	 Xmap = map(Xmap, 0, 1023, -100, 100);                 // remap around zero
	 Ymap = analogRead(4);                                // read the Y-pot for cluster navigation
	 Ymap = map(Ymap, 0, 1023, -100, 100);                 // remap around zero

	 if (Xmap < 0 && Ymap < 0) Sector = 1;
	 if (Xmap < 0 && Ymap > 0) Sector = 2;
	 if (Xmap >= 0 && Ymap >= 0) Sector = 3;
	 if (Xmap >= 0 && Ymap <= 0) Sector = 4;               // determine sector - which 4 clusters do we want to interpolate

	 Xmap = abs(Xmap / 100);
	 Ymap = abs(Ymap / 100);                               // get rid off negative values and make it smaller


	 // if (D2[currentRhythm][Position]>density)

	 	// playNote((0+(octave*12)), 127, 10, 1);
	 
	 if (D2[currentRhythm][Position]>density)playNote(note+0+(octave*12), 127, 100, 1);
	 if (D3[currentRhythm][Position]>density)playNote(note+1+(octave*12), 127, 100, 1);
	 if (D4[currentRhythm][Position]>density)playNote(note+2+(octave*12), 127, 100, 1);
	 if (D5[currentRhythm][Position]>density)playNote(note+3+(octave*12), 127, 100, 1);
	 if (D6[currentRhythm][Position]>density)playNote(note+4+(octave*12), 127, 100, 1);
	 if (D7[currentRhythm][Position]>density)playNote(note+5+(octave*12), 127, 100, 1);
	 if (D8[currentRhythm][Position]>density)playNote(note+6+(octave*12), 127, 100, 1);
	 if (D9[currentRhythm][Position]>density)playNote(note+7+(octave*12), 127, 100, 1);
	 if (D10[currentRhythm][Position]>density)playNote(note+8+(octave*12), 127, 100, 1);
	 if (D10[currentRhythm][Position]>density)playNote(note+9+(octave*12), 127, 100, 1);
	 if (D10[currentRhythm][Position]>density)playNote(note+10+(octave*12), 127, 100, 1);
	 if (D10[currentRhythm][Position]>density)playNote(note+11+(octave*12), 127, 100, 1);
		 // if (D11[currentRhythm][Position]>density)playNote(42+note, 127, 1000, 10);
		 // if (D12[currentRhythm][Position]>density)playNote(42+note, 127, 1000, 10);
		 // if (D13[currentRhythm][Position]>density)playNote(42+note, 127, 1000, 10);
		 // if (D14[currentRhythm][Position]>density)playNote(42+note, 127, 1000, 10);
		 // if (sdrum[currentRhythm]>density)playNote(35+note, 127, 1000, 10);

	 
	 
	//  if (Sector == 1) {
	//    SDrum = ( ( (sdrum1[Position] * Xmap) + (sdrum2[Position] * (1 - Xmap) ) ) * Ymap ) + ( ( (sdrum4[Position] * Xmap) + (sdrum5[Position] * (1 - Xmap) ) ) * ( 1 - Ymap) );
	//    BDrum = ( ( (bdrum1[Position] * Xmap) + (bdrum2[Position] * (1 - Xmap) ) ) * Ymap ) + ( ( (bdrum4[Position] * Xmap) + (bdrum5[Position] * (1 - Xmap) ) ) * ( 1 - Ymap) );
	//    HHats = ( ( (hhopen1[Position] * Xmap) + (hhopen2[Position] * (1 - Xmap) ) ) * Ymap ) + ( ( (hhopen4[Position] * Xmap) + (hhopen5[Position] * (1 - Xmap) ) ) * ( 1 - Ymap) );
	//  }

	//  if (Sector == 2) {
	//    SDrum = ( ( (sdrum4[Position] * Xmap) + (sdrum5[Position] * (1 - Xmap) ) ) * (1 - Ymap ) ) + ( ( (sdrum7[Position] * Xmap) + (sdrum8[Position] * (1 - Xmap) ) ) * Ymap );
	//    BDrum = ( ( (bdrum4[Position] * Xmap) + (bdrum5[Position] * (1 - Xmap) ) ) * (1 - Ymap ) ) + ( ( (bdrum7[Position] * Xmap) + (bdrum8[Position] * (1 - Xmap) ) ) * Ymap );
	//    HHats = ( ( (hhopen4[Position] * Xmap) + (hhopen5[Position] * (1 - Xmap) ) ) * (1 - Ymap ) ) + ( ( (hhopen7[Position] * Xmap) + (hhopen8[Position] * (1 - Xmap) ) ) * Ymap );
	//  }

	//  if (Sector == 3) {
	//    SDrum = ( ( (sdrum5[Position] * ( 1 - Xmap ) ) + (sdrum6[Position] * Xmap ) ) * ( 1 - Ymap ) ) + ( ( (sdrum8[Position] * ( 1 - Xmap )) + (sdrum8[Position] * Xmap ) ) * Ymap );
	//    BDrum = ( ( (bdrum5[Position] * ( 1 - Xmap ) ) + (bdrum6[Position] * Xmap ) ) * ( 1 - Ymap ) ) + ( ( (bdrum8[Position] * ( 1 - Xmap )) + (bdrum8[Position] * Xmap ) ) * Ymap );
	//    HHats = ( ( (hhopen5[Position] * ( 1 - Xmap ) ) + (hhopen6[Position] * Xmap ) ) * ( 1 - Ymap ) ) + ( ( (hhopen8[Position] * ( 1 - Xmap )) + (hhopen8[Position] * Xmap ) ) * Ymap );
	//  }   

	//  if (Sector == 4) {
	//    SDrum = ( ( (sdrum2[Position] * ( 1 - Xmap ) ) + (sdrum3[Position] * Xmap ) ) * Ymap ) + ( ( (sdrum5[Position] * ( 1 - Xmap ) ) + (sdrum6[Position] * Xmap ) ) * ( 1 - Ymap) );
	//    BDrum = ( ( (bdrum2[Position] * ( 1 - Xmap ) ) + (bdrum3[Position] * Xmap ) ) * Ymap ) + ( ( (bdrum5[Position] * ( 1 - Xmap ) ) + (bdrum6[Position] * Xmap ) ) * ( 1 - Ymap) );
	//    HHats = ( ( (hhopen2[Position] * ( 1 - Xmap ) ) + (hhopen3[Position] * Xmap ) ) * Ymap ) + ( ( (hhopen5[Position] * ( 1 - Xmap ) ) + (hhopen6[Position] * Xmap ) ) * ( 1 - Ymap) );
	//  }

	// ///////////////////////////////// above are the equations for interpolating between values. Nothing fancy.

	//  if (BDrum > BDfreq) playNote(35+note, 127, 1000, 10); 
	//  if (SDrum > SDfreq) playNote(36+note, 127, 1000, 10); 
	//  if (HHats > HHfreq) playNote(37+note, 127, 1000, 10);         // sending midi notes

	} 

	void trace(byte cc, byte val){
		MIDI.sendControlChange(cc,val,16);
	}
