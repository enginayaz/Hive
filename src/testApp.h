#pragma once

#include "ofMain.h"
#include "Sensor.h"
#include "StatusReader.h"
#include "Seat.h"
#include "ofxTextSuite.h"


// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS



enum Mode{FULL, TEST_GRAPHICS, TEST_TRACKING, TEST_WEB};

class testApp : public ofBaseApp {
public:
	
	void setup();
	void update();
    void draw();	
	void exit();	
	
	void keyPressed(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);    
    
    Sensor sensor;
    StatusReader statusReader;
    Seat seats[4];
    
    vector<ofImage> halos;
    vector<ofImage> logos;
    vector<ofImage> smallLogos;
    ofImage hiveLogo;
    
    
    time_t lastWebUpdate;
    int webUpdateInterval;
    
private:
    void draw(Seat seat);
    void drawSeats();   
    void loadImages();
    
    Mode mode;
    
    
    //dateTime lastTimeReadStatus
    ofTrueTypeFont	verdana14; //font sizes?
    ofTrueTypeFont	verdana30;
    //vector<ofxTextBlock> textBoxes;
    //TextBlockAlignment  alignment;
   
};
