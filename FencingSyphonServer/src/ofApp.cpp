#include "ofMain.h"
#include "ofxSyphon.h"

#define OPTION 1

static const int width  = 3360;
static const int height = 84;

class ofApp : public ofBaseApp
{
public:
    ofFbo fbo;
    ofxSyphonServer server;
    
    void setup()
    {
#if OPTION == 0
        fbo.allocate(width, height * 2, GL_RGBA, 8);
#elif OPTION == 1
        fbo.allocate(width, height, GL_RGBA, 8);
#endif
        
        server.setName("Screen Output");
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        fbo.begin();
        {
            ofClear(0);
            
#if OPTION == 0
            // -- 1 (Red)
            ofPushMatrix();
            ofTranslate(0, 0);
            ofPushStyle();
            ofSetColor(ofColor::red);
            ofDrawRectangle(0, 0, width/2, height);
            ofPopStyle();
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(0, 0, 20, 20);
            ofPopStyle();
            ofPopMatrix();
            
            // -- 2 (Green)
            ofPushMatrix();
            ofTranslate(0, height);
            ofPushStyle();
            ofSetColor(ofColor::green);
            ofDrawRectangle(0, 0, width/2, height);
            ofPopStyle();
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(0, 0, 20, 20);
            ofPopStyle();
            ofPopMatrix();
            
            // -- 3 (Blue)
            ofPushMatrix();
            ofTranslate(width/2, height);
            ofPushStyle();
            ofSetColor(ofColor::blue);
            ofDrawRectangle(0, 0, width/2, height);
            ofPopStyle();
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(0, 0, 20, 20);
            ofPopStyle();
            ofPopMatrix();
            
            // -- 4 (Yellow)
            ofPushMatrix();
            ofTranslate(width/2, 0);
            ofPushStyle();
            ofSetColor(ofColor::yellow);
            ofDrawRectangle(0, 0, width/2, height);
            ofPopStyle();
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(0, 0, 20, 20);
            ofPopStyle();
            ofPopMatrix();
#elif OPTION == 1
            // -- 1 (Red)
            ofPushMatrix();
            ofTranslate(0, 0);
            ofPushStyle();
            ofSetColor(ofColor::red);
            ofDrawRectangle(0, 0, width/2, height);
            ofPopStyle();
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(0, 0, 20, 20);
            ofPopStyle();
            ofPopMatrix();
            
            // -- 2 (Green)
            ofPushMatrix();
            ofTranslate(width/2, 0);
            ofPushStyle();
            ofSetColor(ofColor::green);
            ofDrawRectangle(0, 0, width/2, height);
            ofPopStyle();
            ofPushStyle();
            ofSetColor(255);
            ofDrawRectangle(0, 0, 20, 20);
            ofPopStyle();
            ofPopMatrix();
#endif
        }
        fbo.end();
        
        fbo.draw(0, 0);
        
        server.publishTexture(&fbo.getTexture());
    }
    
    void keyPressed(int key)
    {
        
    }
    
};


int main(int argc, const char** argv)
{
    ofSetupOpenGL(1920, 1080, OF_WINDOW);
    ofRunApp(new ofApp);
    return 0;
}

