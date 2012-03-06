#include "testApp.h"
using namespace std;

//--------------------------------------------------------------
void testApp::setup() {
	//create 4 seats (with their numbers)
    seats[0] = Seat(0); //add params: initialX, initialY
    seats[1] = Seat(1);
    seats[2] = Seat(2);
    seats[3] = Seat(3);
    
    loadImages();
    
    lastWebUpdate = time(0);
    webUpdateInterval = 1;   
    
    mode = TEST_GRAPHICS; //{FULL, TEST_GRAPHICS, TEST_TRACKING, TEST_WEB};
}

void testApp::loadImages(){
    //HACK: where should this enum live?  Tag { BUSY, AVAILABLE, STUCK, FEEDBACK };
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
    
    hiveLogo = ofImage("images/hiveLogo.png");
    
    /*
    verdana14.loadFont("verdana.ttf", 14, true, true);
	verdana14.setLineHeight(18.0f);
	verdana14.setLetterSpacing(1.037);
    
	verdana30.loadFont("verdana.ttf", 30, true, true);
	verdana30.setLineHeight(34.0f);
	verdana30.setLetterSpacing(1.035);
    */
    /*
    
    ofxTextBlock text1, text2, text3, text4;
    text1.init("verdana.ttf", 14);
    text2.init("verdana.ttf", 14);
    text3.init("verdana.ttf", 14);
    text4.init("verdana.ttf", 14);
    
    textBoxes.push_back(text1);
    textBoxes.push_back(text2);
    textBoxes.push_back(text3);
    textBoxes.push_back(text4);
    
    for(int i = 0; i < textBoxes.size(); i++){
        textBoxes[i].wrapTextX(160);
    }*/
    
    
   
}

//--------------------------------------------------------------
void testApp::update() {
    
    if(mode == FULL){
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
        
        //read user messages
        if(time(0) - lastWebUpdate > webUpdateInterval){        
            statusReader.requestNewStatuses(/*lastTime*/);
            lastWebUpdate = time(0);
        }
        
        for(int i = 0; i < 4; i++){ //seats # hardcoded (there's no array.length. this should eventually be a vector)
            if(seats[i].taken){
                Status newStatus = statusReader.getLatestStatusFor(seats[i].number);
                //if new status is not null
                seats[i].status = newStatus;
            }
        }        
    }
    else if(mode == TEST_GRAPHICS){
        //load fake data.
        seats[0].number = 1;
        seats[0].taken = true;
        seats[0].status = Status("need to work on the pocode project due tomorrow", BUSY, "10:30", 1);
        seats[0].x = 210;  
        seats[0].y = 82; 
        
        seats[1].number = 2;
        seats[1].taken = true;
        seats[1].status = Status("available message", AVAILABLE, "10:30", 2);
        seats[1].x = 685; 
        seats[1].y = 82; 
        
        seats[2].number = 3;
        seats[2].taken = true;
        seats[2].status = Status("stuck message", STUCK, "10:30", 3);
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
        //sensor.draw(this); 
    }
       
    for(int i = 0; i < 4; i++){ //seats # hardcoded 
        draw(seats[i]);        
    }    
    
}

void testApp::draw(Seat seat){  
    int status = seat.status.currentStatus;
    
    ofSetColor(255, 255, 255);
    
    halos[status].draw(seat.x, seat.y);
    if(seat.taken){
        Tag status = seat.status.currentStatus; 
        if(seat.status.currentStatus != NO_STATUS){
            logos[status].draw(seat.x - 62, seat.y - 30);              
            smallLogos[status].draw(seat.x + 192, seat.y + 121); 
        }     
        //verdana14.drawString(seat.status.currentMessage, seat.x - 62, seat.y + 11);
        /*
        textBoxes[status].setText(seat.status.currentMessage);
        textBoxes[status].draw(seat.x - 62, seat.y + 11);
         */
               
    }  
    else{
        hiveLogo.draw(seat.x + 35, seat.y + 105);
        //verdana14.drawString("Login URL", seat.x - 62, seat.y + 11); 
    }
    
    /*
     //Font Usage:
     verdana14.drawString("anti aliased", 145, 92);
     verdana14.drawString("anti aliased", 145, 195);
     verdana14A.drawString("aliased", 525, 92);
     
     ofSetColor(225);
     verdana14.drawString("verdana 30pt - ", 30, 195);
     verdana30.drawString(typeStr, 30, 229);
     */
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
