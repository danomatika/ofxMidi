#pragma once

#include "portmidi.h"

/// static class to manage the port midi context
class ofxPortMidiContext {

public:

	static void init() {
		if(!bInited) {
			PmError error;
			error = Pm_Initialize();
			if(error != pmNoError) {
				ofLog(OF_LOG_ERROR, "ofxMidi: couldn't init portmidi: %s",
					Pm_GetErrorText(error));
			}
			else {
			
				// count inputs & outputs list of both i/o devs
				int numDev = Pm_CountDevices();
				for(int i = 0; i < numDev; ++i) {
					const PmDeviceInfo *devInfo = Pm_GetDeviceInfo(i);
					if(devInfo->input)
						nInputs++;
					if(devInfo->output)
						nOutputs++;
				}
				
				bInited = true;
			}
		}
		references++;
	}
	
	static void terminate() {
		references--;
		if(bInited && references == 0) {
			PmError error;
			error = Pm_Terminate();
			if(error != pmNoError) {
				ofLog(OF_LOG_ERROR, "ofxMidi: couldn't terminate portmidi: %s",
					Pm_GetErrorText(error));
			}
			else {
				bInited = false;
			}
		}
	}
	
	/// portmidi lists both inputs and outputs together, use these to calc
	/// individual indexes/numbers separately
	static unsigned int numInputs() {return nInputs;}
	static unsigned int numOutputs() {return nOutputs;}

private:
	
	static bool bInited;
	static unsigned int references;
	static unsigned int nInputs, nOutputs;
};

bool ofxPortMidiContext::bInited = false;
unsigned int ofxPortMidiContext::references = 0;
unsigned int ofxPortMidiContext::nInputs = 0;
unsigned int ofxPortMidiContext::nOutputs = 0;
