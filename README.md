ofxMidi
=======
<p align="center">
<img src="https://raw.github.com/danomatika/ofxMidi/master/res/midi_din.png"/>
</p>

Midi input and output addon for openFrameworks

Copyright (c) [Dan Wilcox](danomatika.com) 2011-2013<br/>
(original implementation by Chris O'Shea, Arturo Castro, Kyle McDonald)

BSD Simplified License.

For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "LICENSE.txt," in this distribution.

See Documentation on [Github](https://github.com/danomatika/ofxMidi) and the [OpenFrameworks Forum post](http://forum.openframeworks.cc/index.php/topic,2435.0.html).

OpenFrameworks is a cross platform open source toolkit for creative coding in C++.

[http://www.openframeworks.cc](http://www.openframeworks.cc)

Description
-----------

ofxMidi provides [Music Instrument Digial Interface](http://en.wikipedia.org/wiki/Musical_Instrument_Digital_Interface) IO capability to an OpenFrameworks app

* ofxMidiIn: a single midi input port, derive from the ofxMidiListener class to receive messages
* ofxMidiMessage: a received midi message
* ofxMidiOut: a single midi output port, includes a stream << interface

This project utilizes [RtMidi](http://www.music.mcgill.ca/~gary/rtmidi/) for Mac, Windows, & Linux and [PGMidi](https://github.com/petegoodliffe/PGMidi) on iOS.

Installation
------------

To use ofxMidi, first you need to download and install OpenFrameworks. ofxMidi is currently developed against the current stable version of Open Frameworks on github.

To get a copy of the repository you can download the source from [http://github.com/danomatika/ofxMidi/zipball/master](http://github.com/danomatika/ofxMidi/zipball/master) or, alternatively, you can use git clone:
<pre>
git clone git://github.com/danomatika/ofxMidi.git
</pre>

The addon should sit in `openFrameworks/addons/ofxMidi/`.

#### Which version to use?

If you are using a stable version (007, ...) of OpenFrameworks then you want to use a git tag of ofxMidi for that version. You can select the tag in the Github "Current Branch" menu or clone and check it out using git.

For example, the following commands will clone ofxMidi and switch to the OF 007 tagged version:
<pre>
git clone git://github.com/danomatika/ofxMidi.git
cd ofxPd
git checkout 007
</pre>

Midi Routing
------------

### Mac OSX

Checkout a useful app for midi port routing called [MIDI Patchbay](http://notahat.com/midi_patchbay).

### Linux

Check out the Alsa utility apps aconnect & aconnectgui as well as the qjackctl gui for midi port routing control.

### Windows

Windows dosen't come with a virtual MIDI routing system like Linux (ALSA) and OSX (CoreMIDI).

If you want to connect your ofxMidi app to other software (synths, DAWs, etc) check out [loopMIDI](http://www.tobias-erichsen.de/loopMIDI.html). Run the app and create a few virtual ports which you can then connect to within your software. 

Running the Example Project
---------------------------

The example projects are in the ofxMidi/example-input/ or ofxMidi/example-output/ folders.

### OSX

Open the Xcode project, select the project scheme, and hit "Run".

### Linux

Open the Code::Blocks .workspace and hit F9 to build. Optionally, you can build the example with the Makefile.

To built and run it on the terminal:
<pre>
make
make run
</pre>

### Windows

An example Visual Studio solution as well as a Codeblocks workspace are included.

Creating a New ofxMidi Project
------------------------------

_Note: These instructions are for manually creating a new project. You do not need to follow these steps if you use the ProjecGenerator app, except for Xcode where you currently need to add the CoreMIDI framework._

To develop your own project based on ofxMidi, simply copy the example project and rename it. You probably want to put it in your apps folder, for example, after copying:
<pre>
openFrameworks/addons/ofxPd/exampleInput/ => openFrameworks/apps/myApps/exampleInput/
</pre>

It must be 3 levels down in the openframeworks folder structure.

Then rename the folder:
<pre>
openFrameworks/apps/myApps/myPdProject/
</pre>

#### Xcode

Rename the project in Xcode (do not rename the .xcodeproj file in Finder!):
* Xcode Menu->Project->Rename

#### Codeblocks (Win & Linux)

* rename the *.cbp and *.workspace files
* open the workspace and readd the renamed project file by dragging it onto the project tree (it will complain about the missing project you renamed)
* if you renamed the project *folder* make sure to set the project name to this folder name or C::B will not be able to run the binary:
	* right click on project in the tree (not the workspace)
	* Properties...->Title

#### Visual Studio

* rename the *.sln, *.vcxproj, & *.vcxproj.filters files
* open the solution and delete the old project from the projects tree
* go to File->Add->Existing Projects/Solutions and select the *.vcxproj file
* right click on the project in the projects tree and rename it

Adding ofxMidi to an Existing Project
-------------------------------------

_Note: These instructions are for manually adding ofxMidi to a project. You do not need to follow these steps if you use the ProjecGenerator app to regenerate your project files, except for Xcode where you currently need to add the CoreMIDI framework._

### Xcode

* create a new group "ofxMidi"
* drag these directories from ofxMidi into this new group: `ofxMidi/src`, `ofxMini/libs`
  * if building for OSX, remove the src/ios & libs/pgmidi folder references
  * if building for iOS, remove the src/desktop & libs/rtmidi folder references
* add the CoreMIDI framework to your project
  * click on your project in the sidebar
  * select the Summary tab
  * click the + under Linked Frameworks & Libraries
  * search for and select the CoreMIDI.framework from the list
* add the following directories to your search path in your project's Project.xconfig file (See the Project.xconfig of the example project.):
  <pre>
../../../addons/ofxMidi/src
../../../addons/ofxMidi/libs/rtmidi
</pre>

### Linux Makefiles/CodeBlocks

* edit addons.make in your project folder and add the following line to the end of the file: 
	  <pre>ofxMidi</pre>

### Win Codeblocks & Visual Studio

* add the ofxMidi sources to the project tree
  <pre>
ofxMidi/src
ofxMidi/libs/rtmidi
</pre>
  * Codeblocks: right click on the project in the project tree and select Add Files Recursively...
  * Visual Studio: drag the ofxMidi/src & ofxMidi/libs/rtmidi folder onto the project tree
* add the following search paths:
  <pre>
..\\..\\..\addons\ofxMidi\src
..\\..\\..\addons\ofxMidi\libs\rtmidi
</pre>
* Codeblocks
  * right click on the project in the project tree and select Build Options...
  * select the project name in the tree, not release or debug
  * Search directories tab->Add
* Visual Studio
  * right click on the project in the project tree and select Properties
  * set the Configuration to All Configurations
  * Configuration Properties->C/C++->General->Additional Directories

KNOWN ISSUES
------------

### Using static ofxMidi objects on Linux causes seg faults

Avoid creating static ofxMidiIn / ofxMidiOut objects on Linux as the compiler seems to set creation order so they are created *before* ALSA is ready. This leads to a confirmed seg fault on Ubuntu and probably all other flavors of Linux using ALSA. The midi apis on Windows and OSX do not share this problem. 

Instead create a static ofPtr and initialize it later:

    // in .h:

    class MyClass {
    
        ...

        static ofPtr<ofxMidiOut> s_midiOut;
    
        ...

    }

    // in .cpp:

    ofPtr<ofxMidiOut> MyClass::s_midiOut;
    
    ...
    
    // initialize somewhere else
    void MyClass::setup() {
	    if(s_midiOut == NULL) {
	        s_midiOut = ofPtr<ofxMidiOut>(new ofxMidiOut("ofxMidi Client"));
	    }
    }


DEVELOPING
----------

You can help develop ofxMidi on GitHub: [https://github.com/danomatika/ofxMidi](https://github.com/danomatika/ofxMidi)

Create an account, clone or fork the repo, then request a push/merge. Please use the *develop* branch of updates and pull requests.

If you find any bugs or suggestions please log them to GitHub as well.

### Adding a Midi Backend

If you want to add a new midi backend (Android, Jack, etc), you'll need two classes derived from ofxBaseMidiIn & ofxBaseMidiOut.

Place your source files in a new folder named after your platform/library and add new include #ifdef flags to ofxMidiIn.h & ofxMidiIn.cpp.

Last, you'll need to add specific #ifdef flags to the static port info ofxMidiIn/Out functions (listPorts, getPortName, etc).

### Updating Midi Libraries

RtMidi & PGMidi can be updated by running the `update_rtmidi.sh` or `update_pgmidi.sh` shell scripts in the scripts folder.

For RtMidi, edit the version setting in the script header and run the script to download and place the RtMidi sources into `libs/rtmidi`.

PGMidi sources are placed in `libs/pgmidi`.

#### RtMidi.cpp include

Next, make sure to add the following include to `RtMidi.cpp` at around line 38 or there will be link errors:

    #include "ofxConstants.h"

