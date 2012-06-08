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

VER=1.0.15

###

# move to this scripts dir
cd $WD

# get latest source
curl http://www.music.mcgill.ca/~gary/rtmidi/release/rtmidi-$VER.tar.gz -O

# unpack
tar -xvf rtmidi-$VER.tar.gz

# create destination dir
mkdir -p ../src/desktop/rtmidi

# copy license
cp -v rtmidi-$VER/readme ../src/desktop/rtmidi

# copy sources
cp -v rtmidi-$VER/*.h ../src/desktop/rtmidi
cp -v rtmidi-$VER/*.cpp ../src/desktop/rtmidi

# cleanup
rm -rfv rtmidi-$VER rtmidi-$VER.tar.gz

