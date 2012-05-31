#pragma once

#import "PGMidi.h"
#include <vector>

@class PGMidi;
@class PGMidiSource;
@class PGMidiSourceDelegate;

class ofxPGMidiIn;

/// interface to pgmidi input
@interface ofxPGMidiInDelegate : NSObject <PGMidiDelegate, PGMidiSourceDelegate> {
	ofxPGMidiIn * input;
	bool bIgnoreSysex, bIgnoreTiming, bIgnoreSense;	///< ignore midi types?
	unsigned long long lastTime;	///< timestamp form last packet
	bool firstPacket;	///< is this the first received packet?
	std::vector<unsigned char> message;	///< raw byte buffer
}

/// pgmidi callback
- (void) midiSource:(PGMidiSource *)input midiReceived:(const MIDIPacketList *)packetList;

/// set the pointer to the ofxPGMidiIn object to send messages to
- (void) setInputPtr:(void *)i;

@property bool bIgnoreSysex;
@property bool bIgnoreTiming;
@property bool bIgnoreSense;

@end
