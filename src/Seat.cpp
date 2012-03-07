//
//  File.cpp
//  ofxKinectExample
//
//  Created by luisaph on 2/23/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Seat.h"

Seat::Seat(){
    number = 0;
    x = 0;
    y = 0;
    
    currentMessage = "nothing yet";    
    currentStatus = NO_STATUS;
    timeStamp = ""; 
}

Seat::Seat(int _number){    
    number = _number;
    x = 0;
    y = 0;
    
    currentMessage = "nothing yet";    
    currentStatus = NO_STATUS;
    timeStamp = ""; 
}




