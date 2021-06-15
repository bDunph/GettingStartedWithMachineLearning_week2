#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

    /* GUI Setup */
    /* Mode frequency ranges base on the resonant frequencies of a wine glass from http://www.csounds.com/manual/html/MiscModalFreq.html */
    gui.setup();
    gui.setPosition(ofGetWidth() * 0.5f, 20);
    gui.add(mode1Freq.setup("Mode 1 Freq", 210.0f, 110.0f, 990.0f));
    gui.add(mode1Res.setup("Mode 1 Res", 8.0f, 4.0f, 9.0f));
    gui.add(mode2Freq.setup("Mode 2 Freq", 487.2f, 255.2f, 2296.8f));
    gui.add(mode2Res.setup("Mode 2 Res", 9.0f, 5.0f, 10.0f));
    gui.add(mode3Freq.setup("Mode 3 Freq", 892.5f, 467.5f, 4207.5f));
    gui.add(mode3Res.setup("Mode 3 Res", 10.0f, 6.0f, 11.0f));
    gui.add(mode4Freq.setup("Mode 4 Freq", 1392.3f, 729.3f, 6563.7f));
    gui.add(mode4Res.setup("Mode 4 Res", 10.0f, 6.0f, 11.0f));
    gui.add(mode5Freq.setup("Mode 5 Freq", 1969.8f, 1031.8f, 9286.2f));
    gui.add(mode5Res.setup("Mode 5 Res", 12.0f, 7.0f, 13.0f));
    gui.add(ex1Freq.setup("ExcSig 1 Freq", 1000.0f, 900.0f, 1100.0f));
    gui.add(ex1Res.setup("ExcSig 1 Res", 9.0f, 7.0f, 10.0f));
    gui.add(ex2Freq.setup("ExcSig 2 Freq", 1800.0f, 1700.0f, 2000.0f));
    gui.add(ex2Res.setup("ExcSig 2 Res", 10.0f, 8.0f, 11.0f));
    gui.add(contactFreq.setup("Contact Freq", 150.0f, 140.0f, 200.0f));
    gui.add(triggerFreq.setup("Trigger Freq", 2.0f, 1.8f, 2.8f));


    /* RapidLib Bools */
    isTrained = false;
    isRunning = false;
    isEmpty = true;

    /* Audio Stuff */
    int sampleRate = 44100;
    int bufferSize = 512;
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);

    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.sampleRate = sampleRate;
    settings.numOutputChannels = 2;
    settings.numInputChannels = 0;
    settings.bufferSize = bufferSize;
    soundStream.setup(settings);
}

