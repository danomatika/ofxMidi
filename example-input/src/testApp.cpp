#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(255,255,255);
	midiIn.listPorts();
	midiIn.openPort(1);
	midiIn.addListener(this);
	
	// to register only to one controller pass the id as first argument
	// midiIn.addListener(84,this);
	
	// to debug
	// midiIn.setVerbose(true);
}

//--------------------------------------------------------------
void testApp::update(){
	sprintf(msg,"value: %i, received from port: %i, id: %i \n\nwith %f milliseconds difference from last message",value,port,id,timestamp);
}

//--------------------------------------------------------------
void testApp::draw(){
	ofSetColor(0x000000);
	ofRect(20,40,(ofGetWidth()-40)*(float)value/127,20);
	ofDrawBitmapString(msg,20,300);
}

//--------------------------------------------------------------
void testApp::newMidiMessage(ofxMidiEventArgs& eventArgs) {
	
	// store some data from midi message in variables
	value = eventArgs.byteOne;
	id = eventArgs.channel;
	port = eventArgs.port;
	timestamp = eventArgs.timestamp;
	
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
}
