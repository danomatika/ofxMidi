#include "ofxPGMidiIn.h"

#import "pgmidi/iOSVersionDetection.h"
#import <CoreMIDI/CoreMIDI.h>

#include "ofxMidiIn.h"

- (void) midiSource:(PGMidiSource*)midi midiReceived:(const MIDIPacketList *)packetList {
    const MIDIPacket * packet = & packetList->packet[0];
    for (int i = 0; i < packetList->numPackets; ++i) {
        ofxPGMidiIn::
        packet = MIDIPacketNext(packet);
    }
}


// --------------------------------------------------------------------------------------
ofxPGMidiIn::ofxPGMidiIn) {
	PGMidi = [[PGMidi alloc] init];
}

// --------------------------------------------------------------------------------------
ofxPGMidiIn::~ofxPGMidiIn() {
	[midi release];
}

// --------------------------------------------------------------------------------------
void ofxPGMidiIn::listInputPorts() {
	cout << "ofxPGMidiIn: " << midiin.getPortCount() << " ports available" << endl;
	for(unsigned int i = 0; i < midiin.getPortCount(); ++i){
		cout << "ofxPGMidiIn: " <<  i << ": " << midiin.getPortName(i) << endl;
	}
}

// --------------------------------------------------------------------------------------
vector<string>& ofxPGMidiIn::getPortList() {
	portList.clear();
	for(unsigned int i=0; i < midiin.getPortCount(); ++i) {
		portList.push_back(midiin.getPortName(i));
	}
	return portList;
}

// --------------------------------------------------------------------------------------
int ofxPGMidiIn::getNumPorts() {
	return [midi.sources count]
}

// --------------------------------------------------------------------------------------
string ofxPGMidiIn::getPortName(unsigned int portNumber) {
	// handle rtmidi exceptions
	@try {
		return (string) [midi.sources objectAtIndex:portNumber].name;
	}
	@catch(NSException * ex) {
		ofLog(OF_LOG_ERROR, "ofxPGMidiIn: couldn't get name for port %i: %s: %s",
			portNumber, ex.name, ex.reason);
	}
	return "";
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiIn::openPort(unsigned int portNumber) {	
	// handle rtmidi exceptions
	try {
		closePort();
		midiin.setCallback(&_midiMessageCallback, this);
		midiin.openPort(portNumber);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxPGMidiIn: couldn't open port %i: %s", portNumber, err.what());
		return false;
	}
	portNum = portNumber;
	portName = midiin.getPortName(portNumber);
	bOpen = true;
	ofLog(OF_LOG_VERBOSE, "ofxPGMidiIn: opened port %i %s",
		portNum, portName.c_str());
	return true;
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiIn::openPort(string deviceName) {
	
	// iterate through MIDI ports, find requested device
	int port = -1;
	for(unsigned int i = 0; i < midiin.getPortCount(); ++i) {
		string name = midiin.getPortName(i);
		if(name == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLog(OF_LOG_ERROR, "ofxPGMidiIn: port \"%s\" is not available", deviceName.c_str());
		return false;
	} 
	
	return openPort(port);
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiIn::openVirtualPort(string portName) {
	// handle rtmidi exceptions
	try {
		closePort();
		midiin.setCallback(&_midiMessageCallback, this);
		midiin.openVirtualPort(portName);
	}
	catch(RtError& err) {
		ofLog(OF_LOG_ERROR, "ofxPGMidiIn: couldn't open virtual port \"%s\": %s",
			portName.c_str(), err.what());
		return false;
	}
	this->portName = portName;
	bOpen = true;
	bVirtual = true;
	ofLog(OF_LOG_VERBOSE, "ofxMidiOut: opened virtual port %s", portName.c_str());
	return true;
}

// --------------------------------------------------------------------------------------
void ofxPGMidiIn::closePort() {
	if(bVirtual && bOpen) {
		ofLog(OF_LOG_VERBOSE, "ofxPGMidiIn: closing virtual port %s", portName.c_str());
	}
	else if(portNum > -1) {
		ofLog(OF_LOG_VERBOSE, "ofxPGMidiIn: closing port %i %s", portNum, portName.c_str());
	}
	midiin.closePort();
	if(bOpen)
		midiin.cancelCallback();
	portNum = -1;
	portName = "";
	bOpen = false;
	bVirtual = false;
}

// --------------------------------------------------------------------------------------
int ofxPGMidiIn::getPort() {
	return portNum;
}

// --------------------------------------------------------------------------------------
string ofxPGMidiIn::getName() {
	return portName;
}

// --------------------------------------------------------------------------------------
bool ofxPGMidiIn::isOpen() {
	return bOpen;
}

// --------------------------------------------------------------------------------------
void ofxPGMidiIn::ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense) {
	midiin.ignoreTypes(midiSysex, midiTiming, midiSense);
	ofLog(OF_LOG_VERBOSE, "ofxPGMidiIn: ignore types on %s: sysex: %d timing: %d sense: %d",
			portName.c_str(), midiSysex, midiTiming, midiSense);
}

// --------------------------------------------------------------------------------------
void ofxPGMidiIn::addListener(ofxMidiListener* listener) {
	ofAddListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

// --------------------------------------------------------------------------------------
void ofxPGMidiIn::removeListener(ofxMidiListener* listener) {
	ofRemoveListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

// --------------------------------------------------------------------------------------
void ofxPGMidiIn::setVerbose(bool verbose) {
	bVerbose = verbose;
}

// PRIVATE
// --------------------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice, etc?
void ofxPGMidiIn::manageNewMessage(double deltatime, vector<unsigned char> *message) {
			
	// parse message and fill event
	ofxMidiMessage midiMessage(message);
	
	midiMessage.status = (MidiStatus) (message->at(0) & 0xF0);
	midiMessage.channel = (int) (message->at(0) & 0x0F)+1;
	midiMessage.deltatime = deltatime * 1000; // convert s to ms
	midiMessage.portNum = portNum;
	midiMessage.portName = portName;
	
	switch(midiMessage.status) {
		case MIDI_NOTE_ON :
		case MIDI_NOTE_OFF:
			midiMessage.pitch = (int) message->at(1);
			midiMessage.velocity = (int) message->at(2);
			break;
		case MIDI_CONTROL_CHANGE:
			midiMessage.control = (int) message->at(1);
			midiMessage.value = (int) message->at(2);
			break;
		case MIDI_PROGRAM_CHANGE:
		case MIDI_AFTERTOUCH:
			midiMessage.value = (int) message->at(1);
			break;
		case MIDI_PITCH_BEND:
			midiMessage.value = (int) (message->at(2) << 7) +
								(int) message->at(1); // msb + lsb
			break;
		case MIDI_POLY_AFTERTOUCH:
			midiMessage.pitch = (int) message->at(1);
			midiMessage.value = (int) message->at(2);
			break;
		default:
			break;
	}
	
	if(bVerbose) {
		cout << midiMessage.toString() << endl;
	}
	
	// send event to listeners
	ofNotifyEvent(newMessageEvent, midiMessage, this);
}

// --------------------------------------------------------------------------------------
void ofxPGMidiIn::_midiMessageCallback(double deltatime, vector<unsigned char> *message, void *userData) {
	((ofxPGMidiIn*) userData)->manageNewMessage(deltatime, message);
}
