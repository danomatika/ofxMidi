#pragma once

#include "portmidi.h"
#include "porttime.h"

/// static class to manage the port midi context
class ofxPortMidiContext {

public:
	
	/// inits portmidi if not inited and increases ref count
	static void init();
	
	/// decreases ref count and clears portmidi on last ref
	static void terminate();
	
	/// get the current midi timestamp in ms
	static unsigned int getTime();
	
	/// portmidi lists both inputs and outputs together, use these to calc
	/// individual indexes/numbers separately
	static unsigned int numInputs() {return nInputs;}
	static unsigned int numOutputs() {return nOutputs;}

private:
	
	static void _timer(PtTimestamp timestamp, void *userData);
	
	static bool bInited;
	static unsigned int references;
	static unsigned int nInputs, nOutputs;
};
