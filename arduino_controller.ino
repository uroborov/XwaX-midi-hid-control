/*
Implements MIDI controller for standalone xwax on raspberry

Written by Emanuele Virgillito
Uses MIDI_controller library 3.1.0 written by tttapa
https://github.com/tttapa/MIDI_controller
*/

#define USE_ROTARY_ENCODER
#include <MIDI_Controller.h>

const uint8_t velocity = 0b1111111; // Maximum velocity (0b1111111 = 0x7F = 127)

const uint8_t FL = 0x40;
const uint8_t FL_pin = 15;

const uint8_t TOGGLE_pin = 9;

const uint8_t BYPASS= 0x1D;
const uint8_t BYPASS_pin= 7;
const uint8_t CUE = 0x24;
const uint8_t CUE_pin = 6;
const uint8_t LOAD = 0x16;
const uint8_t LOAD_pin = 5;

const uint8_t RESCAN = 0x1E;
const uint8_t RESCAN_pin = 8;

const uint8_t ENC_PIN_A = 14;
const uint8_t ENC_PIN_B = 16;

const uint8_t Channel = 1;       // MIDI channel 1
const uint8_t Controller = 0x14; // MIDI Control Change controller number
const int speedMultiply = 1;     // No change in speed of the encoder (number of steps is multiplied by 1)

Digital buttons[] = {
	{LOAD_pin, LOAD, velocity},
	{CUE_pin, CUE, velocity},
	{BYPASS_pin, BYPASS, velocity},
	{RESCAN_pin, RESCAN, velocity},
	{FL_pin, FL, velocity},
};

RotaryEncoder encoder(ENC_PIN_A, ENC_PIN_B, Controller, Channel, speedMultiply, NORMAL_ENCODER, BINARY_OFFSET);

Bank bank(1);
BankSelector bankSelector(bank, TOGGLE_pin, BankSelector::MOMENTARY);


void setup() {
  bank.add(encoder, Bank::CHANGE_CHANNEL);
  }

void loop() {
  // Refresh the button (check whether the button's state has changed since last time, if so, send it over MIDI)
  MIDI_Controller.refresh();
}