//--------------------------------------------------------------
void ofApp::update() {
    rand = ofRandom(5.0f);
    fundFreq = ofRandom(110.0f, 990.0f);

    resModes[0].setCutoff(mode1Freq);
    resModes[0].setResonance(mode1Res);
    resModes[1].setCutoff(mode2Freq);
    resModes[1].setResonance(mode2Res);
    resModes[2].setCutoff(mode3Freq);
    resModes[2].setResonance(mode3Res);
    resModes[3].setCutoff(mode4Freq);
    resModes[3].setResonance(mode4Res);
    resModes[4].setCutoff(mode5Freq);
    resModes[4].setResonance(mode5Res);

    excModes[0].setCutoff(ex1Freq);
    excModes[0].setResonance(ex1Res);
    excModes[1].setCutoff(ex2Freq);
    excModes[1].setResonance(ex2Res);

    env.setAttack(120.0f * rand);
    env.setDecay(500.0f * rand);
    env.setSustain(0.9f * rand);
    env.setRelease(30.0f * rand);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(100, 100, 100);
    gui.draw();

    ofPushStyle();
    ofSetColor(ofColor::beige);
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

    ofDrawBitmapString("Instructions:\n\nPress 'm' to randomise audio parameters\nPick area on screen to enter training data\nPress space to record training examples", 20, 200);
    ofDrawBitmapString("Keyboard Controls:\n\n\tspace -> toggle record data\n\tt -> train model\n\tr -> toggle run model\n\tc -> clear training examples\n\tq -> reset model", 20, 280);
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& output) {

    std::size_t outChannels = output.getNumChannels();

    for (int i = 0; i < output.getNumFrames(); ++i) {

        contactSignal = contactOsc.sinewave(contactFreq);

        trig = phase.phasor(triggerFreq * rand);
        envSig = env.adsr(contactSignal, (int)trig);

        for (int i = 0; i < EXC_MODES; i++) {
            excSignals[i] = excModes[i].play(envSig, 0, 1, 0, 0);
            excSignalTotal += excSignals[i];
        }

        excSignalTotal *= 0.5f;

        if (excSignalTotal < -0.95f) {
            excSignalTotal = 0.0f;
        }

        for (int i = 0; i < RESONANT_MODES; i++) {
            resSignals[i] = resModes[i].play(excSignalTotal, 0, 1, 0, 0);
            resSignalTotal += resSignals[i];
        }

        resSignalTotal *= 0.2f;

        finalOut = (resSignalTotal + excSignalTotal) * 0.5f;

        if (finalOut > 0.95f) {
            finalOut = 0.95f;
        }

        output[i * outChannels] = finalOut * 0.8f;
        output[i * outChannels + 1] = output[i * outChannels];
    }
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
    case 32:
        isRecording = !isRecording;
    }

    if (key == 'm' || key == 'M') {
        mode1Freq = ofRandom(110.0f, 990.0f);
        mode1Res = ofRandom(4.0f, 9.0f);
        mode2Freq= ofRandom(255.2f, 2296.8f);
        mode2Res = ofRandom(5.0f, 10.0f);
        mode3Freq= ofRandom(467.5f, 4207.5f);
        mode3Res = ofRandom(6.0f, 11.0f);
        mode4Freq = ofRandom(729.3f, 6563.7f);
        mode4Res = ofRandom(6.0f, 11.0f);
        mode5Freq = ofRandom(1031.8f, 9286.2f);
        mode5Res = ofRandom(7.0f, 13.0f);
        ex1Freq = ofRandom(900.0f, 1100.0f);
        ex1Res = ofRandom(7.0f, 10.0f);
        ex2Freq = ofRandom(1700.0f, 2000.0f);
        ex2Res = ofRandom(8.0f, 11.0f);
        contactFreq = ofRandom(140.0f, 200.0f);
        triggerFreq = ofRandom(1.8f, 2.8f);
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

    float _mode1Freq = ofMap(mode1Freq, 110.0f, 990.0f, 0.0f, 1.0f);
    float _mode1Res = ofMap(mode1Res, 4.0f, 9.0f, 0.0f, 1.0f);
    float _mode2Freq = ofMap(mode2Freq, 255.2f, 2296.8f, 0.0f, 1.0f);
    float _mode2Res = ofMap(mode2Res, 5.0f, 10.0f, 0.0f, 1.0f);
    float _mode3Freq = ofMap(mode3Freq, 467.5f, 4207.5f, 0.0f, 1.0f);
    float _mode3Res = ofMap(mode3Res, 6.0f, 11.0f, 0.0f, 1.0f);
    float _mode4Freq = ofMap(mode4Freq, 729.3f, 6563.7f, 0.0f, 1.0f);
    float _mode4Res = ofMap(mode4Res, 6.0f, 11.0f, 0.0f, 1.0f);
    float _mode5Freq = ofMap(mode5Freq, 1031.8f, 9286.2f, 0.0f, 1.0f);
    float _mode5Res = ofMap(mode5Res, 7.0f, 13.0f, 0.0f, 1.0f);
    float _ex1Freq = ofMap(ex1Freq, 900.0f, 1100.0f, 0.0f, 1.0f);
    float _ex1Res = ofMap(ex1Res, 7.0f, 10.0f, 0.0f, 1.0f);
    float _ex2Freq = ofMap(ex2Freq, 1700.0f, 2000.0f, 0.0f, 1.0f);
    float _ex2Res = ofMap(ex2Res, 8.0f, 11.0f, 0.0f, 1.0f);
    float _contactFreq = ofMap(contactFreq, 140.0f, 200.0f, 0.0f, 1.0f);
    float _triggerFreq = ofMap(triggerFreq, 1.8f, 2.8f, 0.0f, 1.0f);

    if (isRecording && !isTrained) {
        trainingInput.push_back(mappedX);
        trainingInput.push_back(mappedY);

        trainingOutput = { _mode1Freq, _mode1Res, _mode2Freq, _mode2Res, _mode3Freq, _mode3Res,_mode4Freq, _mode4Res, _mode5Freq, _mode5Res,
                            _ex1Freq, _ex1Res, _ex2Freq, _ex2Res,
                            _contactFreq, _triggerFreq};

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

        for (int i = 0; i < modelOutput.size(); i++) {
            if (modelOutput[i] < 0.0f) {
                modelOutput[i] = 0.0f;
            }
            else if (modelOutput[i] > 1.0f) {
                modelOutput[i] = 1.0f;
            }
        }

        mode1Freq = ofMap(modelOutput[0], 0.0f, 1.0f, 110.0f, 990.0f);
        mode1Res = ofMap(modelOutput[1], 0.0f, 1.0f, 4.0f, 9.0f);
        mode2Freq = ofMap(modelOutput[2], 0.0f, 1.0f, 255.2f, 2296.8f);
        mode2Res = ofMap(modelOutput[3], 0.0f, 1.0f, 5.0f, 10.0f);
        mode3Freq = ofMap(modelOutput[4], 0.0f, 1.0f, 467.5f, 4207.5f);
        mode3Res = ofMap(modelOutput[5], 0.0f, 1.0f, 6.0f, 11.0f);
        mode4Freq = ofMap(modelOutput[6], 0.0f, 1.0f, 729.3f, 6563.7f);
        mode4Res = ofMap(modelOutput[7], 0.0f, 1.0f, 6.0f, 11.0f);
        mode5Freq = ofMap(modelOutput[8], 0.0f, 1.0f, 1031.8f, 9286.2f);
        mode5Res = ofMap(modelOutput[9], 0.0f, 1.0f, 7.0f, 13.0f);
        ex1Freq = ofMap(modelOutput[10], 0.0f, 1.0f, 900.0f, 1100.0f);
        ex1Res = ofMap(modelOutput[11], 0.0f, 1.0f, 7.0f, 10.0f);
        ex2Freq = ofMap(modelOutput[12], 0.0f, 1.0f, 1700.0f, 2000.0f);
        ex2Res = ofMap(modelOutput[13], 0.0f, 1.0f, 8.0f, 11.0f);
        contactFreq = ofMap(modelOutput[14], 0.0f, 1.0f, 140.0f, 200.0f);
        triggerFreq = ofMap(modelOutput[15], 0.0f, 1.0f, 1.8f, 2.8f);
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
