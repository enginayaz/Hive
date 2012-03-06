//
//  StatusReader.cpp
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "StatusReader.h"
using namespace std;

StatusReader::StatusReader(){
    action_url = "http://itp.nyu.edu/~ea1095/sinatra/hive_v1/example_page";
	ofAddListener(httpUtils.newResponseEvent,this,&StatusReader::newResponse);
	httpUtils.start();
}

void StatusReader::requestNewStatuses(){
    ofxHttpForm form;
    form.action = action_url;
    form.method = OFX_HTTP_GET;
    httpUtils.addForm(form);
    requestStr = "message sent: " + ofToString(counter);
    counter++;
}

void StatusReader::newResponse(ofxHttpResponse & response){
	responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
    //http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
    
    cout << (string)response.responseBody;
    
    vector<string> lines;
    string data = (string)response.responseBody;
    stringstream ss(data);
    string line;
    
    while(getline(ss, line, ';')) {
        //lines.push_back(item);
        Status status;
        
        vector<string> items;
        stringstream lineStream(line);
        string item;
        
        while(getline(lineStream, item, ',')){
            items.push_back(item);
        }
        
        status.currentMessage = items[0];
        
        
        if(items[1] == "busy"){
            status.currentStatus = BUSY;
        }
        if(items[1] == "available"){
            status.currentStatus = AVAILABLE;
        }
        if(items[1] == "feedback"){
            status.currentStatus = FEEDBACK;
        }
        if(items[1] == "stuck"){
            status.currentStatus = STUCK;
        }
        
        status.timeStamp = items[2];
        status.seat = atoi(items[3].c_str()); //equivalent to cast from string to int.
        
        statuses.push_back(status);
    }
    
    
}

Status StatusReader::getLatestStatusFor(int seatNumber/*, dateTime lastUpdated*/){ 
    //implement:
    //iterate through statuses (use a dictionary instead of a vector? keep them ordered by date?). 
    //if latest timeStamp for seatNumber > lastUpdated
        //return status
    //else
        //return null;
}

void StatusReader::draw(){
    ofSetColor(255,0,0);
    //ofDrawBitmapString(requestStr,20,60);
    ofDrawBitmapString(responseStr,20,60);
}


