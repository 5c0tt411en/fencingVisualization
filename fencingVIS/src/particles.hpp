//
//  particles.hpp
//  fencingVIS
//
//  Created by scott on 2017/12/06.
//


#ifndef particles_hpp
#define particles_hpp

#include "ofMain.h"

#include "pingPongBuffer.h"
#include "ofMain.h"

#define W 1680
#define H 84

class particles {
public:
    particles();
    void setup();
    void draw();
    void update(vector <ofVec2f> v, ofColor c, ofVec2f p, float ptTS, float ptSc, float ptSpXY, float ptSpZ);
    
    ofVboMesh particle;
    ofShader render, updatePos;
    pingPongBuffer pingPong;
    ofVec3f emitterPos, prevEmitterPos;
    int particleNum, texRes;
    bool showTex;
    ofColor parCol;
    float ptSz, audio, ptEmNoise, ptTS, ptSc, ptSpXY, ptSpZ;
    
    ofImage sparkImg;
    int     imgWidth, imgHeight;
};

#endif /* particles_hpp */

