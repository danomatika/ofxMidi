#include "ofxPGMidiInDelegate.h"

#include "ofMain.h"

#import "pgmidi/iOSVersionDetection.h"
#include <mach/mach_time.h>
#include "ofxPGMidiIn.h"

// -----------------------------------------------------------------------------
// there is no conversion fucntion on iOS, so we make one here
// from https://developer.apple.com/library/mac/#qa/qa1398/_index.html
uint64_t AbsoluteToNanos(uint64_t time) {
	static struct mach_timebase_info timebaseInfo;

	// only init once
	if(timebaseInfo.denom == 0) {
		mach_timebase_info(&timebaseInfo);
	}

	return time * timebaseInfo.numer / timebaseInfo.denom;
}

// PG MIDI IN DELEGATE
// -----------------------------------------------------------------------------
@implementation ofxPGMidiInDelegate

@synthesize bIgnoreSysex, bIgnoreTiming, bIgnoreSense;

// -----------------------------------------------------------------------------
- (id) init {
	self = [super init];
	inputPtr = NULL;
	lastTime = 0;
	firstPacket = false;
	return self;
}

// -----------------------------------------------------------------------------
// adapted from RTMidi CoreMidi message parsing
- (void) midiSource:(PGMidiSource *)input midiReceived:(const MIDIPacketList *)packetList {

    const MIDIPacket * packet = &packetList->packet[0];
	stringstream msg;
	unsigned long long time;
	double delta;
	
    for(int i = 0; i < packetList->numPackets; ++i) {
       
		message.clear();
	   
		// calc time stamp
		time = 0;
		if(firstPacket) {
			firstPacket = false;
		}
		else {
			time = packet->timeStamp;
			if(time == 0) { // this happens when receiving asynchronous sysex messages
				time = mach_absolute_time();
			}
			time -= lastTime;
			delta = AbsoluteToNanos(time) * 0.000001; // convert to ms
		}
		lastTime = packet->timeStamp;
		if(lastTime == 0 ) { // this happens when receiving asynchronous sysex messages
		  lastTime = mach_absolute_time();
		}
	   
		msg << "packet: " << packet->length << " [ ";
		for(int b = 0; b < packet->length; ++b) {
			message.push_back(packet->data[b]);
			msg << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
				<< (int) packet->data[b] << " " 
				<< std::dec << std::nouppercase << std::setw(1) << std::setfill(' ');
		}
		msg << "] " << delta;
		inputPtr->messageReceived(delta, &message);
		cout << msg;
		msg.str("");
		
		packet = MIDIPacketNext(packet);
    }
}

// -----------------------------------------------------------------------------
- (void) setInputPtr:(void *)i {
	inputPtr = (ofxPGMidiIn*) i;
}

@end
