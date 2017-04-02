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
#include "ofxMidi.h"

// choose the midi backend
#ifdef TARGET_OF_IPHONE
	#include "ios/ofxPGMidiIn.h"
#endif


// -----------------------------------------------------------------------------
void ofxMidiConnectionListener::midiInputAdded(string name, bool isNetwork) {
	ofLogNotice("ofxMidi") << "input added: " << name << " network: " << isNetwork;
}

void ofxMidiConnectionListener::midiInputRemoved(string name, bool isNetwork) {
	ofLogNotice("ofxMidi") << "input removed: " << name << " network: " << isNetwork;
}

void ofxMidiConnectionListener::midiOutputAdded(string name, bool isNetwork) {
	ofLogNotice("ofxMidi") << "output added: " << name << " network: " << isNetwork;
}

void ofxMidiConnectionListener::midiOutputRemoved(string name, bool isNetwork) {
	ofLogNotice("ofxMidi") << "output removed: " << name << " network: " << isNetwork;
}

// -----------------------------------------------------------------------------
// from Pure Data x_acoustics.c
float ofxMidi::mtof(float note) {
	if(note <= -1500) return (0);
	else if(note > 1499) return (mtof(1499));
	else return (8.17579891564 * exp(.0577622650 * note));
}

// from Pure Data x_acoustics.c
float ofxMidi::ftom(float frequency) {
	return (frequency > 0 ? 17.3123405046 * log(.12231220585 * frequency) : -1500);
}

// -----------------------------------------------------------------------------
void ofxMidi::setConnectionListener(ofxMidiConnectionListener *listener) {
	#ifdef TARGET_OF_IPHONE
		ofxPGMidiIn::setConnectionListener(listener);
	#endif
}

// -----------------------------------------------------------------------------
void ofxMidi::clearConnectionListener() {
	#ifdef TARGET_OF_IPHONE
		ofxPGMidiIn::clearConnectionListener();
	#endif
}

// -----------------------------------------------------------------------------
void ofxMidi::enableNetworking() {
	#ifdef TARGET_OF_IPHONE
		ofxPGMidiIn::enableNetworking();
	#endif
}
