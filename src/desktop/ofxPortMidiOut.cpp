#include "ofxPortMidiOut.h"

#include "ofxPortMidiContext.h"
#include "../ofxMidiConstants.h"

// -----------------------------------------------------------------------------
ofxPortMidiOut::ofxPortMidiOut(const string name) : ofxBaseMidiOut(name) {
	stream = NULL;
	ofxPortMidiContext::init();
}

// -----------------------------------------------------------------------------
ofxPortMidiOut::~ofxPortMidiOut() {
	closePort();
	ofxPortMidiContext::terminate();
}

// -----------------------------------------------------------------------------
// TODO: replace cout with ofLogNotice when OF_LOG_NOTICE is the default log level
void ofxPortMidiOut::listPorts() {
	int numDev = Pm_CountDevices();
	cout << "ofxMidiOut: " << numDev-ofxPortMidiContext::numInputs() << " ports available" << endl;
	for(int i = 0; i < numDev; ++i) {
		const PmDeviceInfo *devInfo = Pm_GetDeviceInfo(i);
		if(devInfo->output)
			cout << "ofxMidiOut: " <<  i-ofxPortMidiContext::numInputs()
				 << ": " << devInfo->name << endl;
	}
}

// -----------------------------------------------------------------------------
vector<string>& ofxPortMidiOut::getPortList() {
	portList.clear();
	int numDev = Pm_CountDevices();
	for(int i = 0; i < numDev; ++i) {
		const PmDeviceInfo *devInfo = Pm_GetDeviceInfo(i);
		if(devInfo->output)
			portList.push_back(devInfo->name);
	}
	return portList;
}

// -----------------------------------------------------------------------------
int ofxPortMidiOut::getNumPorts() {
	return Pm_CountDevices();
}

// -----------------------------------------------------------------------------
string ofxPortMidiOut::getPortName(unsigned int portNumber) {
	
	const PmDeviceInfo *devInfo = Pm_GetDeviceInfo(portNumber+ofxPortMidiContext::numInputs());
	if(devInfo) {
		return devInfo->name;
	}
	else {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't get name for port %i: port num out of range",
			portNumber);
	}
	return "";
}

// -----------------------------------------------------------------------------
bool ofxPortMidiOut::openPort(unsigned int portNumber) {	
	closePort();
	PmError error = Pm_OpenOutput(&stream, (int) portNumber+ofxPortMidiContext::numInputs(),
		NULL, 256, NULL, NULL, 1);//(PmTimestamp (*)(void *)) Pt_Time, NULL, 1); // latency
	if(error != pmNoError) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't open port %i: %s", portNumber, Pm_GetErrorText(error));
		return false;
	}
	portNum = portNumber;
	portName = getPortName(portNumber);
	bOpen = true;
	ofLog(OF_LOG_VERBOSE, "ofxMidiOut: opened port %i %s", portNum, portName.c_str());
	return true;
}

// -----------------------------------------------------------------------------
bool ofxPortMidiOut::openPort(string deviceName) {
	
	// iterate through MIDI ports, find requested device
	int port = -1, numDev = Pm_CountDevices();
	for(unsigned int i = 0; i < numDev; ++i) {
		const PmDeviceInfo *devInfo = Pm_GetDeviceInfo(i);
		string name = devInfo->name;
		if(name == deviceName) {
			port = i;
			break;
		}
	}
	
	// bail if not found
	if(port == -1) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: port \"%s\" is not available", deviceName.c_str());
		return false;
	} 
	
	return openPort(port-ofxPortMidiContext::numInputs());
}

// -----------------------------------------------------------------------------
bool ofxPortMidiOut::openVirtualPort(string portName) {
//	// handle rtmidi exceptions
//	try {
//		closePort();
//		midiOut.openVirtualPort(portName);
//	}
//	catch(RtError& err) {
//		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't open virtual port \"%s\": %s",
//			portName.c_str(), err.what());
//		return false;
//	}
//	
//	this->portName = portName;
//	bOpen = true;
//	bVirtual = true;
//	ofLog(OF_LOG_VERBOSE, "ofxMidiOut: opened virtual port %s", portName.c_str());
	return true;
}

// -----------------------------------------------------------------------------
void ofxPortMidiOut::closePort() {
//	if(bVirtual && bOpen) {
//		ofLog(OF_LOG_VERBOSE, "ofxMidiOut: closing virtual port %s", portName.c_str());
//	}
//	else 
	if(bOpen && portNum > -1) {
		ofLog(OF_LOG_VERBOSE, "ofxMidiOut: closing port %i %s", portNum, portName.c_str());
		PmError error = Pm_Close(stream);
		if(error != pmNoError) {
			ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't close port %d: %s", portNum, Pm_GetErrorText(error));
		}
	}
	
	stream = NULL;
	portNum = -1;
	portName = "";
	bOpen = false;
	bMsgInProgress = false;
	bVirtual = false;
}

// PRIVATE
// -----------------------------------------------------------------------------
void ofxPortMidiOut::sendMessage(unsigned int deltatime) {
	sendMessageAtTime(Pt_Time() + deltatime);
}

// derived from RtMidi RtMidiOut::sendMessage
void ofxPortMidiOut::sendMessageAtTime(unsigned long long timestamp) {
	
	if(message.size() == 0) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send, message is empty");
		return;
	}
	
	PmError error = pmNoError;
	if(message[0] == MIDI_SYSEX) { 
		error = Pm_WriteSysEx(stream, 0, &message[0]);
	}
	else if(message.size() > 3) {
		ofLog(OF_LOG_WARNING, "ofxMidiOut: cannot send, message len > 3 bytes but not sysex");
	}
	else {
		PmEvent event;
		event.message = 0;
		event.timestamp = (unsigned int) timestamp;
		switch(message.size()) {
		
			case 1:
				event.message = Pm_MessageStatus(message[0]);
			break;
			
			case 2:
				event.message = Pm_MessageStatus(message[0]);
				event.message = Pm_MessageData1(message[1]);
			break;
							
			default: case 3:
				event.message = Pm_Message(message[0], message[1], message[2]);
				break;
		}
		error = Pm_Write(stream, &event, 1);
	}
	
	if(error != pmNoError) {
		ofLog(OF_LOG_ERROR, "ofxMidiOut: couldn't send message: %s", Pm_GetErrorText(error));
	}
	
	bMsgInProgress = false;
}
