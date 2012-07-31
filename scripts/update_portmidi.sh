#! /bin/bash

WD=$(dirname $0)

VER=217

###

cd $WD

# get latest source
curl -L "http://sourceforge.net/projects/portmedia/files/portmidi/$VER/portmidi-src-$VER.zip/download" > portmidi.zip
unzip -q portmidi.zip

# create dirs
mkdir -p ../libs/portmidi
mkdir -p ../libs/portmidi/mac
mkdir -p ../libs/portmidi/linux
mkdir -p ../libs/portmidi/win

# copy licenses etc
cp -v portmidi/license.txt ../libs/portmidi
cp -v portmidi/README.txt ../libs/portmidi
cp -v portmidi/CHANGELOG.txt ../libs/portmidi

# copy sources
cp -Rv portmidi/pm_common/*.h ../libs/portmidi
cp -Rv portmidi/pm_common/*.c ../libs/portmidi

cp -Rv portmidi/pm_mac/pmmac.h ../libs/portmidi/mac
cp -Rv portmidi/pm_mac/pmmac.c ../libs/portmidi/mac
cp -Rv portmidi/pm_mac/pmmacosxcm.h ../libs/portmidi/mac
cp -Rv portmidi/pm_mac/pmmacosxcm.c ../libs/portmidi/mac

cp -Rv portmidi/pm_linux/pmlinux.h ../libs/portmidi/linux
cp -Rv portmidi/pm_linux/pmlinux.c ../libs/portmidi/linux
cp -Rv portmidi/pm_linux/pmlinuxalsa.h ../libs/portmidi/linux
cp -Rv portmidi/pm_linux/pmlinuxalsa.c ../libs/portmidi/linux

cp -Rv portmidi/pm_win/pmwin.c ../libs/portmidi/win
cp -Rv portmidi/pm_win/pmwinmm.h ../libs/portmidi/win
cp -Rv portmidi/pm_win/pmwinmm.c ../libs/portmidi/win

cp -Rv portmidi/porttime/porttime.h ../libs/portmidi
cp -Rv portmidi/porttime/ptmacosx_cf.c ../libs/portmidi/mac
cp -Rv portmidi/porttime/ptmacosx_mach.c ../libs/portmidi/mac
cp -Rv portmidi/porttime/ptlinux.c ../libs/portmidi/linux
cp -Rv portmidi/porttime/ptwinmm.c ../libs/portmidi/win

# cleanup
rm -rf portmidi*
