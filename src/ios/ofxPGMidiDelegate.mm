#include "ofxPGMidiDelegate.h"

#include "ofMain.h"

// -----------------------------------------------------------------------------
@implementation ofxPGMidiDelegate

// -----------------------------------------------------------------------------
- (id) init {
	self = [super init];
	return self;
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source {
	//source.delegate = self;
	stringstream msg;
	msg << "source added: " << [source.name UTF8String];
	//app->addMessage(msg.str());
	cout << msg << endl;
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source {
	//source.delegate = nil;
	stringstream msg;
	msg << "source removed: " << [source.name UTF8String];
	//app->addMessage(msg.str());
	cout << msg << endl;
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination {
	stringstream msg;
	msg << "dest added: " << [destination.name UTF8String];
	//app->addMessage(msg.str());
	cout << msg << endl;
}

// -----------------------------------------------------------------------------
- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination {
	stringstream msg;
	msg << "dest removed: " << [destination.name UTF8String];
	cout << msg << endl;
	//input->addMessage(msg.str());
}

@end
