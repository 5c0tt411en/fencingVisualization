#include "ofMain.h"
#include "ofxSyphon.h"

#define OPTION 1

static const int width  = 3360;
static const int height = 84;

class ofApp : public ofBaseApp
{
public:
    ofxSyphonServerDirectory dir;
    ofxSyphonClient client;
    int dirIdx;
    
    void setup()
    {
        ofSetFrameRate(60);
        
        dir.setup();
        client.setup();
        
        ofAddListener(dir.events.serverAnnounced, this, &ofApp::serverAnnounced);
        ofAddListener(dir.events.serverRetired, this, &ofApp::serverRetired);
        
        dirIdx = -1;
    }
    
    void update()
    {
        
    }
    
    void draw()
    {
        if ( dir.isValidIndex(dirIdx) )
        {
#if OPTION == 0
            // -- 1 (Red)
            ofPushMatrix();
            ofTranslate(height*0, 0);
            ofTranslate(height/2, width/4);
            ofRotate(-90, 0, 0, 1);
            ofTranslate(-width/4, -height/2);
            client.drawSubsection(0, 0, width/2, height, 0, height, width/2, height);
            ofPopMatrix();
            
            // -- 2 (Green)
            ofPushMatrix();
            ofTranslate(height*1, 0);
            ofTranslate(height/2, width/4);
            ofRotate(-90, 0, 0, 1);
            ofTranslate(-width/4, -height/2);
            client.drawSubsection(0, 0, width/2, height, 0, 0, width/2, height);
            ofPopMatrix();
            
            // -- 3 (blue)
            ofPushMatrix();
            ofTranslate(height*2, 0);
            ofTranslate(height/2, width/4);
            ofRotate(-90, 0, 0, 1);
            ofTranslate(-width/4, -height/2);
            client.drawSubsection(0, 0, width/2, height, width/2, 0, width/2, height);
            ofPopMatrix();
            
            // -- 4 (yellow)
            ofPushMatrix();
            ofTranslate(height*3, 0);
            ofTranslate(height/2, width/4);
            ofRotate(-90, 0, 0, 1);
            ofTranslate(-width/4, -height/2);
            client.drawSubsection(0, 0, width/2, height, width/2, height, width/2, height);
            ofPopMatrix();
#elif OPTION == 1
            // -- 1 (Red)
            ofPushMatrix();
            {
                bool flip = true;
                ofTranslate(height*0, flip ? -width/2 : 0);
                ofTranslate(height/2, width/4);
                ofRotate(-90, 0, 0, 1);
                ofTranslate(-width/4, -height/2);
                client.drawSubsection(0, 0, flip ? -width/2 : width/2, height, 0, 0, width/2, height);
            }
            ofPopMatrix();
            
            // -- 2 (Red)
            ofPushMatrix();
            {
                bool flip = false;
                ofTranslate(height*1, flip ? -width/2 : 0);
                ofTranslate(height/2, width/4);
                ofRotate(-90, 0, 0, 1);
                ofTranslate(-width/4, -height/2);
                client.drawSubsection(0, 0, flip ? -width/2 : width/2, height, 0, 0, width/2, height);
            }
            ofPopMatrix();
            
            // -- 3 (Green)
            ofPushMatrix();
            {
                bool flip = false;
                ofTranslate(height*2, flip ? -width/2 : 0);
                ofTranslate(height/2, width/4);
                ofRotate(-90, 0, 0, 1);
                ofTranslate(-width/4, -height/2);
                client.drawSubsection(0, 0, flip ? -width/2 : width/2, height, width/2, 0, width/2, height);
            }
            ofPopMatrix();
            
            // -- 4 (Green)
            ofPushMatrix();
            {
                bool flip = true;
                ofTranslate(height*3, flip ? -width/2 : 0);
                ofTranslate(height/2, width/4);
                ofRotate(-90, 0, 0, 1);
                ofTranslate(-width/4, -height/2);
                client.drawSubsection(0, 0, flip ? -width/2 : width/2, height, width/2, 0, width/2, height);
            }
            ofPopMatrix();
#endif
        }
    }
    
    void serverAnnounced(ofxSyphonServerDirectoryEventArgs &arg)
    {
        for ( auto& dir : arg.servers )
        {
            ofLogNotice("ofxSyphonServerDirectory Server Announced") << " Server Name: " << dir.serverName << " | App Name: " << dir.appName;
        }
        dirIdx = 0;
    }
    
    void serverUpdated(ofxSyphonServerDirectoryEventArgs &arg)
    {
        for ( auto& dir : arg.servers )
        {
            ofLogNotice("ofxSyphonServerDirectory Server Updated") << " Server Name: " << dir.serverName << " | App Name: " << dir.appName;
        }
        dirIdx = 0;
    }
    
    void serverRetired(ofxSyphonServerDirectoryEventArgs &arg)
    {
        for ( auto& dir : arg.servers )
        {
            ofLogNotice("ofxSyphonServerDirectory Server Retired") << " Server Name: " << dir.serverName << " | App Name: " << dir.appName;
        }
        dirIdx = 0;
    }
    
    void keyPressed(int key)
    {
        if ( key == 'f' )
        {
            ofToggleFullscreen();
        }
        
        if ( key == ' ' )
        {
            if ( dir.size() > 0 )
            {
                dirIdx++;
                
                if( dirIdx > dir.size() - 1 ) dirIdx = 0;
                
                client.set(dir.getDescription(dirIdx));
                string serverName = client.getServerName();
                string appName = client.getApplicationName();
                
                if ( serverName == "" ) serverName = "null";
                if ( appName == "") appName = "null";
                
                ofSetWindowTitle( serverName + " : " + appName );
            }
        }
    }
    
};


int main(int argc, const char** argv)
{
    ofSetupOpenGL(1080, 1920, OF_WINDOW);
    ofRunApp(new ofApp);
    return 0;
}
