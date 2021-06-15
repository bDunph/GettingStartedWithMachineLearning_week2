#pragma once

#include "ofMain.h"
#include "ofxRapidLib.h"
#include "ofxGui.h"
#include "ofxMaxim.h"

#define RESONANT_MODES 5
#define EXC_MODES 2

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	/* RapidLib */
	std::vector<rapidlib::trainingExample> data;
	std::vector<double> modelOutput;
	rapidlib::regression neuralNet;
	bool isRecording;
	bool isTrained;
	bool isEmpty;
	bool isRunning;
	int trainingPos;
	float prevVal;

	/* GUI */
	ofxPanel gui;
	ofxFloatSlider mode1Freq;
	ofxFloatSlider mode1Res;
	ofxFloatSlider mode2Freq;
	ofxFloatSlider mode2Res;
	ofxFloatSlider mode3Freq;
	ofxFloatSlider mode3Res;
	ofxFloatSlider mode4Freq;
	ofxFloatSlider mode4Res;
	ofxFloatSlider mode5Freq;
	ofxFloatSlider mode5Res;
	ofxFloatSlider ex1Freq;
	ofxFloatSlider ex1Res;
	ofxFloatSlider ex2Freq;
	ofxFloatSlider ex2Res;
	ofxFloatSlider attack;
	ofxFloatSlider decay;
	ofxFloatSlider sustain;
	ofxFloatSlider release;
	ofxFloatSlider contactFreq;
	ofxFloatSlider triggerFreq;
	std::vector<ofPolyline> lines;
	ofPolyline tempLine;

	/* Audio Stuff */
	ofSoundStream soundStream;

	void audioOut(ofSoundBuffer& output) override;
	maxiSVF resModes[RESONANT_MODES];
	double resSignals[RESONANT_MODES];
	double resSignalTotal;
	maxiSVF excModes[EXC_MODES];
	double excSignals[EXC_MODES];
	double excSignalTotal;
	maxiOsc pulse;
	double amp;
	convert convert;
	maxiEnv env;
	maxiOsc contactOsc;
	double contactSignal;
	double envSig;
	int trigger;
	maxiFilter lowPass;
	double finalOut;
	maxiFFT fft;
	float specCent;
	float* avgPow;
	maxiOsc phase;
	float rand;
	double trig;
	float fundFreq;
};
