/*
 *      ofxMidiEvents.h
 *
 *      v 0.02 July 2009
 *      Arturo & Chris O'Shea
 */

#ifndef OFXMIDIEVENTS_H_
#define OFXMIDIEVENTS_H_

#include "ofMain.h"
#include <map>



class ofxMidiEventArgs: public ofEventArgs{
public:
	int 	port;
	int		channel;
	int 	status;
	int 	byteOne;
	int 	byteTwo;
	double 	timestamp;
};

typedef ofEvent <ofxMidiEventArgs> ofxMidiEvent ;


class ofxMidiListener {
public:
	ofxMidiListener(){}
	virtual ~ofxMidiListener(){}

	virtual void newMidiMessage(ofxMidiEventArgs& eventArgs)=0;

};


#endif /* OFXMIDIEVENTS_H_ */
