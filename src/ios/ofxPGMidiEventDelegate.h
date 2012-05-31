#pragma once

#import "PGMidi.h"
//#include <vector>

@class PGMidi;
@class PGMidiSource;
@class PGMidiSourceDelegate;

//class ofxPGMidiIn;

/// interface to global pgmidi events
@interface ofxPGMidiEventDelegate : NSObject <PGMidiDelegate, PGMidiSourceDelegate> {
    PGMidi * midi;
//	ofxPGMidiIn * input;
//	unsigned long long lastTime;
//	bool firstPacket;
//	std::vector<unsigned char> message;
}

/// pgmidi callbacks
//- (void) midiSource:(PGMidiSource *)input midiReceived:(const MIDIPacketList *)packetList;

- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source;
- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source;
- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination;
- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination;

/// recieve from all sources
- (void) attachToAllExistingSources;

/// set the global pgmidi pointer
- (void) setMidiPtr:(PGMidi *)m;

///// set the pointer to the ofxPGMidiIn object to send messages to
//- (void) setInputPtr:(void *)i;
//
//// print the available sources
//- (void) listSources;
//
//// print the available destinations
//- (void) listDestinations;

@property (nonatomic,strong) PGMidi * midi;	///< pointer to global midi instance

@end
