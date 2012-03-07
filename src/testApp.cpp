#include "testApp.h"
using namespace std;

//--------------------------------------------------------------
void testApp::setup() {
    mode = FULL; //{FULL, TEST_GRAPHICS, TEST_TRACKING, TEST_WEB};
    
    ofBackground(0,0,0);    
    	
    
    //setup initial seats (should be a vector maybe, to make it easier to extend.)
    numberOfSeats = 4;  
    
    seats[0] = Seat(0); 
    seats[0].x = 210;  // should be 451
    seats[0].y = 82; // should be 253
    
    seats[1] = Seat(1);
    seats[1].x = 685; // should be 926
    seats[1].y = 82;  // should be 253
    
    seats[2] = Seat(2);
    seats[2].x = 685; 
    seats[2].y = 459; // try 439 
    
    seats[3] = Seat(3);
    seats[3].x = 210;  
    seats[3].y = 459;  // try 439
    
    
    seatURLs.push_back("");
    seatURLs.push_back("");
    seatURLs.push_back("");
    seatURLs.push_back("");
    
    
    seatURLs[0] = "            goo.gl/zrJVK";
    seatURLs[1] = "  goo.gl/gJRVi";
    seatURLs[2] = "  goo.gl/B70xT";
    seatURLs[3] = "  goo.gl/pBYza";
     
    
    setupImagesAndFonts();      
    
    
    action_url = "http://itp.nyu.edu/~ea1095/sinatra/hive_v1/latestEntry/allSeats";	
    ofAddListener(httpUtils.newResponseEvent,this,&testApp::newResponse);
	httpUtils.start();
    lastWebUpdate = time(0);
    webUpdateInterval = 1; // check whether 0.5 works   
    
    firstRectLeft = sensor.refRectangles[0].x();
    firstRectRight = sensor.refRectangles[0].x() + sensor.refRectangles[0].z();
    firstRectTop = sensor.refRectangles[0].y();
    firstRectBottom = sensor.refRectangles[0].y() + sensor.refRectangles[0].w();
    
    secondRectLeft = sensor.refRectangles[1].x();
    secondRectRight = sensor.refRectangles[1].x() + sensor.refRectangles[1].z();
    secondRectTop = sensor.refRectangles[1].y();
    secondRectBottom = sensor.refRectangles[1].y() + sensor.refRectangles[1].w();
    
    thirdRectLeft = sensor.refRectangles[2].x();
    thirdRectRight = sensor.refRectangles[2].x() + sensor.refRectangles[2].z();
    thirdRectTop = sensor.refRectangles[2].y();
    thirdRectBottom = sensor.refRectangles[2].y() + sensor.refRectangles[2].w();
    
    fourthRectLeft = sensor.refRectangles[3].x();
    fourthRectRight = sensor.refRectangles[3].x() + sensor.refRectangles[3].z();
    fourthRectTop = sensor.refRectangles[3].y();
    fourthRectBottom = sensor.refRectangles[3].y() + sensor.refRectangles[3].w();
    
}



