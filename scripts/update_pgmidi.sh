#! /bin/bash
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

WD=$(dirname $0)

###

# move to this scripts dir
cd $WD

# get latest source
git clone git://github.com/petegoodliffe/PGMidi.git

# create destination dir
mkdir -p ../src/ios/pgmidi

# copy readme/license
cp -v PGMidi/README.md ../src/ios/pgmidi

# copy sources
cp -v PGMidi/Sources/PGMidi/* ../src/ios/pgmidi

# cleanup
rm -rfv PGMidi

