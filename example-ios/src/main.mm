#include "ofMain.h"
#include "testApp.h"

int main(){
	ofSetupOpenGL(1024,768, OF_FULLSCREEN);			// <-------- setup the GL context

	ofSetLogLevel(OF_LOG_VERBOSE);
	ofRunApp(new testApp);
}
