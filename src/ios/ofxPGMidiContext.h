#pragma once

#import "PGMidi.h"
#import "pgmidi/iOSVersionDetection.h"

/// global static wrapper around PGMidi Obj-C instance
class ofxPGMidiContext {

	public:
	
		/// creates the PGMidi instance if not already existing
		static void setup();
		
		/// get the PGMidi instance
		static PGMidi * getMidi();
		
		/// enable the iOS CoreMidi network interface? 
		static void enableNetwork(bool network);
	
	private:

		static PGMidi * midi;	///< global Obj-C PGMidi instance
};
