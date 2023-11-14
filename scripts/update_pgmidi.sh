#! /bin/sh
#
# this script automatically updates the sources for the PGMidi ios library
#
# to upgrade to a new version, change the version number below
#
# as long as the download link is formatted in the same way and folder
# structure are the same, this script should *just work*
#
# 2012 Dan Wilcox <danomatika@gmail.com> 
#

SRC=PGMidi
DEST=../libs/pgmidi

###

# move to this scripts dir
cd $(dirname $0)

# get latest source
git clone https://github.com/petegoodliffe/PGMidi.git

# create destination dir
mkdir -p $DEST

# copy readme/license
cp -v $SRC/README.md $DEST

# copy sources
cp -v $SRC/Sources/PGMidi/* $DEST

# cleanup
rm -rf $SRC
