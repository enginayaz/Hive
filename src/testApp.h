#pragma once

#include "ofMain.h"
#include "Sensor.h"
#include "Seat.h"
#include "ofxTextSuite.h"
#include "ofxHttpUtils.h"


// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS





enum Mode{FULL, TEST_GRAPHICS, TEST_TRACKING, TEST_WEB};

class testApp : public ofBaseApp{
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
    
    void newStatuses();
    
    Sensor sensor;
    Seat seats[4];
    
    vector<ofImage> halos;
    vector<ofImage> logos;
    vector<ofImage> smallLogos;
    ofImage hiveLogo;
    
    
    time_t lastWebUpdate;
    int webUpdateInterval;
    int numberOfSeats;
    
private:
    void draw(Seat seat);
    void drawSeats();   
    void setupImagesAndFonts();
    
    Mode mode;
    
    
    //dateTime lastTimeReadStatus
    ofTrueTypeFont	verdana14; //font sizes?
    ofTrueTypeFont	verdana30;
    vector<ofxTextBlock> statusTextBoxes;
    ofxTextBlock welcomeText;
    TextBlockAlignment  alignment;
    int maxTextWidth;
    
    vector<string> seatURLs;
    
    //WEB
    ofxHttpUtils httpUtils;
    int counter;
    string responseStr;
    string requestStr;
    string action_url;    
    void requestNewStatuses();
    void newResponse(ofxHttpResponse & response);
    
    int firstRectLeft;
    int firstRectRight;
    int firstRectTop;
    int firstRectBottom;
    
    int secondRectLeft;
    int secondRectRight;
    int secondRectTop;
    int secondRectBottom;
    
    int thirdRectLeft;
    int thirdRectRight;
    int thirdRectTop;
    int thirdRectBottom;
    
    int fourthRectLeft;
    int fourthRectRight;
    int fourthRectTop;
    int fourthRectBottom;
   
};
