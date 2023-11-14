#! /bin/sh
#
# this script automatically updates the sources for the rtmidi library
#
# to upgrade to a new version, change the version number below
#
# as long as the download link is formatted in the same way and folder
# structure are the same, this script should *just work*
#
# 2012 Dan Wilcox <danomatika@gmail.com> 
#

VER=6.0.0

SRC=rtmidi-$VER
DEST=../libs/rtmidi

###

# move to this scripts dir
cd $(dirname $0)

# get latest source
curl http://www.music.mcgill.ca/~gary/rtmidi/release/rtmidi-$VER.tar.gz -O

# unpack
tar -xvf rtmidi-$VER.tar.gz

# create destination dir
mkdir -p $DEST

# copy license
cp -v $SRC/README.md $DEST

# don't need C API
rm $SRC/rtmidi_c.*

# copy sources
cp -v $SRC/*.h $DEST
cp -v $SRC/*.cpp $DEST

# add include so MIDI api is set when compiling
# the line to add may need to be changed as the RtMidi.h header changes
printf "%s\n\n" 47i "#include \"ofxMidiConstants.h\"" . x | ex $DEST/RtMidi.h

# cleanup
rm -rf $SRC rtmidi-$VER.tar.gz
