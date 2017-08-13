/*
 * Copyright (c) 2016 Sol Bekic <s+of at s-ol dot nu>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#include "ofxMidiMTCIn.h"

// -----------------------------------------------------------------------------
ofxMidiMTCIn::ofxMidiMTCIn(ofxMidiIn& midiIn) :
	midiIn(midiIn) {
	midiIn.addListener(this);
}

ofxMidiMTCIn::~ofxMidiMTCIn() {
	midiIn.removeListener(this);
}

void ofxMidiMTCIn::newMidiMessage(ofxMidiMessage& message) {
	switch (message.status) {
		case MIDI_START:
		case MIDI_CONTINUE:
			playing = true;
			break;
		case MIDI_STOP:
			playing = false;
			break;
		case MIDI_TIME_CODE: {
			unsigned char index = message.bytes[1] >> 4;
			unsigned char value = message.bytes[1] & 0x0F;

			if (index % 4 == 0) {
				frames++;
				if (frames >= framesPerSecond) {
					frames %= framesPerSecond;
					seconds++;
					if (seconds >= 60) {
						seconds %= 60;
						minutes++;
						if (minutes >= 60) {
							minutes %= 60;
							hours++;
						}
					}
				}
			}

			switch (index) {
				case 0x0: frames   = (frames  & 0xF0) | value; break;
				case 0x2: seconds  = (seconds & 0xF0) | value; break;
				case 0x4: minutes  = (minutes & 0xF0) | value; break;
				case 0x6: hours    = (hours   & 0xF0) | value; break;
				case 0x1: frames   = (frames  & 0x0F) | value << 4; break;
				case 0x3: seconds  = (seconds & 0x0F) | value << 4; break;
				case 0x5: minutes  = (minutes & 0x0F) | value << 4; break;
				case 0x7:
					hours = (hours & 0x0F) | value << 4;
					updateFramesPerSecond();
					break;
				default: break;
			}
			break;
		}
		case MIDI_SYSEX:
			if (message.bytes[0] == 0xf0 &&
					message.bytes[1] == 0x7f &&
					message.bytes[2] == 0x7f &&
					message.bytes[3] == 0x01 &&
					message.bytes[4] == 0x01 &&
					message.bytes[9] == 0xf7)
			{
				hours    = message.bytes[5];
				minutes  = message.bytes[6];
				seconds  = message.bytes[7];
				frames   = message.bytes[8];
				updateFramesPerSecond();
			}
		default: break;
	}
}

void ofxMidiMTCIn::updateFramesPerSecond() {
	switch (hours >> 5) {
		case 0: framesPerSecond = 24; break;
		case 1: framesPerSecond = 25; break;
		case 2:
			framesPerSecond = 30;
			ofLogError("ofxMidiMTCIn") << "SMPTE Type 2 (30fps, frame-drop) not supported!";
			break;
		case 3: framesPerSecond = 30; break;
		default:
			framesPerSecond = 100;
			ofLogError("ofxMidiMTCIn") << "Unknown SMPTE Type";
			break;
	}
	hours &= 0x1f;
	ready = true;
}

string ofxMidiMTCIn::toString(bool includeFPS) {
	if (!ready) return "Unknown";
	
	stringstream stream;
	stream << std::setw(2) << std::setfill('0')
	  << hours << ":" << minutes << ":" << seconds << ":" << frames;
	if (includeFPS) stream << " [" << framesPerSecond << "]";
	return stream.str();
}

float ofxMidiMTCIn::secondsSinceStart() {
	if (!ready) return 0;
	return hours * 60 * 60
		+ minutes * 60
		+ seconds
		+ frames / (float) framesPerSecond;
}
