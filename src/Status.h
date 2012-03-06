//
//  Status.h
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef ofxKinectExample_Status_h
#define ofxKinectExample_Status_h

using namespace std;


enum Tag { BUSY, AVAILABLE, STUCK, FEEDBACK, NO_STATUS };


class Status{
public:
    Status();
    Status(string _currentMessage, Tag _currentStatus, string _timeStamp, int _seat);
    string currentMessage;
    Tag currentStatus;
    string timeStamp; //should be dateTime format
    int seat;
    
};



#endif
