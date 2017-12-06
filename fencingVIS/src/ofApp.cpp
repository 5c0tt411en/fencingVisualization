#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(54, 54, 54);
    ofSetVerticalSync(true);
    
    rawHistoryP1.assign(NUM, ofVec2f());
    smoothedDataP1.assign(NUM, ofVec2f());
    rawHistoryP2.assign(NUM, ofVec2f());
    smoothedDataP2.assign(NUM, ofVec2f());
    
    server.setName("visual");
    fbo.allocate(2 * W, H, GL_RGBA);
    
#ifdef DEBUG
#else
    receiver.setup(8888);
#endif
    gPanel1.setup();
    gPanel1.setPosition(680, 300);
    gPanel1.add(ptSz1.set("point size", 1.0f, 1.0f, 100.0f));
    gPanel1.add(ptEmNoise1.set("point noise", 30.0, 0.0, 500.0));
    gPanel1.add(ptTS1.set("point time step", 0.5, 0.0, 1.0));
    gPanel1.add(ptSc1.set("point scale", 0.005, 0.0, 0.1));
    gPanel1.add(ptSpXY1.set("point speed X, Y", 0.0, 0.0, 1.0));
    gPanel1.add(ptSpZ1.set("point speed Z", 0.05, 0.0, 1.0));
    
    gPanel2.setup();
    gPanel2.setPosition(680 + W, 300);
    gPanel2.add(ptSz2.set("point size", 1.0f, 1.0f, 100.0f));
    gPanel2.add(ptEmNoise2.set("point noise", 30.0, 0.0, 500.0));
    gPanel2.add(ptTS2.set("point time step", 0.5, 0.0, 1.0));
    gPanel2.add(ptSc2.set("point scale", 0.005, 0.0, 0.1));
    gPanel2.add(ptSpXY2.set("point speed X, Y", 0.0, 0.0, 1.0));
    gPanel2.add(ptSpZ2.set("point speed Z", 0.05, 0.0, 1.0));
    
    particles1->setup();
    particles2->setup();
}

//--------------------------------------------------------------
void ofApp::update(){
    
#ifdef DEBUG
    data1 = ofVec2f(
                            ofNoise(ofGetElapsedTimef()) * W * ofRandom(0.8, 1.2),
                            ofNoise(ofGetElapsedTimef()/2) * ofRandom(0.8, 1.2)
                            );
    data2 = ofVec2f(
                            ofNoise(ofGetElapsedTimef()/3) * W * ofRandom(0.8, 1.2),
                            ofNoise(ofGetElapsedTimef()/4) * ofRandom(0.8, 1.2)
                            );
#else
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(m);
        items = ofSplitString(m.getAddress(), "/");
        if (items[1] == "data") {
            data1 = ofVec2f(
                            m.getArgAsFloat(0),
                            m.getArgAsFloat(1)
                            );
            data2 = ofVec2f(
                            m.getArgAsFloat(2),
                            m.getArgAsFloat(3)
                            );
        }
    }
#endif
    rawHistoryP1.push_back(data1);
    if(rawHistoryP1.size() >= NUM){
        rawHistoryP1.erase(rawHistoryP1.begin(), rawHistoryP1.begin()+1);
    }
    rawHistoryP2.push_back(data2);
    if(rawHistoryP2.size() >= NUM){
        rawHistoryP2.erase(rawHistoryP2.begin(), rawHistoryP2.begin()+1);
    }
    
    particles1->update(smoothedDataP1, ofColor(245, 135, 58), ofVec2f(0,0), ptTS1, ptSc1, ptSpXY1, ptSpZ1);
    particles2->update(smoothedDataP2, ofColor(58, 245, 135), ofVec2f(W,0), ptTS2, ptSc2, ptSpXY2, ptSpZ2);
}

