#ifndef _TEST_APP
#define _TEST_APP


#include "ofMain.h"

#define OF_ADDON_USING_OFXMIDIIN

#include "ofAddons.h"

class testApp : public ofSimpleApp, public ofxMidiListener{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased (int key);

		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased();


		int port;
		int id;
		int value;
		double timestamp;
		char msg[255];

		ofxMidiIn	midiIn;
//midiIn events
		void newMessage(int port, int id, int value, double timestamp);


};

#endif

