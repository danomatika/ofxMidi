#include "ofxPGMidiIn.h"

#import "ofxPGMidiContext.h"
#import "ofxPGMidiSourceDelegate.h"

// PIMPL wrapper from http://stackoverflow.com/questions/7132755/wrapping-objective-c-in-objective-c-c
struct ofxPGMidiIn::InputDelegate {
	ofxPGMidiSourceDelegate * d; ///< Obj-C input delegate
};

// -----------------------------------------------------------------------------
ofxPGMidiIn::ofxPGMidiIn(const string name) : ofxBaseMidiIn(name) {
	
	// setup global midi instance
	ofxPGMidiContext::setup();
	
	// setup Obj-C interface to PGMidi
	inputDelegate = new InputDelegate;
	inputDelegate->d = [[ofxPGMidiSourceDelegate alloc] init];
	[inputDelegate->d setInputPtr:(void*) this];
}

// -----------------------------------------------------------------------------
ofxPGMidiIn::~ofxPGMidiIn() {
	[inputDelegate->d release];
	delete inputDelegate;
}

// -----------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
void ofxPGMidiIn::listPorts() {
	PGMidi * midi = ofxPGMidiContext::getMidi();
	int count = [midi.sources count];
	cout << "ofxMidiIn: " << count << " ports available" << endl;
	for(NSUInteger i = 0; i < count; ++i) {
			PGMidiSource * source = [midi.sources objectAtIndex:i];
			cout << "ofxMidiIn: " << i << ": " << [source.name UTF8String] << endl;
	}
}

// -----------------------------------------------------------------------------
vector<string>& ofxPGMidiIn::getPortList() {
	PGMidi * midi = ofxPGMidiContext::getMidi();
	portList.clear();
	for(PGMidiSource * source in midi.sources) {
		portList.push_back([source.name UTF8String]);
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxPGMidiIn::getNumPorts() {
	return [ofxPGMidiContext::getMidi().sources count];
}

// -----------------------------------------------------------------------------
string ofxPGMidiIn::getPortName(unsigned int portNumber) {
	
	PGMidi * midi = ofxPGMidiContext::getMidi();
	
	// handle OBJ-C exceptions
	@try {
		PGMidiSource * source = [midi.sources objectAtIndex:portNumber]; 
		return [source.name UTF8String];
	}
	@catch(NSException * ex) {
		ofLog(OF_LOG_ERROR, "ofxMidiIn: couldn't get name for port %i: %s: %s",
			portNumber, [ex.name UTF8String], [ex.reason UTF8String]);
	}
	return "";
}

// -----------------------------------------------------------------------------
bool ofxPGMidiIn::openPort(unsigned int portNumber) {	

	PGMidi * midi = ofxPGMidiContext::getMidi();
	PGMidiSource * source = nil;
	
	// handle OBJ-C exceptions
	@try {
		source = [midi.sources objectAtIndex:portNumber]; 
	}
	@catch(NSException * ex) {
		ofLog(OF_LOG_ERROR, "ofxMidiIn: couldn't open port %i: %s: %s",
			portNumber, [ex.name UTF8String], [ex.reason UTF8String]);
		return false;
	}
	source.delegate = inputDelegate->d;
	portNum = portNumber;
	portName = [source.name UTF8String];
	bOpen = true;
	ofLog(OF_LOG_VERBOSE, "ofxMidiIn: opened port %i %s",
		portNum, portName.c_str());
	return true;
}

// -----------------------------------------------------------------------------
bool ofxPGMidiIn::openPort(string deviceName) {

	PGMidi * midi = ofxPGMidiContext::getMidi();

	// iterate through MIDI ports, find requested device
	int port = -1;
	for(NSUInteger i = 0; i < [midi.sources count]; ++i) {
		PGMidiSource * source = [midi.sources objectAtIndex:i];
		if([source.name UTF8String] == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLog(OF_LOG_ERROR, "ofxMidiIn: port \"%s\" is not available", deviceName.c_str());
		return false;
	} 
	
	return openPort(port);
}

// -----------------------------------------------------------------------------
bool ofxPGMidiIn::openVirtualPort(string portName) {
	ofLog(OF_LOG_WARNING, "ofxMidiIn: couldn't open virtual port \"%s\"", portName.c_str());
	ofLog(OF_LOG_WARNING, "ofxMidiIn: virtual ports are currently not supported on iOS");
	return false;
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::closePort() {
	
	if(bOpen) {
		ofLog(OF_LOG_VERBOSE, "ofxMidiIn: closing port %i %s", portNum, portName.c_str());
	
		PGMidi * midi = ofxPGMidiContext::getMidi();
		PGMidiSource * source = [midi.sources objectAtIndex:portNum];
		source.delegate = nil;
	}
	
	portNum = -1;
	portName = "";
	bOpen = false;
	bVirtual = false;
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::ignoreTypes(bool midiSysex, bool midiTiming, bool midiSense) {
	
	inputDelegate->d.bIgnoreSysex = midiSysex;
	inputDelegate->d.bIgnoreTiming = midiTiming;
	inputDelegate->d.bIgnoreSense = midiSense;
	
	ofLog(OF_LOG_VERBOSE, "ofxPGMidiIn: ignore types on %s: sysex: %d timing: %d sense: %d",
			portName.c_str(), midiSysex, midiTiming, midiSense);
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::messageReceived(double deltatime, vector<unsigned char> *message) {
	manageNewMessage(deltatime, message);
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::setConnectionListener(ofxMidiConnectionListener * listener) {
	ofxPGMidiContext::setConnectionListener(listener);
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::clearConnectionListener() {
	ofxPGMidiContext::clearConnectionListener();
}

// -----------------------------------------------------------------------------
void ofxPGMidiIn::enableNetworking() {
	ofxPGMidiContext::enableNetwork();
}
