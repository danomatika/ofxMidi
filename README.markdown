ofxMidi
===========

Midi input and output addon for openFrameworks

By Chris O'Shea, Arturo Castro, Kyle McDonald & Dan Wilcox

See Documentation on [Github](https://github.com/chrisoshea/ofxMidi) and the [OpenFrameworks Forum post](http://forum.openframeworks.cc/index.php/topic,2435.0.html).

OpenFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc/](http://www.openframeworks.cc/)

Description
-----------

ofxMidi provides [Music Instrument Digial Interface](http://en.wikipedia.org/wiki/Musical_Instrument_Digital_Interface) IO capability to an OpenFrameworks app

* ofxMidiIn: a single midi input port, derive from the ofxMidiListener class to receive messages
* ofxMidiMessage: a received midi message
* ofxMidiOut: a single midi output port, includes a stream << interface

This project utilizes [RtMidi](http://www.music.mcgill.ca/~gary/rtmidi/) for the backend and currently supports Mac, Windows, and Linux. iOS support is in the works.

Installation
------------

To use ofxMidi, first you need to download and install Open Frameworks. ofxMidi is currently developed against the latest version of Open Frameworks on github (007).

To get a copy of the repository you can download the source from [http://github.com/chrisoshea/ofxMidi/zipball/master](http://github.com/chrisoshea/ofxMidi/zipball/master) or, alternatively, you can use git clone:
<pre>
git clone git://github.com/chrisoshea/ofxMidi.git
</pre>

The addon should sit in `openFrameworks/addons/ofxMidi/`.

Running the Example Project
---------------------------

If you're using OS X, open an Xcode project in ofxMidi/example-input/ or ofxMidi/example-output/ and hit "Build and Run".

On Linux, you can use the Makefile and/or Codeblocks project files (without the "_win" suffix).

The addon should work fine in Windows, although there are no example build files yet.

Creating a New ofxMidi Project
------------------------------

To develop your own project based on ofxMidi, simply copy the example project and rename it. You probably want to put it in your apps folder, for example, after copying:
<pre>
openFrameworks/addons/ofxPd/example-input/ => openFrameworks/apps/myApps/example-input/
</pre>

It must be 3 levels down in the openframeworks folder structure.

Then rename the folder:
<pre>
openFrameworks/apps/myApps/myPdProject/
</pre>

#### For Xcode:

Rename the project in Xcode (do not rename the .xcodeproj file in Finder!):
* Xcode Menu->Project->Rename

#### For Codeblocks (Win & Linux):

* rename the *.cbp and *.workspace files
* open the workspace and readd the renamed project file by dragging it onto the project tree (it will complain about the missing project you renamed)
* if you renamed the project *folder* make sure to set the project name to this folder name or C::B will not be able to run the binary:
	* right click on project in the tree (not the workspace)
	* Properties...->Title


Adding ofxMidi to an Existing Project
-------------------------------------

If you want to add ofxMidi to another project, you may need to make sure to include the src folder:
<pre>
openFrameworks/addons/ofxMidi/src
</pre>

### Xcode

* create a new group "ofxMidi"
* drag these directories from ofxMidi into this new group: ofxMidi/src
* add the CoreMIDI framework to your project
	* Xcode 3
		* right click on your project in the groups & files sidebar
		* Add > Existing frameworks
		* select CoreMIDI.framework from the list
	* Xcode 4
		* click on your project in the sidebar
		* select the Summary tab
		* click the + under Linked Frameworks & Libraries
		* search for and select the CoreMIDI.framework from the list

### Linux Makefiles/CodeBlocks

* edit addons.make in your project folder and add the following line to the end of the file: 
	<pre>ofxMidi</pre>

DEVELOPING
----------

You can help develop ofxMidi on GitHub: [https://github.com/chrisoshea/ofxMidi](https://github.com/chrisoshea/ofxMidi)

Create an account, clone or fork the repo, then request a push/merge. Please use the *develop* branch of updates and pull requests.

If you find any bugs or suggestions please log them to GitHub as well.

### Updating RtMidi

RtMidi can be updated by running the `update_rtmidi.sh` shell script in the scripts folder. Edit the version setting in the script header and run the script to download and place the RtMidi sources into `src/rtmidi`.

Next, make sure to add the following include to `RtMidi.cpp` at around line 38 or there will be link errors:
<pre>
#include "ofxConstants.h"
</pre>

