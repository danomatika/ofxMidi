/*
 * Copyright (c) 2013 Dan Wilcox <danomatika@gmail.com>
 *
 * BSD Simplified License.
 * For information on usage and redistribution, and for a DISCLAIMER OF ALL
 * WARRANTIES, see the file, "LICENSE.txt," in this distribution.
 *
 * See https://github.com/danomatika/ofxMidi for documentation
 *
 */
#include "ofMain.h"
#include "ofApp.h"

// quick fix until 64bit OF on iOS,
// from https://github.com/openframeworks/openFrameworks/issues/3178
#if OF_VERSION_MAJOR == 0 && OF_VERSION_MINOR <= 8
extern "C" {
	size_t fwrite$UNIX2003(const void *a, size_t b, size_t c, FILE *d) {
		return fwrite(a, b, c, d);
	}
	char* strerror$UNIX2003(int errnum) {
		return strerror(errnum);
	}
	time_t mktime$UNIX2003(struct tm * a) {
		return mktime(a);
	}
	double strtod$UNIX2003(const char * a, char ** b) {
		return strtod(a, b);
	}
}
#endif

int main() {
	ofSetupOpenGL(1024, 768, OF_FULLSCREEN);
	ofRunApp(new ofApp);
}