//--------------------------------------------------------------
void ofApp::draw(){
    //player1 graph
    //RAW DATA======================================
    graph(rawHistoryP1, ofColor(245, 135, 58), NUM, ofVec2f(20, 300));
    //smoothing
    smoothedDataP1.push_back(smoothing(rawHistoryP1, .1));
    smoothedDataP1.erase(smoothedDataP1.begin());
    //SMOOTHED DATA======================================
    graph(smoothedDataP1, ofColor(135, 58, 245), NUM, ofVec2f(20, 630));
    
    //player2 graph
    //RAW DATA======================================
    graph(rawHistoryP2, ofColor(58, 245, 135), NUM, ofVec2f(W + 20, 300));
    //smoothing
    smoothedDataP2.push_back(smoothing(rawHistoryP2, .1));
    smoothedDataP2.erase(smoothedDataP2.begin());
    //SMOOTHED DATA======================================
    graph(smoothedDataP2, ofColor(58, 135, 245), NUM, ofVec2f(W + 20, 630));
    
    fbo.begin();
    ofClear(0);
    
    ofSetColor(0);
    ofDrawRectangle(0, 0, 2 * W, H);
    ofSetColor(255);
    
    particles1->draw();
    particles2->draw();
    
    ofSetColor(255);
    fbo.end();
    
    ofSetColor(255);
    fbo.draw(0, 0);
    
    server.publishTexture(&fbo.getTexture());
    

    
    ofSetLineWidth(3);
    ofNoFill();
    ofSetColor(245, 135, 58);
    ofDrawRectangle(0, 0, W, H);
    ofSetLineWidth(1);

    ofSetLineWidth(3);
    ofSetColor(58, 245, 135);
    ofDrawRectangle(W, 0, W, H);
    ofSetLineWidth(1);
    
    graphDEB(ofColor(135, 58, 245), smoothedDataP1, ofVec2f(0, H * 3 / 2));
    graphDEB(ofColor(58, 135, 245), smoothedDataP2, ofVec2f(W, H * 3 / 2));
    ofFill();
    
    gPanel1.draw();
    gPanel2.draw();
}



//--------------------------------------------------------------
ofVec2f ofApp::smoothing(vector <ofVec2f> v, float range) {
    ofVec2f smoothed;
    for (int i = int(NUM * (1. - range)); i < NUM; i++) {
        smoothed.x += v[i].x;
        if (i == v.size() - 1) smoothed.x /= NUM * range;
        smoothed.y += v[i].y;
        if (i == v.size() - 1) smoothed.y /= NUM * range;
    }
    return smoothed;
}

//--------------------------------------------------------------
void ofApp::graph(vector <ofVec2f> v, ofColor c, int n, ofVec2f p) {
    //position
    ofPushStyle();
        ofPushMatrix();
        ofTranslate(p.x, p.y, 0);
        ofNoFill();
        ofSetColor(225);
        ofDrawRectangle(0, 0, n, n);
        ofDrawBitmapString("position: " + ofToString(v.back().x), 20, 20);
        ofSetColor(c);
        ofFill();
        ofBeginShape();
        for (unsigned int i = 0; i < v.size(); i++){
            if( i == 0 ) ofVertex(i, n);
            ofVertex(i, n - v[i].x * n / W);
            if( i == v.size() - 1 ) ofVertex(i, n);
        }
        ofEndShape(false);
        ofPopMatrix();
    ofPopStyle();
    
    //stamp strength
    ofPushStyle();
        ofPushMatrix();
        ofTranslate(p.x + n + 30, p.y, 0);
        ofNoFill();
        ofSetColor(225);
        ofDrawRectangle(0, 0, n, n);
        ofDrawBitmapString("stamp strength: " + ofToString(v.back().y), 20, 20);
        ofSetColor(c);
        ofFill();
        ofBeginShape();
        for (unsigned int i = 0; i < v.size(); i++){
            if( i == 0 ) ofVertex(i, n);
            ofVertex(i, n - v[i].y * n);
            if( i == v.size() - 1 ) ofVertex(i, n);
        }
        ofEndShape(false);
        ofPopMatrix();
    ofPopStyle();
}

void ofApp::graphDEB(ofColor c, vector <ofVec2f> v, ofVec2f p) {
    ofSetColor(c);
    ofDrawLine(p.x + v.back().x - 40, p.y, p.x + v.back().x + 40, p.y);
    ofDrawLine(p.x + v.back().x, p.y - 40, p.x + v.back().x, p.y + 40);
    ofDrawCircle(p.x + v.back().x, p.y, v.back().y * 50.);
    string info;
    info += "      smoothed position: " + ofToString(v.back().x) + '\n';
    info += "smoothed stamp strength: " + ofToString(v.back().y);
    ofDrawBitmapStringHighlight(info, p.x + v.back().x + 10, p.y + 20);
}
