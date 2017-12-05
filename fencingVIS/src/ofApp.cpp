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
}

//--------------------------------------------------------------
void ofApp::update(){
    
#ifdef DEBUG
    ofVec2f data1 = ofVec2f(
                            ofNoise(ofGetElapsedTimef()) * NUM * ofRandom(0.8, 1.2),
                            ofNoise(ofGetElapsedTimef()/2) * ofRandom(0.8, 1.2)
                            );
    ofVec2f data2 = ofVec2f(
                            ofNoise(ofGetElapsedTimef()/3) * NUM * ofRandom(0.8, 1.2),
                            ofNoise(ofGetElapsedTimef()/4) * ofRandom(0.8, 1.2)
                            );
#else
    
#endif
    rawHistoryP1.push_back(data1);
    if(rawHistoryP1.size() >= NUM){
        rawHistoryP1.erase(rawHistoryP1.begin(), rawHistoryP1.begin()+1);
    }
    rawHistoryP2.push_back(data2);
    if(rawHistoryP2.size() >= NUM){
        rawHistoryP2.erase(rawHistoryP2.begin(), rawHistoryP2.begin()+1);
    }
    
    fbo.allocate(W, H, GL_RGBA, 8);
}

//--------------------------------------------------------------
void ofApp::draw(){
    fbo.begin();
    ofClear(255);
    
    ofFill();
    ofSetColor(0);
    ofDrawRectangle(0, 0, 2 * W, H);
    ofSetColor(255);
    ofDrawCircle(ofRandom(2 * W), H / 2, 50);
    ofSetColor(255);

    fbo.end();
    
    fbo.draw(0, 0);
    
    server.publishTexture(&fbo.getTexture());
    
    
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
    graph(rawHistoryP2, ofColor(58, 245, 135), NUM, ofVec2f(680, 300));
    //smoothing
    smoothedDataP2.push_back(smoothing(rawHistoryP2, .1));
    smoothedDataP2.erase(smoothedDataP2.begin());
    //SMOOTHED DATA======================================
    graph(smoothedDataP2, ofColor(58, 135, 245), NUM, ofVec2f(680, 630));
    
    
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
}



//--------------------------------------------------------------
ofVec2f ofApp::smoothing(vector <ofVec2f> v, float range) {
    ofVec2f smoothed;
    for (int i = int(NUM * (1. - range)); i < NUM; i++) {
        smoothed.x += v[i].x;
        ofLog() << v[i] << endl;
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
            ofVertex(i, n - v[i].x);
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
