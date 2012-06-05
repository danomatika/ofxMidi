#pragma once

#import "PGMidi.h"

@class PGMidi;

/// interface to global pgmidi events
@interface ofxPGMidiDelegate : NSObject <PGMidiDelegate> {}

- (void) midi:(PGMidi*)midi sourceAdded:(PGMidiSource *)source;
- (void) midi:(PGMidi*)midi sourceRemoved:(PGMidiSource *)source;
- (void) midi:(PGMidi*)midi destinationAdded:(PGMidiDestination *)destination;
- (void) midi:(PGMidi*)midi destinationRemoved:(PGMidiDestination *)destination;

@end
