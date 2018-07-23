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
	ofSetLogLevel("ofxMidiClock", OF_LOG_NOTICE);
	ofSetLogLevel("ofxMidiTimecode", OF_LOG_NOTICE);

	// open port by number (you may need to change this)
	midiIn.openPort(0);
	midiIn.ignoreTypes(false, // sysex  <-- don't ignore timecode messages!
	                   false, // timing <-- don't ignore clock messages!
	                   true); // sensing

	// add ofApp as a listener
	midiIn.addListener(this);
}

//--------------------------------------------------------------
void ofApp::update() {

	// MIDI TIMECODE

	// if we havent received a quareter frame message from the timecode master,
	// assume playback has stopped
	if(timecodeRunning && ofGetElapsedTimeMillis() - timecodeTimestamp > 100) {
		ofLog() << "timecode stopped";
		timecodeRunning = false;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0);

	// MIDI CLOCK

	ofDrawBitmapString((clockRunning ? "MIDI clock: running" : "MIDI clock: stopped"), 20, 30);
	ofDrawBitmapString("pos MIDI beats: "+ofToString(beats), 20, 58);
	ofDrawBitmapString("pos seconds: "+ofToString(seconds), 20, 74);
	ofDrawBitmapString("bpm: "+ofToString(round(bpm)), 20, 90);

	// a MIDI beat is a 16th note, so do a little math to convert to a time signature:
	// 4/4 -> 4 notes per bar & quarter note = 1 beat, add 1 to count from 1 instead of 0
	int quarters = beats / 4; // convert total # beats to # quarters
	int bars = (quarters / 4) + 1; // compute # of bars
	int beatsInBar = (quarters % 4) + 1; // compute remainder as # notes within the current bar
	ofDrawBitmapString("4/4 bars: "+ofToString(bars)+" beat: "+ofToString(beatsInBar), 20, 106);

	// MIDI TIMECODE

	ofDrawBitmapString((timecodeRunning ? "MIDI timecode: running" : "MIDI timecode: stopped"), 20, 160);
	ofDrawBitmapString("pos time: "+frame.toString(), 20, 188);
	ofDrawBitmapString("pos seconds: "+ofToString(frame.toSeconds()), 20, 204);
	ofDrawBitmapString("framerate: "+ofToString(ofxMidiTimecode::rateToFps(frame.rate)), 20, 220);

	// if you are working with a timecode master which uses an offset
	// simply add or subtract the time, ie. Logic defaults to +1 hour
	//ofDrawBitmapString("pos seconds (no offset): "+ofToString(frame.toSeconds() - 3600), 20, 236);
}

//--------------------------------------------------------------
void ofApp::exit() {

	// clean up
	midiIn.closePort();
	midiIn.removeListener(this);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// toggle verbose printing
	switch(key) {
		case 'v':
			verbose = !verbose;
			if(verbose) {
				ofSetLogLevel("ofxMidiClock", OF_LOG_VERBOSE);
				ofSetLogLevel("ofxMidiTimecode", OF_LOG_VERBOSE);
			}
			else {
				ofSetLogLevel("ofxMidiClock", OF_LOG_NOTICE);
				ofSetLogLevel("ofxMidiTimecode", OF_LOG_NOTICE);
			}
			ofLog() << (verbose ? "on" : "off");
	}
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

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& message) {

	// MIDI CLOCK

	// update the clock length and song pos in beats
	if(clock.update(message.bytes)) {
		// we got a new song pos
		beats = clock.getBeats();
		seconds = clock.getSeconds();
	}

	// compute the seconds and bpm
	switch(message.status) {

		// compute seconds and bpm live, you may or may not always need this
		// which is why it is not integrated into the ofxMidiClock parser class
		case MIDI_TIME_CLOCK:
			seconds = clock.getSeconds();
			bpm += (clock.getBpm() - bpm) / 5; // average the last 5 bpm values
			// no break here so the next case statement is checked,
			// this way we can set clockRunning if we've missed a MIDI_START
			// ie. master was running before we started this example

		// transport control
		case MIDI_START: case MIDI_CONTINUE:
			if(!clockRunning) {
				clockRunning = true;
				ofLog() << "clock started";
			}
			break;
		case MIDI_STOP:
			if(clockRunning) {
				clockRunning = false;
				ofLog() << "clock stopped";
			}
			break;

		default:
			break;
	}

	// MIDI TIMECODE

	// update the timecode pos
	if(timecode.update(message.bytes)) {

		// we got a new frame pos
		frame = timecode.getFrame();

		// if the last message was a timecode quarter frame message,
		// then assume the timecode master has started playback
		if(message.status == MIDI_TIME_CODE) {
			if(!timecodeRunning) {
				timecodeRunning = true;
				ofLog() << "timecode started";
			}
			timecodeTimestamp = ofGetElapsedTimeMillis();
		}
	}
}
