/*
This is an example of the "Digital" class of the MIDI_controller library.
Connect a push buttons to digital pin 2. Connect the other pin of the button to the ground, 
a pull-up resistor is not necessary, because the internal one will be used. 
This button will play MIDI note C4 when pressed.
Map it in your DAW or DJ software.

Written by tttapa, 08/09/2017
https://github.com/tttapa/MIDI_controller
*/

#define USE_ROTARY_ENCODER
#include <MIDI_Controller.h>

const uint8_t velocity = 0b1111111; // Maximum velocity (0b1111111 = 0x7F = 127)

const uint8_t FL = 0x40;
const uint8_t FL_pin = 15;

const uint8_t TOGGLE_ = 0x45;
const uint8_t TOGGLE_pin = 9;

const uint8_t BYPASS= 0x1D;
const uint8_t BYPASS_pin= 7;
const uint8_t CUE = 0x24;
const uint8_t CUE_pin = 6;
const uint8_t LOAD = 0x16;
const uint8_t LOAD_pin = 5;

const uint8_t RESCAN = 0x1E;
const uint8_t RESCAN_pin = 8;

const uint8_t Channel = 1;       // MIDI channel 1
const uint8_t Controller = 0x14; // MIDI Control Change controller number
const int speedMultiply = 1;     // No change in speed of the encoder (number of steps is multiplied by 1)

// Create a new instance of the class 'Digital', called 'button', on pin 2, that sends MIDI messages with note 'C4' (60) on channel 1, with velocity 127
Digital buttons[] = {
  {LOAD_pin, LOAD, velocity},
  {CUE_pin, CUE, velocity},
  {BYPASS_pin, BYPASS, velocity},
  {RESCAN_pin, RESCAN, velocity},
	{FL_pin, FL, velocity},
};


// Create a new instance of the class 'RotaryEncoder', called 'enc', on pin 2 and 3, controller number 0x14, on channel1,
// no change in speed (speed is multiplied by 1), it's used as a jog wheel, and the sign mode is set to two's complement.
RotaryEncoder encoder(14, 16, Controller, Channel, speedMultiply, NORMAL_ENCODER, BINARY_OFFSET);


Bank bank(1);
BankSelector bankSelector(bank, TOGGLE_pin, BankSelector::MOMENTARY);


void setup() {
  bank.add(encoder, Bank::CHANGE_CHANNEL);
  }

void loop() {
  // Refresh the button (check whether the button's state has changed since last time, if so, send it over MIDI)
  MIDI_Controller.refresh();
}
