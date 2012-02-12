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
-------------------------------

If you're using OS X, open an Xcode project in ofxMidi/example-input/ or ofxMidi/example-output/ and hit "Build and Run".

The addon should work fine in Windows and Linux, although there are no example build files yet.

Adding ofxMidi to an Existing Project
---------------------------------------

If you want to add ofxMidi to another project, you need to make sure you include the src folders:
<pre>
openFrameworks/addons/ofxMidi/src
</pre>

For Xcode:

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

DEVELOPING
----------

You can help develop ofxMidi on GitHub: [https://github.com/chrisoshea/ofxMidi](https://github.com/chrisoshea/ofxMidi)

Create an account, clone or fork the repo, then request a push/merge. Please use the *develop* branch of updates and pull requests.

If you find any bugs or suggestions please log them to GitHub as well.
