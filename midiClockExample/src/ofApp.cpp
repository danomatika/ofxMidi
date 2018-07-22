/*
 * Copyright (c) 2018 Dan Wilcox <danomatika@gmail.com>
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
	ofSetVerticalSync(true);
	ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);

	// open port by number (you may need to change this)
	midiIn.openPort(0);
	midiIn.ignoreTypes(true,  // sysex
	                   false, // timing <-- don't ignore clock messages!
	                   true); // sensing

	// add ofApp as a listener
	midiIn.addListener(this);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0);

	ofDrawBitmapString((running ? "midi clock running" : "midi clock stopped"), 20, 30);
	ofDrawBitmapString("song pos beats: "+ofToString(floor(clock.getBeats())), 20, 58);
	ofDrawBitmapString("song pos seconds: "+ofToString(floor(seconds)), 20, 46);
	ofDrawBitmapString("bpm: "+ofToString(round(bpm)), 20, 70);
}

//--------------------------------------------------------------
void ofApp::exit() {

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& message) {

	// update the clock length and song pos in beats
	clock.update(message.bytes);

	// compute the seconds and bpm,
	switch(message.status) {

		// compute seconds and bpm live, you may or may not always need this
		// which is why it is not integrated into the ofxMidiClock parser class
		case MIDI_TIME_CLOCK:
			seconds = clock.getSeconds();
			bpm += (clock.getBpm() - bpm) / 5; // average the last 5 bpm values
			break;
		case MIDI_SONG_POS_POINTER:
			seconds = clock.getSeconds();
			break;

		// transport control
		case MIDI_START: case MIDI_CONTINUE:
			if(!running) {
				running = true;
				ofLog() << "clock started";
			}
			break;
		case MIDI_STOP:
			if(running) {
				running = false;
				ofLog() << "clock stopped";
			}
			break;

		default:
			break;
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) { 

}
