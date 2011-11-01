#include "ofxMidiIn.h"

void ofxMidiInCallback( double deltatime, vector< unsigned char > *message, void *userData ){
	((ofxMidiIn*)userData)->manageNewMessage(deltatime,message);
	
}
// --------------------------------------------------------------------------------------
ofxMidiIn::ofxMidiIn() {
	
	// Check available ports.
	findPorts();
	
	bVerbose = false;
	
}
// --------------------------------------------------------------------------------------
ofxMidiIn::~ofxMidiIn() {
	closePort();
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::listPorts(){
	printf( "ofxMidiIn: %i ports available \n", nPorts );
	for(unsigned int i=0; i<nPorts; i++){
		printf("%i: %s\n",i, portNames[i].c_str());
	}
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::openPort(unsigned int _port){
	if ( nPorts == 0 ) {
		ofLogError() << "No ports available!";
		return;
	}
	if ( _port+1 > nPorts ){
		ofLogError() << "The selected port is not available";
		return;
	}
	
	port = _port;
	midii.openPort( port );
	
	// Set our callback function. This should be done immediately after
	// opening the port to avoid having incoming messages written to the
	// queue.
	midii.setCallback( &ofxMidiInCallback, this );
	
	// Don't ignore sysex, timing, or active sensing messages.
	midii.ignoreTypes( false, false, false );
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::openPort(string _deviceName){
	if ( nPorts == 0 ) {
		ofLogError() << "No ports available!";
		return;
	}
	
	// Iterate through MIDI ports, find requested devices
	bool foundDevice = false;
	int _port;
	for(unsigned int i=0; i < nPorts; ++i){
		string portName = portNames[i].c_str();
		if(portName.compare(_deviceName) == 0) {
			foundDevice = true;
			_port = i;
		}
	}
	if(!foundDevice) {
		// if not found
		ofLogError() << "The selected port is not available";
		return;
	}
	
	openPort( _port );	
	
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::openVirtualPort(string _port){
	
	midii.openVirtualPort(_port);
	
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::closePort(){
	midii.closePort();
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::findPorts(){
	
	// how many ports?
	nPorts = midii.getPortCount();
	
	portNames.clear();
	
	// store port names
	for(unsigned int i=0; i<nPorts; i++){
		portNames.push_back( midii.getPortName(i) );
	}
}
// --------------------------------------------------------------------------------------
void ofxMidiIn::manageNewMessage(double deltatime, vector< unsigned char > *message){
	
	unsigned int nBytes = message->size();
	if(bVerbose){
		cout << "num bytes: "<<nBytes;
		for ( unsigned int i=0; i<nBytes; i++ )
			cout << " Byte " << i << " = " << (int)message->at(i) << ", ";
		if ( nBytes > 0 )
			cout << "stamp = " << deltatime << '\n';
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
	 if(it == newIdMessageEvents.end()){
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
