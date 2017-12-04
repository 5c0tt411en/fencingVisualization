#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(54, 54, 54);
    ofSetVerticalSync(true);
    
    history.assign(400, ofVec2f());
//    history
}

//--------------------------------------------------------------
void ofApp::update(){
    
#ifdef DEBUG
    ofVec2f data = ofVec2f(
                            ofNoise(ofGetElapsedTimef()) * 400,
                            ofNoise(ofGetElapsedTimef()/2) * 400
                            );
#endif
    history.push_back(data);
    if(history.size() >= 400){
        history.erase(history.begin(), history.begin()+1);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    //draw the graph of data history
    //1: position
    ofPushStyle();
        ofPushMatrix();
        ofTranslate(20, 300, 0);
        ofNoFill();
        ofSetColor(225);
        ofDrawRectangle(0, 0, 400, 400);
        ofSetColor(245, 58, 135);
        ofFill();
        ofBeginShape();
        for (unsigned int i = 0; i < history.size(); i++){
            if( i == 0 ) ofVertex(i, 400);
            ofVertex(i, 400 - history[i].x);
            if( i == history.size() - 1 ) ofVertex(i, 400);
        }
        ofEndShape(false);
        ofPopMatrix();
    ofPopStyle();
    
    //2: stamp strength
    ofPushStyle();
        ofPushMatrix();
        ofTranslate(410 + 20, 300, 0);
        ofNoFill();
        ofSetColor(225);
        ofDrawRectangle(0, 0, 400, 400);
        ofSetColor(245, 58, 135);
        ofFill();
        ofBeginShape();
        for (unsigned int i = 0; i < history.size(); i++){
            if( i == 0 ) ofVertex(i, 400);
            ofVertex(i, 400 - history[i].y);
            if( i == history.size() - 1 ) ofVertex(i, 400);
        }
        ofEndShape(false);
        ofPopMatrix();
    ofPopStyle();
    
    
    ofSetLineWidth(3);
    ofNoFill();
    ofSetColor(255, 0, 0);
    ofDrawRectangle(0, 0, W, H);
    ofSetColor(0, 255, 0);
    ofDrawRectangle(W, 0, W, H);
    ofSetLineWidth(1);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
