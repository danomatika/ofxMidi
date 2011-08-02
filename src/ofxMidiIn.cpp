/*
 *      ofxMidiIn.cpp
 *
 *      v 0.02 July 2009
 *      Arturo & Chris O'Shea
 */

#include "ofxMidiIn.h"


void ofxMidiInCallback( double deltatime, std::vector< unsigned char > *message, void *userData ){
	((ofxMidiIn*)userData)->manageNewMessage(deltatime,message);

}
// --------------------------------------------------------------------------------------
ofxMidiIn::ofxMidiIn() {
	//eventManager = new ofxMidiInEventManager(this);
	//newMessageEvent.init("ofxMidiInIn::newMessage");
	//ofEvents::addEvent(newMessageEvent,"ofxMidiInIn::newMessage");
	// Check available ports.
	nPorts = midii.getPortCount();
	portNames.clear();
}
// --------------------------------------------------------------------------------------
ofxMidiIn::~ofxMidiIn() {
    closePort();
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::listPorts(){
	printf( "ofxMidiIn: %i ports available \n", nPorts );
	for(unsigned int i=0; i<nPorts; i++){
		printf("%i: %s\n",i,midii.getPortName(i).c_str());
		portNames.push_back( midii.getPortName(i) );
	}
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::openPort(unsigned int _port){
	if ( nPorts == 0 ) {
		printf( "No ports available!\n" );
		return;
	}
	if ( _port+1 > nPorts ){
		printf("The selected port is not available\n");
		return;
	}

	port = _port;
	midii.openPort( port );
	// Set our callback function.  This should be done immediately after
	// opening the port to avoid having incoming messages written to the
	// queue.
	midii.setCallback( &ofxMidiInCallback, this );

	// Don't ignore sysex, timing, or active sensing messages.
	midii.ignoreTypes( false, false, false );
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::closePort(){
    midii.closePort();
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::manageNewMessage(double deltatime, std::vector< unsigned char > *message){

	unsigned int nBytes = message->size();
	if(bVerbose){
		std::cout << "num bytes: "<<nBytes;
		for ( unsigned int i=0; i<nBytes; i++ )
			std::cout << " Byte " << i << " = " << (int)message->at(i) << ", ";
		if ( nBytes > 0 )
			std::cout << "stamp = " << deltatime << '\n';
	}

    if(nBytes>0){

        ofxMidiEventArgs eventArgs;

        eventArgs.channel = ((int)(message->at(0)) % 16)+1;
        eventArgs.status = ((int)message->at(0)) - (eventArgs.channel-1);
        eventArgs.timestamp = deltatime;

        if(nBytes==2){
            eventArgs.byteOne = (int)message->at(1);
        }else if(nBytes==3){
            eventArgs.byteOne = (int)message->at(1);
            eventArgs.byteTwo = (int)message->at(2);
        }


        ofNotifyEvent( newMessageEvent, eventArgs, this );

        // i have taken this out for now whilst I work out the best way to handle event types
        //ofNotifyEvent( *newIdMessageEvents[eventArgs.status], eventArgs, this );
    }


}
// --------------------------------------------------------------------------------------
void ofxMidiIn::setVerbose(bool verbose){
	bVerbose=verbose;
}
// --------------------------------------------------------------------------------------
unsigned int ofxMidiIn::getPort(){
	return port;
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::addListener(ofxMidiListener* listener){
	ofAddListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}

void ofxMidiIn::removeListener(ofxMidiListener* listener){
	ofRemoveListener(newMessageEvent, listener, &ofxMidiListener::newMidiMessage);
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::addListener(int id,ofxMidiListener* listener){

	// i have taken this out for now whilst I work out the best way to handle event types
	/*
	ofEvent<ofxMidiEventArgs> * event;
	map<int,ofEvent<ofxMidiEventArgs>*>::iterator it=newIdMessageEvents.find(id);
	if(it  == newIdMessageEvents.end()){
		event = new ofEvent<ofxMidiEventArgs>();
		///event->init("ofxMidiInIn:" + ofToString(id) +"::newMessage");
		newIdMessageEvents[id]=event;
	}else{
		event=it->second;
	}
	ofAddListener(*event,listener, &ofxMidiListener::newMidiMessage);
	*/
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::removeListener(int id,ofxMidiListener* listener){

	// i have taken this out for now whilst I work out the best way to handle event types

	/*
	map<int,ofEvent<ofxMidiEventArgs>*>::iterator it=newIdMessageEvents.find(id);
	if(it != newIdMessageEvents.end()){
		ofEvent<ofxMidiEventArgs> * event = it->second;
		ofRemoveListener(*event,listener,&ofxMidiListener::newMidiMessage);
	}*/
}
