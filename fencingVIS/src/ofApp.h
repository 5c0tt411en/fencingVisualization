#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"

#define W 1680
#define H 84
#define NUM 300
#define DEBUG

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
    
    ofVec2f smoothing(vector <ofVec2f> v, float range);
    void graph(vector <ofVec2f> v, ofColor c, int n, ofVec2f p);
    void graphDEB(ofColor c, vector <ofVec2f> v, ofVec2f p);
    
    vector <ofVec2f> rawHistoryP1, rawHistoryP2;
    vector <ofVec2f> smoothedDataP1, smoothedDataP2;
    
    ofxSyphonServer server;
    ofFbo fbo;
};
