#pragma once

#include "ofMain.h"

#include "RtMidi.h"
#include "ofxMidiConstants.h"
#include "ofxMidiMessage.h"
#include "ofxMidiTypes.h"

///
/// a midi output port
///
/// create multiple instances to connect to multiple ports
///
class ofxPGMidiOut {

public:

	/// set the output client name (optional)
	ofxPGMidiOut(const string name="ofxPGMidiOut Client");
	virtual ~ofxPGMidiOut();
	
/// \section Port Info
	
	/// print the connected output ports
	void listPorts();
	
	/// get a list of output port names
	/// 
	/// the vector index corresponds with the name's port number
	///
	/// note: this order may change when new devices are added/removed
	///		  from the system
	///
	vector<string>& getPortList();
	
	/// get the number of output ports
	int getNumPorts();
	
	/// get the name of an output port by it's number
	///
	/// returns "" if number is invalid
	///
	string getPortName(unsigned int portNumber);
	
/// \section Connection
	
	/// connect to an output port
	///
	/// setting port = 0 will open the first available
	///
	bool openPort(unsigned int portNumber=0);
	bool openPort(string deviceName);
	
	/// create and connect to a virtual output port (MacOS and Linux ALSA only)
	///
	/// allows for connections between software
	///
	/// note: a connected virtual port has a portNum = -1
	///	note: an open virtual port ofxPGMidiOut object cannot see it's virtual
	///       own virtual port when listing ports
	///
	bool openVirtualPort(string portName="ofxMidi Virtual Output");
	
	/// close the port connection
	void closePort();
	
	/// get the port number if connected
	///
	/// returns -1 if not connected or this is a virtual port
	///
	int getPort();
	
	/// get the connected output port name
	///
	/// returns "" if not connected
	///
	string getName();
	
	/// returns true if connected
	bool isOpen();
	
	/// returns true if this is a virtual port
	bool isVirtual();
	
/// \section Sending
	
	///
	/// midi events
	///
	/// number ranges:
	///		channel			1 - 16
	///		pitch			0 - 127
	///		velocity		0 - 127
	///		control value	0 - 127
	///		program value	0 - 127
	///		bend value		0 - 16383
	///		touch value		0 - 127
	///
	/// note:
	///		- a noteon with vel = 0 is equivalent to a noteoff
	///		- send velocity = 64 if not using velocity values
	///		- most synths don't use the velocity value in a noteoff
	///		- the lsb & msb for raw pitch bend bytes are 7 bit
	///
	/// references:
	///		http://www.srm.com/qtma/davidsmidispec.html
	///
	void sendNoteOn(int channel, int pitch, int velocity=64);
	void sendNoteOff(int channel, int pitch, int velocity=64);
	void sendControlChange(int channel, int control, int value);
	void sendProgramChange(int channel, int value);
	void sendPitchBend(int channel, int value);
	void sendPitchBend(int channel, unsigned char lsb, unsigned char msb);
	void sendAftertouch(int channel, int value);
	void sendPolyAftertouch(int channel, int pitch, int value);
	
	///
	/// raw midi bytes
	///
	void sendMidiByte(unsigned char byte);
	void sendMidiBytes(vector<unsigned char>& bytes);
	
/// \section Sending Stream Interface
	
	///
	/// midi events
	///
	/// midiout << NoteOn(1, 64, 64) << NoteOff(1, 64);
	/// midiout << ControlChange(1, 100, 64) << ProgramChange(1, 100) << PitchBend(1, 2000);
	/// midiout << Aftertouch(1, 127) << PolyAftertouch(1, 64, 127);
	///
	ofxPGMidiOut& operator<<(const NoteOn& var);
	ofxPGMidiOut& operator<<(const NoteOff& var);
	ofxPGMidiOut& operator<<(const ControlChange& var);
	ofxPGMidiOut& operator<<(const ProgramChange& var);
	ofxPGMidiOut& operator<<(const PitchBend& var);
	ofxPGMidiOut& operator<<(const Aftertouch& var);
	ofxPGMidiOut& operator<<(const PolyAftertouch& var);
	
	/// compound raw midi byte stream
	///
	/// midiout << StartMidi() << 0x90 << 0x3C << 0x40 << FinishMidi();
	///
	/// build a raw midi byte message and send it with FinishMidi()
	///
	/// note: other midi messages cannot be sent while the stream is in progress
	///
	ofxPGMidiOut& operator<<(const StartMidi& var);
	ofxPGMidiOut& operator<<(const FinishMidi& var);
	ofxPGMidiOut& operator<<(const unsigned char var);
	
private:
	
	void sendMessage();

	RtMidiOut midiout;
	int portNum;					//< current port num, -1 if not connected
	string portName;				//< current port name, "" if not connected
	
	vector<string> portList;		//< list of port names
	vector<unsigned char> message;	//< message byte buffer
	
	bool bOpen;						//< is the port currently open?
	bool bMsgInProgress;			//< used with byte stream
	bool bVirtual;					//< are we connected to a virtual port?
};
