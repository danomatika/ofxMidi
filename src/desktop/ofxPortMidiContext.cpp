#include "ofxPortMidiContext.h"

#include "ofLog.h"

bool ofxPortMidiContext::bInited = false;
unsigned int ofxPortMidiContext::references = 0;
unsigned int ofxPortMidiContext::nInputs = 0;
unsigned int ofxPortMidiContext::nOutputs = 0;

// -----------------------------------------------------------------------------
void ofxPortMidiContext::init() {
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
			
			// start the porttime timer
			Pt_Start(1, NULL, NULL);//ofxPortMidiContext::_timer, NULL);
			
			if(Pt_Started()) {
				cout << "PT is started " << Pt_Started() << endl;
			}
			
			bInited = true;
		}
	}
	references++;
}

// -----------------------------------------------------------------------------
void ofxPortMidiContext::terminate() {
	references--;
	if(bInited && references == 0) {
		
		PmError error;
		error = Pm_Terminate(); // stops timer?
		if(error != pmNoError) {
			ofLog(OF_LOG_ERROR, "ofxMidi: couldn't terminate portmidi: %s",
				Pm_GetErrorText(error));
		}
		else {
			bInited = false;
		}
	}
}
	
// -----------------------------------------------------------------------------
unsigned int ofxPortMidiContext::getTime() {return Pt_Time();}

// PRIVATE
// -----------------------------------------------------------------------------
void ofxPortMidiContext::_timer(PtTimestamp timestamp, void *userData) {
	//cout << "timer: " << timestamp << endl;
}
