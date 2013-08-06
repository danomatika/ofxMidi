/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#include "ofxMidiMessage.h"

// -----------------------------------------------------------------------------
ofxMidiMessage::ofxMidiMessage() {
	clear();
}

// -----------------------------------------------------------------------------
ofxMidiMessage::ofxMidiMessage(vector<unsigned char>* rawBytes) {
	clear();
	for(unsigned int i = 0; i < rawBytes->size(); ++i)
		bytes.push_back(rawBytes->at(i));
}

// -----------------------------------------------------------------------------
ofxMidiMessage::ofxMidiMessage(const ofxMidiMessage& from) {
	status = from.status;
	channel = from.channel;
	pitch = from.pitch;
	velocity = from.velocity;
	control = from.control;
	value = from.value;
	bytes = from.bytes;
	deltatime = from.deltatime;
	portNum = from.portNum;
	portName = from.portName;

	bytes.clear();
	for(unsigned int i = 0; i < from.bytes.size(); ++i)
		bytes.push_back(from.bytes[i]);
}

// -----------------------------------------------------------------------------
ofxMidiMessage& ofxMidiMessage::operator=(const ofxMidiMessage& from) {
	status = from.status;
	channel = from.channel;
	pitch = from.pitch;
	velocity = from.velocity;
	control = from.control;
	value = from.value;
	bytes = from.bytes;
	deltatime = from.deltatime;
	portNum = from.portNum;
	portName = from.portName;
	
	bytes.clear();
	for(unsigned int i = 0; i < from.bytes.size(); ++i)
		bytes.push_back(from.bytes[i]);
	return *this;
}

// -----------------------------------------------------------------------------
void ofxMidiMessage::clear() {
	status = MIDI_UNKNOWN;
	channel = 0;
	pitch = 0;
	velocity = 0;
	control = 0;
	value = 0;
	deltatime = 0;
	portNum = -1;
	portName = "";
	bytes.clear();
}

// -----------------------------------------------------------------------------
string ofxMidiMessage::toString() {
	stringstream stream;
	stream << portName << ": " << getStatusString(status) << " "
		   << channel << " [ ";
	for(unsigned int i = 0; i < bytes.size(); ++i) {
		stream << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
			   << (int) bytes[i] << " "
			   << std::dec << std::nouppercase << std::setw(1) << std::setfill(' ');
	}
	stream << "] " << deltatime;
	return stream.str();
}

// -----------------------------------------------------------------------------
string ofxMidiMessage::getStatusString(MidiStatus status) {
	switch(status) {
		case MIDI_NOTE_OFF:
			return "Note Off";
		case MIDI_NOTE_ON:
			return "Note On";
		case MIDI_CONTROL_CHANGE:
			return "Control Change";
		case MIDI_PROGRAM_CHANGE:
			return "Program Change";
		case MIDI_PITCH_BEND:
			return "Pitch Bend";
		case MIDI_AFTERTOUCH:
			return "Aftertouch";
		case MIDI_POLY_AFTERTOUCH:
			return "Poly Aftertouch";
		case MIDI_SYSEX:
			return "Sysex";
		case MIDI_TIME_CODE:
			return "Time Code";
		case MIDI_SONG_POS_POINTER:
			return "Song Pos";
		case MIDI_SONG_SELECT:
			return "Song Select";
		case MIDI_TUNE_REQUEST:
			return "Tune Request";
		case MIDI_SYSEX_END:
			return "Sysex End";
		case MIDI_TIME_CLOCK:
			return "Time Clock";
		case MIDI_START:
			return "Start";
		case MIDI_CONTINUE:
			return "Continue";
		case MIDI_STOP:
			return "Stop";
		case MIDI_ACTIVE_SENSING:
			return "Active Sensing";
		case MIDI_SYSTEM_RESET:
			return "System Reset";
		default:
			return "Unknown";
	}
}
