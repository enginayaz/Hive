#include "testApp.h"
using namespace std;

//--------------------------------------------------------------
void testApp::setup() {
    mode = TEST_WEB; //{FULL, TEST_GRAPHICS, TEST_TRACKING, TEST_WEB};
    
    ofBackground(0,0,0);    
    	
    
    //setup initial seats (should be a vector maybe, to make it easier to extend.)
    numberOfSeats = 4;
    /*
    seat1 = Seat(0); 
    seat1.x = 210;  
    seat1.y = 82; 
    
    seat2 = Seat(1);
    seat2.x = 685; 
    seat2.y = 82; 
    
    seat3 = Seat(2);
    seat3.x = 210; 
    seat3.y = 459;
    
    seat4 = Seat(3);
    seat4.x = 685;  
    seat4.y = 459;  
    
    seats.push_back(seat1);
    seats.push_back(seat2);
    seats.push_back(seat3);
    seats.push_back(seat4);
     */
    
    seats[0] = Seat(0); 
    seats[0].x = 210;  
    seats[0].y = 82; 
    
    seats[1] = Seat(1);
    seats[1].x = 685; 
    seats[1].y = 82; 
    
    seats[2] = Seat(2);
    seats[2].x = 210; 
    seats[2].y = 459;
    
    seats[3] = Seat(3);
    seats[3].x = 685;  
    seats[3].y = 459;  
    
    
    setupImagesAndFonts();      
    
    
    action_url = "http://itp.nyu.edu/~ea1095/sinatra/hive_v1/latestEntry/allSeats";	
    ofAddListener(httpUtils.newResponseEvent,this,&testApp::newResponse);
	httpUtils.start();
    lastWebUpdate = time(0);
    webUpdateInterval = 1;    
    
    
}



//--------------------------------------------------------------
void testApp::update() {
    
    if(mode == TEST_TRACKING || mode == FULL){
        int tolerance = 10; //set distance tolerance
        
        //set all seats as not taken (necessary?)
        for(int i = 0; i < 4; i++){ //seats # hardcoded!
            seats[i].taken = false;
        }
        
        //determine which seats are taken and which are free
        vector<ofxCvBlob> blobs = sensor.getContours();
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
        seats[0].number = 1;
        seats[0].taken = true;
        seats[0].currentMessage = "need to work on the pocode project due tomorrow";
        seats[0].currentStatus = BUSY;
        seats[0].timeStamp = "10:30";
        seats[0].x = 210;  
        seats[0].y = 82; 
        
        seats[1].number = 2;
        seats[1].taken = true;
        seats[1].currentMessage = "available message";
        seats[1].currentStatus = AVAILABLE;
        seats[1].timeStamp = "10:30";
        seats[1].x = 685; 
        seats[1].y = 82; 
        
        seats[2].number = 3;
        seats[2].taken = true;
        seats[2].currentMessage = "stuck message";
        seats[2].currentStatus = STUCK;
        seats[2].timeStamp = "10:30";
        seats[2].x = 210; 
        seats[2].y = 459; 
        
        seats[3].number = 1;
        seats[3].taken = false;
        seats[3].x = 685;  
        seats[3].y = 459;        
        
        
    }
    
    
}

//--------------------------------------------------------------
void testApp::draw() {  
    if(mode == TEST_TRACKING){
        sensor.draw(this); 
    }
       
    for(int i = 0; i < numberOfSeats; i++){ 
        draw(seats[i]);        
    }    
    
}

void testApp::draw(Seat seat){  
    int status = seat.currentStatus;
    if(seat.number == 3){
        cout << "seat " << seat.number << "status at DRAW: " << seat.currentStatus << endl;
    }
    
    
    ofSetColor(255, 255, 255);
    
    halos[status].resize(245, 171);
    halos[status].draw(seat.x, seat.y);
    if(seat.taken){
        int status = (int)seat.currentStatus; 
        
        //cout << "seat: " << seat.number << ". status: " << status << endl; 
        
        if(seat.currentStatus != NO_STATUS){
            logos[status].resize(50, 33);
            logos[status].draw(seat.x - 60, seat.y - 30);  //w:50, h:33
            smallLogos[status].resize(24, 22);
            smallLogos[status].draw(seat.x + 192, seat.y + 122);//w:24, h:22
            
            
        }   
        /*
        statusTextBoxes[status].setText(seat.currentMessage); 
        statusTextBoxes[status].wrapTextX(maxTextWidth);
        welcomeText.wrapTextX(176); //height: 39.
        ofPushMatrix();
            ofTranslate(seat.x - 10, seat.y + 3);
            ofRotate(90); 
            statusTextBoxes[status].draw(0, 0);
        ofPopMatrix();*/
         
        
         
               
    }  
    else{
        hiveLogo.resize(55, 49);        
        hiveLogo.draw(seat.x + 94, seat.y + 37); //55 x 49        
        welcomeText.setText("http://itp.nyu.edu/~ea1095/sinatra/hive_v1/seat/" + seat.number );
        welcomeText.wrapTextX(maxTextWidth);
        welcomeText.draw(seat.x + 35, seat.y + 105);         
    }
    
}


//--------------------------------------------------------------
void testApp::exit() {
    sensor.close();
	
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
    sensor.keyPressed(key); 
    /*
     case 'd':
     vector<Status> statuses = statusReader.statuses;
     for(int i = 0; i < statuses.size(); i++){
     cout << "i: " << i << "(message: " << statuses[i].currentMessage << "; status: " << statuses[i].currentStatus << "; timeStamp: " << statuses[i].timeStamp << "; seat: " << statuses[i].seat << endl;
     }
     break;
     */
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
        
        seats[seatNumber].timeStamp = items[0];        
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
        
        cout << "Seat: " << seatNumber << endl;        
        cout << "timeStamp: " << seats[seatNumber].timeStamp << endl;
        cout << "message: " << seats[seatNumber].currentMessage << endl;
        cout << "status: " << seats[seatNumber].currentStatus << endl << endl;
        
        
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
