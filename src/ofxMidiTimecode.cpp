/*
 * Copyright (c) 2018 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 * Adapted from code written in Swift for the Hertz-Lab @ ZKM | Karlsruhe
 *
 */
#include "ofxMidiTimecode.h"

#include <sstream>
#include <iomanip>
#include <cmath>
#include "ofLog.h"
#include "ofxMidi.h"

// ofxMidiTimecodeFrame

// -----------------------------------------------------------------------------
double ofxMidiTimecodeFrame::getFps() const {
	return ofxMidiTimecode::rateToFps(rate);
}

// -----------------------------------------------------------------------------
std::string ofxMidiTimecodeFrame::toString() const {
	std::stringstream stream;
	stream << std::setw(2) << std::setfill('0') << hours << ":"
	       << std::setw(2) << std::setfill('0') << minutes << ":"
	       << std::setw(2) << std::setfill('0') << seconds << ":"
	       << std::setw(2) << std::setfill('0') << frames;
	return stream.str();
}

/// convert to time in seconds
double ofxMidiTimecodeFrame::toSeconds() const {
	double time = (double)hours * 3600.0; // 60.0 * 60.0
	time += (double)minutes * 60.0;
	time += (double)seconds;
	time += ofxMidiTimecode::framesToMs(frames, rate) / 100;
	return time;
}

/// convert from time in seconds, ignores framerate
void ofxMidiTimecodeFrame::fromSeconds(double s) {
	return fromSeconds(s, 0x0);
}

/// convert from time in seconds & framerate value (not fps!)
void ofxMidiTimecodeFrame::fromSeconds(double s, unsigned char r) {
	seconds = (int)s % 60;
	minutes = (int)((s - seconds) * 0.016666667) % 60;
	hours = (int)(s * 0.00027777778) % 60;

	// round fractional part of seconds for ms
	double ms = (int)(floor((s - floor(s)) * 100.0) + 0.5);
	frames = ofxMidiTimecode::msToFrames(ms, r);
	rate = r;
}

// ofxMidiTimecode

// -----------------------------------------------------------------------------
bool ofxMidiTimecode::update(std::vector<unsigned char> &message) {
	unsigned char statusByte = message[0];
	if(statusByte == MIDI_TIME_CODE) {
		return decodeQuarterFrame(message);
	}
	else if(statusByte == MIDI_SYSEX) {
		return decodeFullFrame(message);
	}
	return false;
}

// -----------------------------------------------------------------------------
void ofxMidiTimecode::reset() {
	frame = ofxMidiTimecodeFrame();
	quarterFrame = QuarterFrame();
}

/// Util

// -----------------------------------------------------------------------------
int ofxMidiTimecode::framesToMs(int frames, unsigned char rate) {
	return (int)((1.0/rateToFps(rate)) * (double)frames * 100.0);
}

// -----------------------------------------------------------------------------
int ofxMidiTimecode::msToFrames(int ms, unsigned char rate) {
	return (int)((double)ms / (1.0/rateToFps(rate)) / 100.0);
}

// -----------------------------------------------------------------------------
double ofxMidiTimecode::rateToFps(unsigned char rate) {
	switch(rate) {
		default:
		case FRAMERATE_24:
			return 24.0;
		case FRAMERATE_25:
			return 25.0;
		case FRAMERATE_30_DROP:
			return 29.97;
		case FRAMERATE_30:
			return 30.0;
	}
}

// -----------------------------------------------------------------------------
unsigned char ofxMidiTimecode::fpsToRate(double fps) {
	int rate = (int)round(fps);
	if(fps < 25) {return FRAMERATE_24;}
	if(fps < 29) {return FRAMERATE_25;}
	if(fps < 30) {return FRAMERATE_30_DROP;}
	return FRAMERATE_30; // 30 fps
}

/// PROTECTED

