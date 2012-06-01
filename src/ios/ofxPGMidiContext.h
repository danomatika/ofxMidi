#pragma once

#import "PGMidi.h"
#import "pgmidi/iOSVersionDetection.h"

/// global static wrapper around PGMidi Obj-C instance
class ofxPGMidiContext {

	public:
	
		static void setup() {
			if(isSetup())
				return;
//			IF_IOS_HAS_COREMIDI
//			(
				midi = [[PGMidi alloc] init];
//			)
		}
	
		static bool isSetup() {return midi != NULL;}
		static PGMidi * getMidi() {return midi;}

//		// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
//		static void listSources() {
//			int count = [midi.sources count];
//			cout << "ofxMidiIn: " << count << " ports available" << endl;
//			//for(PGMidiSource * source in midi.sources) {
//			for(NSUInteger i = 0; i < count; ++i) {
//					PGMidiSource * source = [midi.sources objectAtIndex:i];
//					cout << "ofxMidiIn: " << i << ": " << [source.name UTF8String] << endl;
//			}
//		}
//		
//		// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
//		static void listDestinations() {
//			int count = [midi.destinations count]; 
//			cout << "ofxMidiOut: " << count << " ports available" << endl;
//			//for(PGMidiDestination * dest in midi.destinations) {
//			for(NSUInteger i = 0; i < count; ++i) {
//					PGMidiDestination * dest = [midi.destinations objectAtIndex:i];
//				cout << "ofxMidiOut: " << i << ": " << [dest.name UTF8String] << endl;
//			}
//		}
		
		static void enableNetwork(bool network) {
			if(network)
				[midi enableNetwork:YES];
			else
				[midi enableNetwork:NO];
		}

	private:

		static PGMidi * midi;
};
