//
//  Header.h
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#ifndef ofxKinectExample_Header_h
#define ofxKinectExample_Header_h


enum Tag { BUSY, AVAILABLE, STUCK, FEEDBACK, NO_STATUS };
using namespace std;



class Seat{
public:
    Seat();
    Seat(int _number);
    
    int number;
    bool taken;
    int x, y;    
    
    string currentMessage;
    Tag currentStatus;
    int timeStamp; //should be dateTime format

    
};



#endif
