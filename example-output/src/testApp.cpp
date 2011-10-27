#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofBackground(255);
	midiOut.listPorts();
	midiOut.openPort(1);
}

//--------------------------------------------------------------
void testApp::update(){
}

//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	midiOut.sendNoteOn(1, key, 127);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	midiOut.sendNoteOff(1, key, 0);
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
