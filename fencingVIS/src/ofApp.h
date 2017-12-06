#pragma once

#include "ofMain.h"
#include "ofxSyphon.h"
#include "ofxOsc.h"
#include "ofxGui.h"

#include "particles.hpp"

#define W 1680
#define H 84
#define NUM 300
//#define DEBUG

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
    
    ofxOscReceiver receiver;
    vector <string> items;
    char address;

    ofxPanel gPanel1, gPanel2;
    ofParameter<float> ptSz1,
    ptEmNoise1,
    ptTS1,
    ptSc1,
    ptSpXY1,
    ptSpZ1,
    ptSz2,
    ptEmNoise2,
    ptTS2,
    ptSc2,
    ptSpXY2,
    ptSpZ2;
    
    ofVec2f data1, data2;
    
    particles* particles1 = new particles();
    particles* particles2 = new particles();
};
