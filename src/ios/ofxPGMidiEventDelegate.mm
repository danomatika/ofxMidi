#include "ofxPGMidiEventDelegate.h"

#include "ofMain.h"

#import "pgmidi/iOSVersionDetection.h"
#include <mach/mach_time.h>
//#include "ofxPGMidiIn.h"

////--------------------------------------------------------------
//// from https://developer.apple.com/library/mac/#qa/qa1398/_index.html
//uint64_t AbsoluteToNanos(uint64_t time) {
//	static struct mach_timebase_info timebaseInfo;
//
//	// only init once
//	if(timebaseInfo.denom == 0) {
//		mach_timebase_info(&timebaseInfo);
//	}
//
//	return time * timebaseInfo.numer / timebaseInfo.denom;
//}

//--------------------------------------------------------------
@implementation ofxPGMidiEventDelegate

@synthesize midi;

- (id) init {
	self = [super init];
//	lastTime = 0;
//	firstPacket = false;
	return self;
}

//// -----------------------------------------------------------------------------
//// adapted from RTMidi CoreMidi message parsing
//- (void) midiSource:(PGMidiSource*)midi midiReceived:(const MIDIPacketList *)packetList {
//
//    const MIDIPacket *packet = &packetList->packet[0];
//	stringstream msg;
//	unsigned long long time;
//	double delta;
//	
//    for(int i = 0; i < packetList->numPackets; ++i) {
//       
//		message.clear();
//	   
//		// calc time stamp
//		time = 0;
//		if(firstPacket) {
//			firstPacket = false;
//		}
//		else {
//			time = packet->timeStamp;
//			if(time == 0) { // this happens when receiving asynchronous sysex messages
//				time = mach_absolute_time();
//			}
//			time -= lastTime;
//			delta = AbsoluteToNanos(time) * 0.000001; // convert to ms
//		}
//		lastTime = packet->timeStamp;
//		if(lastTime == 0 ) { // this happens when receiving asynchronous sysex messages
//		  lastTime = mach_absolute_time();
//		}
//	   
//		msg << "packet: " << packet->length << " [ ";
//		for(int b = 0; b < packet->length; ++b) {
//			message.push_back(packet->data[b]);
//			msg << std::hex << std::uppercase << std::setw(2) << std::setfill('0')
//				<< (int) packet->data[b] << " " 
//				<< std::dec << std::nouppercase << std::setw(1) << std::setfill(' ');
//		}
//		msg << "] " << delta;
//		//app->addMessage(msg.str());
//		input->manageNewMessage(delta, &message);
//		msg.str("");
//		
//		packet = MIDIPacketNext(packet);
//    }
//}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source {
	//source.delegate = self;
	stringstream msg;
	msg << "source added: " << [source.name UTF8String];
	//app->addMessage(msg.str());
	cout << msg << endl;
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source {
	//source.delegate = nil;
	stringstream msg;
	msg << "source removed: " << [source.name UTF8String];
	//app->addMessage(msg.str());
	cout << msg << endl;
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination {
	stringstream msg;
	msg << "dest added: " << [destination.name UTF8String];
	//app->addMessage(msg.str());
	cout << msg << endl;
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination {
	stringstream msg;
	msg << "dest removed: " << [destination.name UTF8String];
	cout << msg << endl;
	//input->addMessage(msg.str());
}

//// -----------------------------------------------------------------------------
//- (void) attachToAllExistingSources {
//    for(PGMidiSource * source in midi.sources) {
//        source.delegate = self;
//    }
//}

// -----------------------------------------------------------------------------
- (void) setMidiPtr:(PGMidi *)m {
    midi.delegate = nil;
    midi = m;
    midi.delegate = self;
    //[self attachToAllExistingSources];
}

//// -----------------------------------------------------------------------------
//- (void) setInputPtr:(void *)i {
//	input = (ofxPGMidiIn*) i;
//}
//
//// -----------------------------------------------------------------------------
//- (void) listSources {
//	cout << "Sources" << endl;
//    for(PGMidiSource * source in midi.sources) {
//        cout << "\t" << [source.name UTF8String] << endl;
//    }
//}
//
//// -----------------------------------------------------------------------------
//- (void) listDestinations {
//	cout << "Destinations" << endl;
//    for(PGMidiDestination * dest in midi.destinations) {
//        cout << "\t" << [dest.name UTF8String] << endl;
//    }
//}

@end
