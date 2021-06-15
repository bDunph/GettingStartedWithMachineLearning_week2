#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

    /* GUI Setup */
    gui.setup();
    gui.setPosition(ofGetWidth() * 0.5f, 20);
    gui.add(slider.setup("Model Output", 0.5f, 0.0f, 1.0f));

    /* RapidLib Bools */
    isTrained = false;
    isRunning = false;
    isEmpty = true;
}

//--------------------------------------------------------------
void ofApp::update() {
    ofBackground(100, 100, 100);
}

//--------------------------------------------------------------
void ofApp::draw() {
    gui.draw();

    ofPushStyle();
    ofSetColor(ofColor::beige);
    ofDrawBitmapString("Current output: " + ofToString((float)slider), 20, 40);
    ofDrawBitmapString("Number of training examples: " + ofToString(), 20, 68); // calculate the number of examples using the data vector size
    ofDrawBitmapString("Model running: " + ofToString(isRunning), 20, 82);
    ofDrawBitmapString("Model Trained: " + ofToString(), 20, 96);
    ofDrawBitmapString("Recording: " + ofToString(isRecording), 20, 110);
    ofPopStyle();

    ofPushStyle();
    ofSetColor(ofColor::cyan);
    ofRectangle trainingExamplesBar;
    trainingExamplesBar.setPosition(20, 120);
    trainingExamplesBar.setHeight(50);
    trainingExamplesBar.setWidth(); // set the width of the bar to the size of the data vector
    ofDrawRectangle(trainingExamplesBar);
    for (int i = 0; i < lines.size(); i++) {
        lines[i].draw();
    }
    ofPopStyle();

    ofDrawBitmapString("Instructions:\n\nPosition the Model Output slider to desired output\nPick area on screen to enter training data\nPress space to record training examples", 20, 200);
    ofDrawBitmapString("Keyboard Controls:\n\n\tspace -> toggle record data\n\tt -> train model\n\tr -> toggle run model\n\tc -> clear training examples\n\tq -> reset model", 20, 280);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'c' || key == 'C') {
        /* use the 'c' key to clear the data vector */
        lines.clear();
    }

    if (key == 'r' || key == 'R') {
        isRunning = !isRunning;
    }

    if (key == 'q' || key == 'Q') {
        /* use the 'q' key to reset the neural network */
        isEmpty = true;
        isTrained = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
    case 32:
        isRecording = !isRecording;
        break;
    }

    if (key == 't' || key == 'T') {
        /* train the neural network using the 't' key
        * send the data vector as input
        * use the isTrained bool to report if the network trained or not */
        isEmpty = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    /* set up two double vectors to hold the training input and output */

    float mappedX = ofMap(x, 0, ofGetWidth(), 0.0f, 1.0f);
    float mappedY = ofMap(y, 0, ofGetHeight(), 0.0f, 1.0f);

    if (isRecording && !isTrained) {
        /* use the mapped x and y values to populate the input vector */
        /* use the slider values to populate the output vector */
        /* create a temporary example of type rapidlib::traininExample
        * set the input to the trainingInput vector
        * set the output to the trainingOutput vector
        * push back the temporary training example on to the data vector */
        tempLine.addVertex(x, y);
        lines.push_back(tempLine);
    }

    if (isRunning && isTrained) {
        /* create a vector to carry new data to the network
        * use the mapped x and y values and input
        * run the neural network on the new data vector */
        if (modelOutput[0] < 0.0f) {
            modelOutput[0] = 0.0f;
        }
        else if (modelOutput[0] > 1.0f) {
            modelOutput[0] = 1.0f;
        }

        /* send the output of the model to the slider object */
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
    tempLine.clear();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
