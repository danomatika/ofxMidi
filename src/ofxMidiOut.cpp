#include "ofxMidiOut.h"

// --------------------------------------------------------------------------------------
ofxMidiOut::ofxMidiOut() {

	// Check available ports.
	findPorts();

}
// --------------------------------------------------------------------------------------
ofxMidiOut::~ofxMidiOut() {
	closePort();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::listPorts(){
	printf( "ofxMidiOut: %i ports available \n", nPorts );
	for(unsigned int i=0; i<nPorts; i++){
		printf("%i: %s\n", i, portNames[i].c_str());
	}
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::findPorts(){
	
	// how many ports?
	nPorts = midiout.getPortCount();
	
	portNames.clear();
	
	// store port names
	for(unsigned int i=0; i<nPorts; i++){
		portNames.push_back( midiout.getPortName(i) );
	}
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::openPort(unsigned int _port){
	if ( nPorts == 0 ) {
		ofLogError() << "No ports available!";
		return;
	}
	if ( _port+1 > nPorts ){
		ofLogError() << "The selected port is not available";
		return;
	}
	
	port = _port;
	midiout.openPort( port );
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::openPort(string _deviceName){
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
void ofxMidiOut::openVirtualPort(string _port){
	
	midiout.openVirtualPort(_port);
	
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::closePort(){
	midiout.closePort();
}
// --------------------------------------------------------------------------------------
unsigned int ofxMidiOut::getPort(){
	return port;
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendMessage(){
	midiout.sendMessage( &message );
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendNoteOn(int channel, int id, int value){
	message.clear();
	message.push_back( MIDI_NOTE_ON+(channel-1) );
	message.push_back( id );
	message.push_back( value );
	sendMessage();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendNoteOff(int channel, int id, int value){
	message.clear();
	message.push_back( MIDI_NOTE_OFF+(channel-1) );
	message.push_back( id );
	message.push_back( value );
	sendMessage();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendControlChange(int channel, int id, int value){
	message.clear();
	message.push_back( MIDI_CONTROL_CHANGE+(channel-1) );
	message.push_back( id );
	message.push_back( value );
	sendMessage();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendProgramChange(int channel, int value){
	message.clear();
	message.push_back( MIDI_PROGRAM_CHANGE+(channel-1) );
	message.push_back( value );
	sendMessage();
}
// --------------------------------------------------------------------------------------
void ofxMidiOut::sendPitchBend(int channel, int lsb, int msb){
	message.clear();
	message.push_back( MIDI_PITCH_BEND+(channel-1) );
	message.push_back( lsb );
	message.push_back( msb );
	sendMessage();
}
