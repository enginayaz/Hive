//
//  Status.cpp
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Status.h"

Status::Status(){
    currentMessage = "";    
    currentStatus = NO_STATUS;//what is the default status?
    timeStamp = ""; 
    seat = 0;
}



Status::Status(string _currentMessage, Tag _currentStatus, string _timeStamp, int _seat){
    currentMessage = _currentMessage;    
    currentStatus = _currentStatus;
    timeStamp = _timeStamp; 
    seat = _seat;
}
