#! /bin/bash
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

WD=$(dirname $0)

VER=2.1.1

SRC=rtmidi-$VER
DEST=../libs/rtmidi

###

# move to this scripts dir
cd $WD

# get latest source
curl http://www.music.mcgill.ca/~gary/rtmidi/release/rtmidi-$VER.tar.gz -O

# unpack
tar -xvf rtmidi-$VER.tar.gz

# create destination dir
mkdir -p $DEST

# copy license
cp -v $SRC/readme $DEST

# don't need C API
rm $SRC/rtmidi_c.*

# copy sources
cp -v $SRC/*.h $DEST
cp -v $SRC/*.cpp $DEST

# cleanup
rm -rf $SRC rtmidi-$VER.tar.gz

