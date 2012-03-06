//
//  StatusReader.h
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ofxKinectExample_StatusReader_h
#define ofxKinectExample_StatusReader_h


#include "ofxHttpUtils.h"
#include "Status.h"
using namespace std;



class StatusReader{
public:
    StatusReader();
    void requestNewStatuses();
    void newResponse(ofxHttpResponse & response);
    void draw();
    
    
    ofxHttpUtils httpUtils;
    int counter;
    string responseStr;
    string requestStr;
    string action_url;
    
    vector<Status> statuses;//historical values?
    Status getLatestStatusFor(int seatNumber);
        
    
    
};
#endif
