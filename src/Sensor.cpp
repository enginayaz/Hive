//
//  Sensor.cpp
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Sensor.h"

Sensor::Sensor(){
    ofSetLogLevel(OF_LOG_VERBOSE);
	
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	//kinect.init(); // shows RGB video image
	kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	kinect.open();
	
#ifdef USE_TWO_KINECTS
	kinect2.init();
	kinect2.open();
#endif
	
	colorImg.allocate(kinect.width, kinect.height);
	grayDepthImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    
    //colorImg.allocate(kinect.width, kinect.height);
	//grayImage.allocate(kinect.width, kinect.height);
	grayBg.allocate(kinect.width, kinect.height);
	grayDiff.allocate(kinect.width, kinect.height);
	
	nearThreshold = 200;
	farThreshold = 50;
	bThreshWithOpenCV = true;
    
    
	threshold = 1;
	
	ofSetFrameRate(60);
	
	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);
	
	// start from the front
	bDrawPointCloud = false;
    
    /*
     A.x = 0;
     A.y = kinect.height/2;
     B.x = kinect.width;
     B.y = kinect.height/2;
     C.x = kinect.width;
     C.y = kinect.height;
     D.x = 0;
     D.y = kinect.height;
     
     */
    minArea = 3500; // for 11 inch laptop
    maxArea = 20000; // for 15 inch laptop
    
    
    refRectangles[0] = ofQuaternion(51, 21, 110, 80);
    refRectangles[1] = ofQuaternion(257, 21, 103, 80);
    refRectangles[2] = ofQuaternion(257, 156, 96, 80);
    refRectangles[3] = ofQuaternion(51, 166, 110, 80);
    
    
}

void Sensor::update(){
    
    kinect.update();
	
	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {        
                
		
		// load grayscale depth image from the kinect source
		grayDepthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
		
        //grayDiff.warpPerspective(A, B, C, D); 
        
        //background substraction (ours)
        if (bLearnBakground == true){
			grayBg = grayDepthImage;		// the = sign copys the pixels from grayImage into grayBg (operator overloading)
			bLearnBakground = false;
		}
        
        grayDiff.absDiff(grayBg, grayDepthImage);
        grayDiff.threshold(threshold);
        //
        
        
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
		if(bThreshWithOpenCV) {
			grayThreshNear = grayDiff;
			grayThreshFar = grayDiff;
			grayThreshNear.threshold(nearThreshold, true); //why invert??
			grayThreshFar.threshold(farThreshold);
            
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayDepthImage.getCvImage(), NULL);
		} else {
			
			// or we do it ourselves - show people how they can work with the pixels
			unsigned char * pix = grayDiff.getPixels();
			
			int numPixels = grayDiff.getWidth() * grayDiff.getHeight();
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
                    //if y is greater than the bottom edge of the table (and make that dynamic), set pix[i] to 0
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
		}
		
		// update the cv images
		grayDiff.flagImageChanged();
		
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
		// also, find holes is set to true so we will get interior contours as well....
       
		contourFinder.findContours(grayDiff, minArea, maxArea, 20, false);
        
	}
	
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif
    
}

void Sensor::draw(/*ofBaseApp *parent*/){
    ofBackground(100, 100, 100);	
	ofSetColor(255, 255, 255);
	
	if(bDrawPointCloud) {
		easyCam.begin();
		drawPointCloud();
		easyCam.end();
	} else {
		// draw from the live kinect
		kinect.drawDepth(10, 10, 400, 300);
		kinect.draw(420, 10, 400, 300);
		
		grayDiff.draw(10, 320, 400, 300);
        
        ofSetColor(0,255,0);
        ofNoFill();
        /*
        ofRect(51+10, 21+320, 110, 80);
        ofRect(257+10, 21+320, 103, 80);
        ofRect(257+10, 156+320, 96, 80);
        ofRect(51+10, 166+320, 110, 80);
         */
        ofRect(refRectangles[0].x() +10, refRectangles[0].y() +320, refRectangles[0].z(), refRectangles[0].w());
        ofRect(refRectangles[1].x() +10, refRectangles[1].y() +320, refRectangles[1].z(), refRectangles[1].w());
        ofRect(refRectangles[2].x() +10, refRectangles[2].y() +320, refRectangles[2].z(), refRectangles[2].w());
        ofRect(refRectangles[3].x() +10, refRectangles[3].y() +320, refRectangles[3].z(), refRectangles[3].w());
        
		contourFinder.draw(10, 320, 400, 300);
		
#ifdef USE_TWO_KINECTS
		kinect2.draw(420, 320, 400, 300);
#endif
	}
	
	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
    reportStream << "bg subst. threshold (1,2) is: " << threshold << ". " << endl;
    reportStream << "minArea: " << minArea << "; maxArea: " << maxArea << "." << endl;
	reportStream << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
	<< ofToString(kinect.getMksAccel().y, 2) << " / "
	<< ofToString(kinect.getMksAccel().z, 2) << endl
	<< "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
	<< "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
	<< "set near threshold " << nearThreshold << " (press: + -)" << endl
	<< "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
	<< "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl;
	ofDrawBitmapString(reportStream.str(),20,652);
    
    
}

void Sensor::drawPointCloud() {
	int w = 640;
	int h = 480;
	ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			if(kinect.getDistanceAt(x, y) > 0) {
				mesh.addColor(kinect.getColorAt(x,y));
				mesh.addVertex(kinect.getWorldCoordinateAt(x, y));
			}
		}
	}
	glPointSize(3);
	ofPushMatrix();
	// the projected points are 'upside down' and 'backwards' 
	ofScale(1, -1, -1);
	ofTranslate(0, 0, -1000); // center the points a bit
	glEnable(GL_DEPTH_TEST);
	mesh.drawVertices();
	glDisable(GL_DEPTH_TEST);
	ofPopMatrix();
}

void Sensor::close(){
    kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}

vector<ofxCvBlob> Sensor::getContours(){
    update();
    return contourFinder.blobs;
}

void Sensor::keyPressed(int key){
    switch (key) {     
            /*
        case 'a':
            A.x = mouseX;
            A.y = mouseY;
            break;
        case 's':
            B.x = mouseX;
            B.y = mouseY;
             break;
        case 'd':
            C.x = mouseX;
            C.y = mouseY;
             break;
        case 'e':
            D.x = mouseX;
            D.y = mouseY;
             break;
             */
        case '3':
            minArea -= 100;
            break;
        case '4':
            minArea += 100;
            break;
        case '5':
            maxArea -= 100;
            break;
        case '6':
            maxArea +=100;
            break;
        case '1':
            threshold--;
            if(threshold < 0) threshold = 0;
            break;
        case '2': 
            threshold ++;
            if(threshold > 255) threshold = 255;
            break;
        case 'l':
			bLearnBakground = true;
			break;
		case ' ':
			bThreshWithOpenCV = !bThreshWithOpenCV;
			break;
			
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
            
	}
}

