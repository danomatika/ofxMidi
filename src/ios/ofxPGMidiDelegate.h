#pragma once

#import "PGMidi.h"

class ofxMidiConnectionListener;

/// interface to global pgmidi events
@interface ofxPGMidiDelegate : NSObject <PGMidiDelegate> {	
	ofxMidiConnectionListener * listenerPtr; ///< object to send receieved events to
}

- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source;
- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source;
- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination;
- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination;

/// set the pointer to the ofxMidiConnectionListener object to send messages to
- (void) setListenerPtr:(void *)p;

@end
