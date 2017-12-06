//
//  particles.cpp
//  fencingVIS
//
//  Created by scott on 2017/12/06.
//

#include "particles.hpp"

particles::particles() {
    
}

void particles::setup() {
    ofDisableAlphaBlending();
    particleNum = 200000;
    texRes = ceil(sqrt(particleNum));
    
    render.setGeometryInputType(GL_POINTS);
    render.setGeometryOutputType(GL_TRIANGLE_STRIP);
    render.setGeometryOutputCount(6);
    render.load("shaders/gpuParticle/render");
    //    render.load("shaders/gpuParticle/render.vert","shaders/gpuParticle/render.frag","shaders/gpuParticle/render.geom");
    
    updatePos.load("","shaders/gpuParticle/update.frag");
    
    parCol.set(255, 57, 143);
    
    particle.setMode(OF_PRIMITIVE_POINTS);
    for(int i=0;i<texRes;i++){
        for(int j=0;j<texRes;j++){
            int index = i * texRes + j;
            if(index < particleNum){
                particle.addVertex(ofVec3f(0,0,0));
                particle.addTexCoord(ofVec2f(i, j));
                particle.addColor(parCol);
            }
        }
    }
    
    pingPong.allocate(texRes, texRes, GL_RGBA32F, 2);
    
    float * posAndAge = new float[texRes * texRes * 4];
    for (int x = 0; x < texRes; x++){
        for (int y = 0; y < texRes; y++){
            int i = texRes * y + x;
            posAndAge[i*4 + 0] = ofRandom(-1.0,1.0);
            posAndAge[i*4 + 1] = ofRandom(-1.0,1.0);
            posAndAge[i*4 + 2] = ofRandom(-1.0,1.0);
            posAndAge[i*4 + 3] = 0;
        }
    }
    
    pingPong.src->getTexture(0).loadData(posAndAge, texRes, texRes, GL_RGBA);
    delete [] posAndAge;
    
    float * velAndMaxAge = new float[texRes * texRes * 4];
    for (int x = 0; x < texRes; x++){
        for (int y = 0; y < texRes; y++){
            int i = texRes * y + x;
            velAndMaxAge[i*4 + 0] = 0.0;
            velAndMaxAge[i*4 + 1] = 0.0;
            velAndMaxAge[i*4 + 2] = 0.0;
            velAndMaxAge[i*4 + 3] = ofRandom(1,150);
        }
    }
    
    pingPong.src->getTexture(1).loadData(velAndMaxAge, texRes, texRes, GL_RGBA);
    delete [] velAndMaxAge;
    
    showTex = false;
    
    sparkImg.load("images/spark.png");
    imgWidth = sparkImg.getWidth();
    imgHeight = sparkImg.getHeight();
}

void particles::update(vector <ofVec2f> v, ofColor c, ofVec2f p, float ptTS, float ptSc, float ptSpXY, float ptSpZ) {
    for (int i = 0; i < particleNum; i++) particle.setColor(i, c);
    float time = ofGetElapsedTimef();
    
    //update stamp strength
    ptSc = v.back().y;
    
    //update position
    prevEmitterPos = emitterPos;
    emitterPos = ofVec3f(p.x + v.back().x,
                         (H / 2 + (ofNoise(ofGetElapsedTimef() * 10) - 0.5) * v.back().y * H),
                         0);
    
    pingPong.dst->begin();
    
    pingPong.dst->activateAllDrawBuffers();
    ofClear(0);
    updatePos.begin();
    
    updatePos.setUniformTexture("u_posAndAgeTex", pingPong.src->getTexture(0), 0);
    
    updatePos.setUniformTexture("u_velAndMaxAgeTex", pingPong.src->getTexture(1), 1);
    updatePos.setUniform1f("u_time", time);
    updatePos.setUniform1f("u_timestep", ptTS);
    updatePos.setUniform1f("u_scale", ptSc);
    updatePos.setUniform1f("ptSpXY", ptSpXY);
    updatePos.setUniform1f("ptSpZ", ptSpZ);
    //    updatePos.setUniform1f("emitterPosNoise", ptNoise);
    updatePos.setUniform3f("u_emitterPos", emitterPos.x, emitterPos.y, emitterPos.z);
    updatePos.setUniform3f("u_prevEmitterPos", prevEmitterPos.x, prevEmitterPos.y, prevEmitterPos.z);
    pingPong.src->draw(0, 0);
    updatePos.end();
    pingPong.dst->end();
    pingPong.swap();
}

void particles::draw() {
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    render.begin();
    //    render.setUniform1f("pointSize", ptSz);
    render.setUniformTexture("u_posAndAgeTex", pingPong.src->getTexture(0), 0);
    render.setUniformTexture("sparkTex", sparkImg.getTexture(), 1);
    render.setUniform1f("size", (float)ptSz);
    render.setUniform1i("resolution", (float)texRes);
    render.setUniform1f("imgWidth", (float)sparkImg.getWidth());
    render.setUniform1f("imgHeight", (float)sparkImg.getHeight());
    
    ofPushStyle();
    ofEnableBlendMode( OF_BLENDMODE_ADD );
    ofSetColor(255);
    
    particle.draw();
    
    ofDisableBlendMode();
    glEnd();
    
    render.end();
    
    //    ofDisablePointSprites();
    ofPopStyle();

}
