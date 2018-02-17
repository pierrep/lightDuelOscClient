#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	// listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);

	current_msg_string = 0;
	mouseX = 0;
	mouseY = 0;
	mouseButtonState = "";

	ofBackground(30, 30, 130);

    hitSound.load("Samples/hit2.wav");
    missSound.load("Samples/miss2.wav");
    roundWonSound.load("Samples/roundWon.wav");
}

//--------------------------------------------------------------
void ofApp::update(){

    checkOscMessages();
}


//--------------------------------------------------------------
void ofApp::draw(){

	string buf;
	buf = "listening for osc messages on port" + ofToString(PORT);
	ofDrawBitmapString(buf, 10, 20);
    
    if(receivedImage.getWidth() > 0){
        ofDrawBitmapString("Image:", 10, 160);
        receivedImage.draw(10, 180);
    }

	// draw mouse state
	buf = "mouse: " + ofToString(mouseX, 4) +  " " + ofToString(mouseY, 4);
	ofDrawBitmapString(buf, 430, 20);
	ofDrawBitmapString(mouseButtonState, 580, 20);

	for(int i = 0; i < NUM_MSG_STRINGS; i++){
		ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
	}



}

//--------------------------------------------------------------
void ofApp::checkOscMessages()
{
    // hide old messages
    for(int i = 0; i < NUM_MSG_STRINGS; i++){
        if(timers[i] < ofGetElapsedTimef()){
            msg_strings[i] = "";
        }
    }

    /*
    string ofApp::_GetSettingsOSCAdd = "/lightduel/getsettings";
    string ofApp::_SetSettingsOSCAdd = "/lightduel/setsettings";
    string ofApp::_NewGameOSCAdd = "/lightduel/newgame";
    string ofApp::_ResetGameOSCAdd = "/lightduel/resetgame";
    string ofApp::_GameFinishedOSCAdd = "/lightduel/gamefinished";// Sent when game is over so we know we are ready for new users

    // Events and states
    string ofApp::_ButtonPressOSCAdd = "/lightduel/button";		//[int - laneIndex] [int - near/far 0/1]  [int - 0 for miss 1 for hit]
    string ofApp::_RoundWonOSCAdd = "/lightduel/roundwon";		//[int - player index] [int - lane index] [int - rally length]
    string ofApp::_GameWonOSCAdd = "/lightduel/gamewon";		//[int - player index]  [int - total rally length]
    string ofApp::_Lane0PuckOSCAdd = "/lightduel/puck0"; //[float - normalized pos]
    string ofApp::_Lane1PuckOSCAdd = "/lightduel/puck1"; //[float - normalized pos]
    */

    while(receiver.hasWaitingMessages())
    {
        ofxOscMessage m;
        receiver.getNextMessage(m);

        displayMessages(m);

        if(m.getAddress() == "/lightduel/button" )
        {
            int laneindex = m.getArgAsInt32(0);
            int nearFar = m.getArgAsInt32(1);
            int hitMiss = m.getArgAsInt32(2);

            if(hitMiss) hitSound.play();
            else missSound.play();
        }
        else if (m.getAddress() == "/lightduel/roundwon"){
            float pan = m.getArgAsInt32(0);
            roundWonSound.setPan(pan);
            roundWonSound.play();

        }

    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if(key == '1') hitSound.play();
    if(key == '2') missSound.play();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}

void ofApp::displayMessages(ofxOscMessage m)
{
    // unrecognized message: display on the bottom of the screen
    string msg_string;
    msg_string = m.getAddress();
    msg_string += ": ";
    for(int i = 0; i < m.getNumArgs(); i++){
        // get the argument type
        msg_string += m.getArgTypeName(i);
        msg_string += ":";
        // display the argument - make sure we get the right type
        if(m.getArgType(i) == OFXOSC_TYPE_INT32){
            msg_string += ofToString(m.getArgAsInt32(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
            msg_string += ofToString(m.getArgAsFloat(i));
        }
        else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
            msg_string += m.getArgAsString(i);
        }
        else{
            msg_string += "unknown";
        }
    }
    // add to the list of strings to display
    msg_strings[current_msg_string] = msg_string;
    timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
    current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
    // clear the next line
    msg_strings[current_msg_string] = "";
}
