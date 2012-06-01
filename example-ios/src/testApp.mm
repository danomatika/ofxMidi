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
	
	ofBackground(127,127,127);
	
	maxMessages = 28;
	messages.push_back("nothing yet ...");
	
	note = -1;
	ctl = -1;
	
	midiIn.listPorts();
	midiOut.listPorts();
	
	midiIn.openPort();
	midiOut.openPort();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
	
}

//--------------------------------------------------------------
void testApp::exit(){
	midiIn.closePort();
	midiOut.closePort();
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){

	// send note on
	note = (int) ofMap(touch.y, 0, ofGetHeight(), 0, 127);
	midiOut.sendNoteOn(1, note);
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){

	// send ctl change
	ctl = (int) ofMap(touch.x, 0, ofGetWidth(), 0, 127); 
	midiOut.sendControlChange(1, 10, ctl); // 10: pan
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	
	// send note off
	midiOut.sendNoteOff(1, note);
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
void testApp::newMidiMessage(ofxMidiMessage& msg) {

	messages.push_back(msg.toString());
	while(messages.size() > maxMessages)
		messages.pop_front();
}

//--------------------------------------------------------------
void testApp::addMessage(string msg) {
	messages.push_back(msg);
	while(messages.size() > maxMessages)
		messages.pop_front();
}
