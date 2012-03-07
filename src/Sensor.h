//
//  Sensor.h
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ofxKinectExample_Sensor_h
#define ofxKinectExample_Sensor_h



#include "ofxOpenCv.h"
#include "ofxKinect.h"
using namespace std;


class Sensor{
public: 
    Sensor();
    void update();
    void draw(/*ofBaseApp *parent*/);
    void close();
    vector<ofxCvBlob> getContours();
    
    void drawPointCloud();
    void keyPressed(int key);
    
    ofxKinect kinect;
	
#ifdef USE_TWO_KINECTS
	ofxKinect kinect2;
#endif
	
	ofxCvColorImage colorImg;
	
	ofxCvGrayscaleImage grayDepthImage; // grayscale depth image
	ofxCvGrayscaleImage grayThreshNear; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFar; // the far thresholded image
    
    
    ofxCvGrayscaleImage 	grayBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    
    bool	bLearnBakground;
    int     threshold;
    
    
    ofxCvContourFinder 	contourFinder;

	
	
	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
    
    /*
     ofPoint A;
     ofPoint B;
     ofPoint C;
     ofPoint D;
     */
	
	int nearThreshold;
	int farThreshold;
	
	int angle;
	
	// used for viewing the point cloud
	ofEasyCam easyCam;
    
    int minArea;
    int maxArea;
    
    ofQuaternion refRectangles[4];
    
    
};
#endif
