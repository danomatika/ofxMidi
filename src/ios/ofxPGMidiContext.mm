#include "ofxPGMidiContext.h"

#include "ofLog.h"

PGMidi * ofxPGMidiContext::midi = nil;

void ofxPGMidiContext::setup() {
	if(midi != nil)
		return;
//			IF_IOS_HAS_COREMIDI
//			(
	midi = [[PGMidi alloc] init];
	enableNetwork(true);
//			)
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