// -----------------------------------------------------------------------------
bool ofxMidiTimecode::decodeQuarterFrame(std::vector<unsigned char> &message) {

	ofLogVerbose("ofxMidiTimecode") << "Quarter Frame " << ofxMidi::bytesToString(message);

	bool complete = false;
	unsigned char dataByte = message[1];
	unsigned char msgType = dataByte & 0xF0;

	if(quarterFrame.direction == QuarterFrame::UNKNOWN && quarterFrame.count > 1) {
		unsigned char lastMsgType = quarterFrame.lastDataByte & 0xF0;
		if(lastMsgType < msgType) {
			quarterFrame.direction = QuarterFrame::FORWARDS;
		}
		else if(lastMsgType > msgType) {
			quarterFrame.direction = QuarterFrame::BACKWARDS;
		}
	}
	quarterFrame.lastDataByte = dataByte;

	switch(msgType) {
		case 0x00: // frame LSB
			quarterFrame.frame.frames = (int)(dataByte & 0x0F);
			quarterFrame.count += 1;
			quarterFrame.receivedFirst = true;
			if(quarterFrame.count >= QUARTERFRAME_LEN &&
				quarterFrame.direction == QuarterFrame::BACKWARDS &&
				quarterFrame.receivedLast) {
				complete = true;
			}
			break;
		case 0x10: // frame MSB
			quarterFrame.frame.frames |= (int)((dataByte & 0x01) << 4);
			quarterFrame.count += 1;
			break;
		case 0x20: // second LSB
			quarterFrame.frame.seconds = (int)(dataByte & 0x0F);
			quarterFrame.count += 1;
			break;
		case 0x30: // second MSB
			quarterFrame.frame.seconds |= (int)((dataByte & 0x03) << 4);
			quarterFrame.count += 1;
			break;
		case 0x40: // minute LSB
			quarterFrame.frame.minutes = (int)(dataByte & 0x0F);
			quarterFrame.count += 1;
			break;
		case 0x50: // minute MSB
			quarterFrame.frame.minutes |= (int)((dataByte & 0x03) << 4);
			quarterFrame.count += 1;
			break;
		case 0x60: // hours LSB
			quarterFrame.frame.hours = (int)(dataByte & 0x0F);
			quarterFrame.count += 1;
			break;
		case 0x70: // hours MSB & framerate
			quarterFrame.frame.hours |= (int)((dataByte & 0x01) << 4);
			quarterFrame.frame.rate = (dataByte & 0x06) >> 1;
			quarterFrame.count += 1;
			quarterFrame.receivedLast = true;
			if(quarterFrame.count >= QUARTERFRAME_LEN &&
				quarterFrame.direction == QuarterFrame::FORWARDS &&
				quarterFrame.receivedFirst) {
				complete = true;
			}
			break;
		default:
			return false;
	}

	// update time using the (hopefully) complete message
	if(complete) {
		// add 2 frames to compensate for time it takes to receive 8 QF messages
		quarterFrame.frame.frames += 2;
		frame = quarterFrame.frame;
		ofLogVerbose("ofxMidiTimecode") << frame.toString();
		quarterFrame = QuarterFrame();
		return true;
	}

	return false;
}

// -----------------------------------------------------------------------------
bool ofxMidiTimecode::decodeFullFrame(std::vector<unsigned char> &message) {
	if(message.size() == FULLFRAME_LEN && isFullFrame(message)) {
		ofLogVerbose("ofxMidiTimecode") << "Full Frame " << ofxMidi::bytesToString(message);
		frame.hours = (int)(message[5] & 0x1F);
		frame.rate = (int)((message[5] & 0x60) >> 5);
		frame.minutes = (int)(message[6]);
		frame.seconds = (int)(message[7]);
		frame.frames = (int)(message[8]);
		ofLogVerbose("ofxMidiTimecode") << frame.toString();
		return true;
	}
	return false;
}

// -----------------------------------------------------------------------------
bool ofxMidiTimecode::isFullFrame(std::vector<unsigned char> &message) {
	return
		(message[1] == 0x7F) && // universal message
		(message[2] == 0x7F) && // global broadcast
		(message[3] == 0x01) && // time code
		(message[4] == 0x01) && // full frame
		(message[9] == 0xF7);   // end of sysex
}
