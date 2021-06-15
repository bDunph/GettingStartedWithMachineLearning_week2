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
    prevVal = 0.0f;
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
    ofDrawBitmapString("Number of training examples: " + ofToString(data.size()), 20, 68);
    ofDrawBitmapString("Model running: " + ofToString(isRunning), 20, 82);
    ofDrawBitmapString("Model Trained: " + ofToString(isTrained), 20, 96);
    ofDrawBitmapString("Recording: " + ofToString(isRecording), 20, 110);
    ofPopStyle();

    ofPushStyle();
    ofSetColor(ofColor::cyan);
    ofRectangle trainingExamplesBar;
    trainingExamplesBar.setPosition(20, 120);
    trainingExamplesBar.setHeight(50);
    trainingExamplesBar.setWidth(data.size());
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
        data.clear();
        lines.clear();
    }

    if (key == 'r' || key == 'R') {
        isRunning = !isRunning;
    }

    if (key == 'q' || key == 'Q') {
        neuralNet.reset();
        isEmpty = true;
        isTrained = false;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
    case 49:
        trainingPos = 1.0f;
        break;
    case 50:
        trainingPos = 2.0f;
        break;
    case 51:
        trainingPos = 3.0f;
        break;
    case 32:
        isRecording = !isRecording;
    }

    if (key == 't' || key == 'T') {
        isTrained = neuralNet.train(data);
        isEmpty = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
    std::vector<double> trainingInput;
    std::vector<double> trainingOutput;

    float mappedX = ofMap(x, 0, ofGetWidth(), 0.0f, 1.0f);
    float mappedY = ofMap(y, 0, ofGetHeight(), 0.0f, 1.0f);

    if (isRecording && !isTrained) {
        trainingInput.push_back(mappedX);
        trainingInput.push_back(mappedY);
        trainingOutput = { (double)slider };
        rapidlib::trainingExample tempExample;
        tempExample.input = trainingInput;
        tempExample.output = trainingOutput;
        data.push_back(tempExample);
        tempLine.addVertex(x, y);
        lines.push_back(tempLine);
    }

    if (isRunning && isTrained) {
        std::vector<double> modelInput;
        modelInput.push_back(mappedX);
        modelInput.push_back(mappedY);
        modelOutput = neuralNet.run(modelInput);
        if (modelOutput[0] < 0.0f) {
            modelOutput[0] = 0.0f;
        }
        else if (modelOutput[0] > 1.0f) {
            modelOutput[0] = 1.0f;
        }

        slider = modelOutput[0];
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
