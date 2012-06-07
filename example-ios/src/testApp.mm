#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
	
	//If you want a landscape oreintation 
	//iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	
	// lets see what's going on inside
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// gray background
	ofBackground(127,127,127);
	
	maxMessages = 28;
	messages.push_back("nothing yet ...");
	
	note = -1;
	ctl = -1;
	
	// enables the network midi session between iOS and Mac OSX on a
	// local wifi network
	//
	// in ofxMidi: open the input/outport network ports named "Session 1"
	//
	// on OSX: use the Audio MIDI Setup Utility to connect to the iOS device
	//
	ofxMidi::enableNetworking();
	
	// create a set of i/o ports
	inputs.push_back(new ofxMidiIn);
	outputs.push_back(new ofxMidiOut);
	
	// list the number of available input & output ports
	inputs[0]->listPorts();
	outputs[0]->listPorts();
	
	// set this class to receive incoming midi events
	inputs[0]->addListener(this);
	
	// set the class to receieve midi device (dis)connection events
	ofxMidi::setConnectionListener(this);
	
	// open the first available ports
	inputs[0]->openPort();
	outputs[0]->openPort();
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
	
	ofSetColor(0);

	ofDrawBitmapString("Input:", 10, 20);
	int x = 10, y = 34;
	deque<string>::iterator iter = messages.begin();
	for(; iter != messages.end(); ++iter) {
		ofDrawBitmapString((*iter), x, y);
		y += 14;
	}
	
	ofDrawBitmapString("Output:", 10, ofGetHeight()-42);
	if(note > 0) {
		ofDrawBitmapString("note "+ofToString(note), 10, ofGetHeight()-28);
	}
	if(ctl > 0) {
		ofDrawBitmapString("pan "+ofToString(ctl), 10, ofGetHeight()-14);
	}
}

//--------------------------------------------------------------
void testApp::exit(){

	// clean up
	
	for(int i = 0; i < inputs.size(); ++i) {
		inputs[i]->closePort();
		inputs[i]->removeListener(this);
		delete inputs[i];
	}

	for(int i = 0; i < outputs.size(); ++i) {
		outputs[i]->closePort();
		delete outputs[i];
	}
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

	// send note on
	note = (int) ofMap(touch.y, ofGetHeight(), 0, 0, 127);
	for(int i = 0; i < outputs.size(); ++i) {
		outputs[i]->sendNoteOn(1, note);
	}
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

	// send ctl change
	ctl = (int) ofMap(touch.x, 0, ofGetWidth(), 0, 127);
	for(int i = 0; i < outputs.size(); ++i) {
		outputs[i]->sendControlChange(1, 10, ctl); // 10: pan
	}
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	
	// send note off
	for(int i = 0; i < outputs.size(); ++i) {
		outputs[i]->sendNoteOff(1, note);
	}
	note = -1;
	ctl = -1;
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}


//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}

//--------------------------------------------------------------
void testApp::addMessage(string msg) {
	cout << msg << endl;
	messages.push_back(msg);
	while(messages.size() > maxMessages)
		messages.pop_front();
}

//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiMessage& msg) {
	addMessage(msg.toString());
}

//--------------------------------------------------------------
void testApp::midiInputAdded(string name, bool isNetwork) {
	stringstream msg;
	msg << "ofxMidi: input added: " << name << " network: " << isNetwork;
	addMessage(msg.str());
	
	ofxMidiIn * newInput = new ofxMidiIn;
	newInput->openPort(name);
	newInput->addListener(this);
	inputs.push_back(newInput);
}

void testApp::midiInputRemoved(string name, bool isNetwork) {
	stringstream msg;
	msg << "ofxMidi: input removed: " << name << " network: " << isNetwork << endl;
	addMessage(msg.str());
	
	vector<ofxMidiIn*>::iterator iter;
	for(iter = inputs.begin(); iter != inputs.end(); ++iter) {
		ofxMidiIn * input = (*iter);
		if(input->getName() == name) {
			input->closePort();
			input->removeListener(this);
			delete input;
			inputs.erase(iter);
			break;
		}
	}
}

void testApp::midiOutputAdded(string name, bool isNetwork) {
	stringstream msg;
	msg << "ofxMidi: output added: " << name << " network: " << isNetwork << endl;
	addMessage(msg.str());
	
	ofxMidiOut * newOutput = new ofxMidiOut;
	newOutput->openPort(name);
	outputs.push_back(newOutput);
}

void testApp::midiOutputRemoved(string name, bool isNetwork) {
	stringstream msg;
	msg << "ofxMidi: output removed: " << name << " network: " << isNetwork << endl;
	addMessage(msg.str());
	
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		ofxMidiOut * output = (*iter);
		if(output->getName() == name) {
			output->closePort();
			delete output;
			outputs.erase(iter);
			break;
		}
	}
}
