#include "ofxPGMidiContext.h"

void ofxPGMidiContext::setup() {
	if(isSetup())
		return;
//			IF_IOS_HAS_COREMIDI
//			(
		midi = [[PGMidi alloc] init];
//			)
}
	
bool ofxPGMidiContext::isSetup() {return midi != nil;}

PGMidi * ofxPGMidiContext::getMidi() {return midi;}
		
void ofxPGMidiContext::enableNetwork(bool network) {
	if(network)
		[midi enableNetwork:YES];
	else
		[midi enableNetwork:NO];
}
