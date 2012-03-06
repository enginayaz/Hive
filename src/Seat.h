//
//  Header.h
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//


#ifndef ofxKinectExample_Header_h
#define ofxKinectExample_Header_h

#include "Status.h"
using namespace std;



class Seat{
public:
    Seat();
    Seat(int _number);
    
    int number;
    bool taken;
    //dateTime lastUpdated;
    Status status;
    int x, y;    
    
};



#endif
