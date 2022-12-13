/******************************************************************/
/**
 * @file	testApp.h
 * @brief	this app creates strctured patterns using kinect hand movement and arduino inputs
 * @note	you need openframeworks sdk and arduinos sdk
 * @todo
 * @bug
 *
 * @author	Nanaka Okamoto
 * @date	21. June, 2013
 */
 /******************************************************************/
#pragma once

#include "ofxKinectNui.h"
#include "ofMain.h"
#include "ofxKinectNuiPlayer.h"
#include "ofxKinectNuiRecorder.h"
#include "Patterns.h" 
#include "Grabber.h"


class ofxKinectNuiDrawTexture;
class ofxKinectNuiDrawSkeleton;

static const string portNumber = "COM3";
static const int portBPS = 57600;  
static const int red = 5;
static const int green = 7;
static const int blue = 9;
static const int flash = 13; 

// uncomment this to read from two kinects simultaneously
//#define USE_TWO_KINECTS


class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void drawCalibratedTexture();
		void exit();
		void drawCircle3f(int n, int radius, ofVec3f cur);
		void kinectCheckHead(); 
		void kinectCheckHand(); 
		void kinectInitializeationRelated(); 

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void kinectPlugged();
		void kinectUnplugged();
		
		void startRecording();
		void stopRecording();
		void startPlayback();
		void stopPlayback();


		void setupArduino(const int & version);
		void updateArduino();

		ofxKinectNui kinect;

#ifdef USE_TWO_KINECTS
		ofxKinectNui kinect2;
#endif
		ofxKinectNuiPlayer kinectPlayer;
		ofxKinectNuiRecorder kinectRecorder;

		ofxBase3DVideo* kinectSource;

		ofTexture calibratedTexture;

		bool bRecord;
		bool bPlayback;
		bool bDrawVideo;
		bool bDrawDepthLabel;
		bool bDrawSkeleton;
		bool bDrawCalibratedTexture;
		bool bPlugged;
		bool bUnplugged;
		
		unsigned short nearClipping;
		unsigned short farClipping;
		int angle;
		
		int mRotationX, mRotationY;

		// Please declare these texture pointer and initialize when you want to draw them
		ofxKinectNuiDrawTexture*	videoDraw_;
		ofxKinectNuiDrawTexture*	depthDraw_;
		ofxKinectNuiDrawTexture*	labelDraw_;
		ofxKinectNuiDrawSkeleton*	skeletonDraw_;

		int valid; 
		//bool isStarted; 
		bool debug;
		bool stopStructural; 
		Patterns pat; 
		Grabber gr; 


		ofArduino ard;       
		bool bSetupArduino;

		float kinectSize = 300f;
		float videoSize = 300f;
		const int kinectPoints = 6;
		float minDistance = 500f;
		float maxDistance = 1500f;
		int originX = 512;
		int originY = 386;
		int boxSize = 5;
		const string recordFileName = "recording.dat";

};
