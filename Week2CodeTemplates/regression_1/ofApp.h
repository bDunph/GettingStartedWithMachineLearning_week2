#pragma once

#include "ofMain.h"
#include "ofxGui.h"

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
	/*Step 1: put data vector and output vector here*/
	/*Step 2: put neural network object here*/
	bool isRecording;
	bool isTrained;
	bool isEmpty;
	bool isRunning;

	/* GUI */
	ofxPanel gui;
	ofxFloatSlider slider;
	std::vector<ofPolyline> lines;
	ofPolyline tempLine;
		 
};
