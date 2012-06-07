#include "ofxPGMidiContext.h"

#import "pgmidi/iOSVersionDetection.h"
#include "ofLog.h"

NSAutoreleasePool * ofxPGMidiContext::pool = nil;
PGMidi * ofxPGMidiContext::midi = nil;
ofxPGMidiDelegate * ofxPGMidiContext::delegate = nil;

// -----------------------------------------------------------------------------
void ofxPGMidiContext::setup() {
	if(midi != nil)
		return;
	IF_IOS_HAS_COREMIDI (
		pool = [[NSAutoreleasePool alloc] init];
		midi = [[PGMidi alloc] init];
		delegate = [[ofxPGMidiDelegate alloc] init];
		midi.delegate = delegate;
	)
}

// -----------------------------------------------------------------------------
PGMidi * ofxPGMidiContext::getMidi() {
	setup();
	return midi;
}

// -----------------------------------------------------------------------------
void ofxPGMidiContext::setConnectionListener(ofxMidiConnectionListener * listener) {
	[delegate setListenerPtr:(void*) listener];
}

// -----------------------------------------------------------------------------
void ofxPGMidiContext::clearConnectionListener() {
	[delegate setListenerPtr:NULL];
}

// -----------------------------------------------------------------------------
void ofxPGMidiContext::enableNetwork() {
	[midi enableNetwork:YES];
	ofLog(OF_LOG_VERBOSE, "ofxMidi: iOS Midi Networking enabled");
}
