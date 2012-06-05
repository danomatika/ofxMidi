#include "ofxPGMidiContext.h"

#import "pgmidi/iOSVersionDetection.h"
#include "ofLog.h"

NSAutoreleasePool * ofxPGMidiContext::pool = nil;
PGMidi * ofxPGMidiContext::midi = nil;

void ofxPGMidiContext::setup() {
	if(midi != nil)
		return;
	IF_IOS_HAS_COREMIDI (
		pool = [[NSAutoreleasePool alloc] init];
		midi = [[PGMidi alloc] init];
		midi.delegate = [[ofxPGMidiDelegate alloc] init];
		enableNetwork(true);
	)
}

PGMidi * ofxPGMidiContext::getMidi() {
	return midi;
}
		
void ofxPGMidiContext::enableNetwork(bool network) {
	if(network) {
		[midi enableNetwork:YES];
		ofLog(OF_LOG_VERBOSE, "ofxMidi: iOS Midi Networking enabled");
	}
	else {
		[midi enableNetwork:NO];
		ofLog(OF_LOG_VERBOSE, "ofxMidi: iOS Midi Networking enabled");
	}
}
