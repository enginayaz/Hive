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
    void draw(ofBaseApp *parent);
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
    ofxCvGrayscaleImage 	grayImage;
    ofxCvGrayscaleImage 	depthBg;
    ofxCvGrayscaleImage 	grayDiff;
    
    ofxCvContourFinder 	contourFinder;
    
    int 				bckSubstractionThreshold;
    bool				bLearnBackground;
	
	
	bool bThreshWithOpenCV;
	bool bDrawPointCloud;
	
	int nearThreshold;
	int farThreshold;
	
	int angle;
	
	// used for viewing the point cloud
	ofEasyCam easyCam;
    
    
};
#endif