//--------------------------------------------------------------
void testApp::update() {
    
    if(mode == TEST_TRACKING || mode == FULL){
        
        int tolerance = 10; //set distance tolerance
        
        //set all seats as not taken (necessary?)
        for(int i = 0; i < numberOfSeats; i++){ //seats # hardcoded!
            seats[i].taken = false;
        }
        
        //determine which seats are taken and which are free
        vector<ofxCvBlob> blobs = sensor.getContours();
        
        for(int i = 0; i < blobs.size(); i++){
            ofxCvBlob blob = blobs[i];
            
            ofQuaternion secondRect = sensor.refRectangles[1];
            ofQuaternion thirdRect = sensor.refRectangles[2];
            ofQuaternion fourthRect = sensor.refRectangles[3];
            
            float scale = 1.6f; //scaleFrom640x480to400x300
            //float scaledBlobX = blob.boundingRect.x / scale;
            //float scaledBlobY = blob.boundingRect.y / scale;
            float scaledBlobX = blob.boundingRect.getCenter().x / scale;
            float scaledBlobY = blob.boundingRect.getCenter().y / scale;
            
            /*
            cout << "scaledBlobX: " << scaledBlobX << endl;
            cout << "scaledBlobY: " << scaledBlobY << endl;
            cout << "thirdRectLeft: " << thirdRectLeft << endl;
            cout << "thirdRectRight: " << thirdRectRight << endl;
            cout << "thirdRectBottom: " << thirdRectBottom << endl;
            cout << "thirdRectTop: " << thirdRectTop << endl;*/
            
            if(firstRectLeft < scaledBlobX  && scaledBlobX < firstRectRight 
               && firstRectTop < scaledBlobY && scaledBlobY < firstRectBottom){
                
                seats[0].taken = true;
                //cout << "seats[0].taken: " << seats[0].taken << endl; 
            }
            if(secondRectLeft < scaledBlobX  && scaledBlobX < secondRectRight
               && secondRectTop < scaledBlobY && scaledBlobY < secondRectBottom){
                
                seats[1].taken = true;
                //cout << "seats[1].taken: " << seats[1].taken << endl;
            }
            if(thirdRectLeft < scaledBlobX  && scaledBlobX < thirdRectRight 
               && thirdRectTop < scaledBlobY && scaledBlobY < thirdRectBottom){
                
                seats[2].taken = true;
                //cout << "seats[2].taken: " << seats[2].taken << endl;
            }
            if(fourthRectLeft < scaledBlobX  && scaledBlobX < fourthRectRight 
               && fourthRectTop < scaledBlobY && scaledBlobY < fourthRectBottom){
                
                seats[3].taken = true;
                //cout << "seats[3].taken: " << seats[3].taken << endl;
            }
            /*
            cout << "seats[0].taken: " << seats[0].taken << endl; 
            cout << "seats[1].taken: " << seats[1].taken << endl; 
            cout << "seats[2].taken: " << seats[2].taken << endl; 
            cout << "seats[3].taken: " << seats[3].taken << endl; */
            //cout << "seats[2].taken: " << seats[2].taken << endl;
            
        }
        
        
        
        
        /*
        for(int i = 0; i < blobs.size(); i++){
            ofxCvBlob blob = blobs[i];
            int squaredDistance = pow(abs(blob.boundingRect.x - seats[0].x), 2) + 
            pow(abs(blob.boundingRect.y - seats[0].y), 2);        
            if(squaredDistance < tolerance){
                seats[i].x = blob.boundingRect.x;
                seats[i].y = blob.boundingRect.y;
                seats[i].taken = true;
            }        
        }   
         */
         
    }
    if(mode == TEST_WEB || mode == FULL){
        //read user messages
        
        if(time(0) - lastWebUpdate > webUpdateInterval){        
            requestNewStatuses();
            lastWebUpdate = time(0);
        }    
    }               
    
    else if(mode == TEST_GRAPHICS){
        //load fake data.
        seats[0].number = 0;
        seats[0].taken = true;
        seats[0].currentMessage = "need to work on the pocode project";
        seats[0].currentStatus = BUSY;
        seats[0].timeStamp = 0;
        seats[0].x = 210;  
        seats[0].y = 82; 
        
        seats[1].number = 1;
        seats[1].taken = true;
        seats[1].currentMessage = "available message";
        seats[1].currentStatus = AVAILABLE;
        seats[1].timeStamp = 0;
        seats[1].x = 685; 
        seats[1].y = 82; 
        
        seats[2].number = 2;
        seats[2].taken = true;
        seats[2].currentMessage = "stuck message";
        seats[2].currentStatus = STUCK;
        seats[2].timeStamp = 0  ;
        seats[2].x = 210; 
        seats[2].y = 459; 
        
        seats[3].number = 3;
        seats[3].taken = false;
        seats[3].x = 685;  
        seats[3].y = 459;        
        
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw() {  
    ofBackground(0,0,0);
    if(mode == TEST_TRACKING){
        sensor.draw(/*this*/); 
    }
    else{
        for(int i = 0; i < numberOfSeats; i++){ 
            draw(seats[i]);        
        }
    }
        
    
}

void testApp::draw(Seat seat){  
    int status = seat.currentStatus;    
    
    ofSetColor(255, 255, 255);
    
    ofPushMatrix();
     if(seat.number == 0 || seat.number == 1)
     {         
         ofTranslate(seat.x + 245, seat.y + 171); //halo's width and height
         ofRotate(180);
        
     }   
    if(seat.number == 2 || seat.number == 3){
        ofTranslate(seat.x, seat.y);
    }

    
    halos[status].resize(245, 171);
    halos[status].draw(0, 0);
    //halos[status].draw(seat.x, seat.y);
    bool statusUpdatesInTheLast5Minutes = (time(0) - seat.timeStamp) < 300;
    
    
    if(seat.taken){
        if(statusUpdatesInTheLast5Minutes){
            int status = (int)seat.currentStatus; 
            
            //cout << "seat: " << seat.number << ". status: " << status << endl; 
            
            if(seat.currentStatus != NO_STATUS){
                logos[status].resize(50, 33);
                logos[status].draw( -60, - 30);  //w:50, h:33
                smallLogos[status].resize(24, 22);
                smallLogos[status].draw(192, 122);//w:24, h:22           
                
            }           
            
            statusTextBoxes[seat.number].setText(seat.currentMessage); 
            statusTextBoxes[seat.number].wrapTextX(maxTextWidth);
            
            ofPushMatrix();
            ofTranslate(- 10, 13);
            ofRotate(90); 
            ofSetColor(255, 255, 255);
            statusTextBoxes[seat.number].draw(0, 0);
            ofPopMatrix();
        }
        else{
            halos[5].resize(245, 171); //5 is the yellow halo, for "welcome"
            halos[5].draw(0, 0);
            hiveLogo.resize(55, 49);        
            hiveLogo.draw( 94,  37); //55 x 49        
            welcomeText.setText(seatURLs[seat.number]);
            welcomeText.wrapTextX(maxTextWidth);//176
            welcomeText.draw( 35,  105); 
            
        }
                 
               
    }  
    
    else{
        halos[5].resize(245, 171); //5 is the yellow halo, for "welcome"
        halos[5].draw(0, 0);        
        hiveLogo.resize(55, 49);        
        hiveLogo.draw(94, 37); //55 x 49        
              
    }
    
    
     
     ofPopMatrix();
     
     
    
}


//--------------------------------------------------------------
void testApp::exit() {
    sensor.close();
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
    switch (key) { 
        case 'm':
            
            if(mode == FULL){
                mode = TEST_TRACKING;
            }
            else if(mode == TEST_TRACKING){
                mode = FULL;
            }  
            cout << "mode: " << mode << endl;
            break;
    }
    
    sensor.keyPressed(key); 
    
}

void testApp::setupImagesAndFonts(){
    
    //images
    halos.push_back(ofImage("images/haloBusy.png"));
    halos.push_back(ofImage("images/haloAvailable.png"));
    halos.push_back(ofImage("images/haloStuck.png"));
    halos.push_back(ofImage("images/haloFeedback.png"));
    halos.push_back(ofImage("images/haloNoStatus.png"));
    halos.push_back(ofImage("images/haloHive.png"));    
    
    
    logos.push_back(ofImage("images/logoBusy.png"));
    logos.push_back(ofImage("images/logoAvailable.png"));
    logos.push_back(ofImage("images/logoStuck.png"));
    logos.push_back(ofImage("images/logoFeedback.png"));   
    logos.push_back(ofImage("images/logoNoStatus.png")); 
    
    
    smallLogos.push_back(ofImage("images/smallLogoBusy.png"));
    smallLogos.push_back(ofImage("images/smallLogoAvailable.png"));
    smallLogos.push_back(ofImage("images/smallLogoStuck.png"));
    smallLogos.push_back(ofImage("images/smallLogoFeedback.png"));    
    smallLogos.push_back(ofImage("images/smallLogoNoStatus.png"));
    
    for(int i = 0; i < halos.size(); i++){
        halos[i].setImageType(OF_IMAGE_COLOR_ALPHA); 
    }
    for(int i = 0; i < logos.size(); i++){
        logos[i].setImageType(OF_IMAGE_COLOR_ALPHA); 
        logos[i].rotate90(1);
    }
    for(int i = 0; i < smallLogos.size(); i++){
        smallLogos[i].setImageType(OF_IMAGE_COLOR_ALPHA); 
    }
    
    hiveLogo = ofImage("images/logoHive.png");
    hiveLogo.setImageType(OF_IMAGE_COLOR_ALPHA);  
    
    //fonts    
    maxTextWidth = 167;
    alignment = OF_TEXT_ALIGN_LEFT;
    
    for(int i = 0; i < numberOfSeats; i++){
        ofxTextBlock textBlock;
        textBlock.init("verdana.ttf", 14);
        statusTextBoxes.push_back(textBlock);
        
    }
    
    welcomeText.init("verdana.ttf", 14);
    
}

void testApp::requestNewStatuses(){
    ofxHttpForm form;
    form.action = action_url;
    form.method = OFX_HTTP_GET;
    httpUtils.addForm(form);
    requestStr = "message sent: " + ofToString(counter);
    counter++;
}

void testApp::newResponse(ofxHttpResponse & response){
	responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
    //http://stackoverflow.com/questions/236129/how-to-split-a-string-in-c
    
    //cout << (string)response.responseBody;
    
    vector<string> lines;
    string data = (string)response.responseBody;
    stringstream ss(data);
    string line;
    
    int counter = 0;
    
    int itemCounter = 0;
    while(getline(ss, line, ';') && itemCounter < numberOfSeats) {
        
        //lines.push_back(item);
        cout << "line " << counter << endl;
        counter++;
        
        vector<string> items;
        stringstream lineStream(line);
        string item;
        itemCounter++;
        
        
        while(getline(lineStream, item, '$')){
            items.push_back(item);
            /*cout << "item[" << itemCounter << "]" << items[itemCounter] << endl;
             */
            
        }  
        //cout << "item[1]" << items[0] << endl;
        
        
        /* 
         cout << "item[0]" << items[0] << endl;
         cout << "item[1]" << items[1] << endl;
         cout << "item[2]" << items[2] << endl;
         cout << "item[3]" << items[3] << endl;
         cout << "item[4]" << items[4] << endl <<endl;*/
        
        
        
        int seatNumber = atoi(items[1].c_str()) - 1; //equivalent to cast from string to int.            
        
        //seats[seatNumber].timeStamp = items[0];  
        seats[seatNumber].timeStamp = atoi(items[0].c_str());  
        
        
        seats[seatNumber].currentMessage = items[3];        
        
        if(items[4] == "busy"){
            seats[seatNumber].currentStatus = BUSY;
        }
        if(items[4] == "available"){
            seats[seatNumber].currentStatus = AVAILABLE;
        }
        if(items[4] == "feedback"){
            seats[seatNumber].currentStatus = FEEDBACK;
        }
        if(items[4] == "stuck"){
            seats[seatNumber].currentStatus = STUCK;
        } /*
        if(items[4] == ""){
            seats[seatNumber].currentStatus = NO_STATUS;
        } */
        
        
        
    }    
    
}


//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}
