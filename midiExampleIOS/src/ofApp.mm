/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	// iPhoneAlerts will be sent to this
	ofxiOSAlerts.addListener(this);
	
	// if you want a landscape oreintation 
	// ofxiOSSetOrientation(OF_ORIENTATION_90_RIGHT);
	
	// lets see what's going on inside
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// gray background
	ofBackground(127, 127, 127);
	
	maxMessages = 28;
	messages.push_back("nothing yet ...");
	
	note = -1;
	ctl  = -1;
	
	// enables the network midi session between iOS and Mac OSX on a
	// local wifi network
	//
	// in ofxMidi: open the input/outport network ports named "Session 1"
	//
	// on OSX: use the Audio MIDI Setup Utility to connect to the iOS device
	//
	ofxMidi::enableNetworking();
	
	// list the number of available input & output ports
	ofxMidiIn input;
	ofxMidiOut output;
	input.listInPorts();
	output.listOutPorts();
	
	// create and open input ports
	for(int i = 0; i < input.getNumInPorts(); ++i) {
		
		// new object
		inputs.push_back(new ofxMidiIn);
		
		// set this class to receive incoming midi events
		inputs[i]->addListener(this);
		
		// open input port via port number
		inputs[i]->openPort(i);
	}
	
	// create and open output ports
	for(int i = 0; i < output.getNumOutPorts(); ++i) {
		
		// new object
		outputs.push_back(new ofxMidiOut);
		
		// open input port via port number
		outputs[i]->openPort(i);
	}
	
	// set this class to receieve midi device (dis)connection events
	ofxMidi::setConnectionListener(this);
}

//--------------------------------------------------------------
void ofApp::update() {}

//--------------------------------------------------------------
void ofApp::draw() {
	
	ofSetColor(0);

	ofDrawBitmapString("Input:", 10, 20);
	int x = 10, y = 34;
	messageMutex.lock();
	deque<string>::iterator iter = messages.begin();
	for(; iter != messages.end(); ++iter) {
		ofDrawBitmapString((*iter), x, y);
		y += 14;
	}
	messageMutex.unlock();
	
	ofDrawBitmapString("Output:", 10, ofGetHeight()-42);
	if(note > 0) {
		ofDrawBitmapString("note "+ofToString(note), 10, ofGetHeight()-28);
		ofDrawRectangle(80, ofGetHeight()-38, ofMap(note, 0, 127, 0, ofGetWidth()-10), 12);
	}
	if(ctl > 0) {
		ofDrawBitmapString("pan "+ofToString(ctl), 10, ofGetHeight()-14);
		ofDrawRectangle(80, ofGetHeight()-24, ofMap(ctl, 0, 127, 0, ofGetWidth()-10), 12);
	}
}

//--------------------------------------------------------------
void ofApp::exit() {

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
void ofApp::touchDown(ofTouchEventArgs &touch) {

	// send note on
	note = (int) ofMap(touch.y, ofGetHeight(), 0, 0, 127);
	for(int i = 0; i < outputs.size(); ++i) {
		outputs[i]->sendNoteOn(1, note);
	}
}

//--------------------------------------------------------------
void ofApp::touchMoved(ofTouchEventArgs &touch) {

	// send ctl change
	ctl = (int) ofMap(touch.x, 0, ofGetWidth(), 0, 127);
	for(int i = 0; i < outputs.size(); ++i) {
		outputs[i]->sendControlChange(1, 10, ctl); // 10: pan
	}
}

//--------------------------------------------------------------
void ofApp::touchUp(ofTouchEventArgs &touch) {
	
	// send note off
	for(int i = 0; i < outputs.size(); ++i) {
		outputs[i]->sendNoteOff(1, note);
	}
	note = -1;
	ctl = -1;
}

//--------------------------------------------------------------
void ofApp::touchDoubleTap(ofTouchEventArgs &touch) {

}

//--------------------------------------------------------------
void ofApp::lostFocus() {

}

//--------------------------------------------------------------
void ofApp::gotFocus() {

}

//--------------------------------------------------------------
void ofApp::gotMemoryWarning() {

}

//--------------------------------------------------------------
void ofApp::deviceOrientationChanged(int newOrientation) {

}

//--------------------------------------------------------------
void ofApp::touchCancelled(ofTouchEventArgs& args) {

}

//--------------------------------------------------------------
void ofApp::addMessage(string msg) {
	messageMutex.lock();
	cout << msg << endl;
	messages.push_back(msg);
	while(messages.size() > maxMessages) {
		messages.pop_front();
	}
	messageMutex.unlock();
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
	addMessage(msg.toString());
}

//--------------------------------------------------------------
void ofApp::midiInputAdded(string name, bool isNetwork) {
	stringstream msg;
	msg << "ofxMidi: input added: " << name << " network: " << isNetwork;
	addMessage(msg.str());
	
	// create and open a new input port
	ofxMidiIn *newInput = new ofxMidiIn;
	newInput->openPort(name);
	newInput->addListener(this);
	inputs.push_back(newInput);
}

//--------------------------------------------------------------
void ofApp::midiInputRemoved(string name, bool isNetwork) {
	stringstream msg;
	msg << "ofxMidi: input removed: " << name << " network: " << isNetwork << endl;
	addMessage(msg.str());
	
	// close and remove input port
	vector<ofxMidiIn*>::iterator iter;
	for(iter = inputs.begin(); iter != inputs.end(); ++iter) {
		ofxMidiIn *input = (*iter);
		if(input->getName() == name) {
			input->closePort();
			input->removeListener(this);
			delete input;
			inputs.erase(iter);
			break;
		}
	}
}

//--------------------------------------------------------------
void ofApp::midiOutputAdded(string name, bool isNetwork) {
	stringstream msg;
	msg << "ofxMidi: output added: " << name << " network: " << isNetwork << endl;
	addMessage(msg.str());
	
	// create and open new output port
	ofxMidiOut *newOutput = new ofxMidiOut;
	newOutput->openPort(name);
	outputs.push_back(newOutput);
}

//--------------------------------------------------------------
void ofApp::midiOutputRemoved(string name, bool isNetwork) {
	stringstream msg;
	msg << "ofxMidi: output removed: " << name << " network: " << isNetwork << endl;
	addMessage(msg.str());
	
	// close and remove output port
	vector<ofxMidiOut*>::iterator iter;
	for(iter = outputs.begin(); iter != outputs.end(); ++iter) {
		ofxMidiOut *output = (*iter);
		if(output->getName() == name) {
			output->closePort();
			delete output;
			outputs.erase(iter);
			break;
		}
	}
}
