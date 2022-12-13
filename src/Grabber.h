#pragma once
#include "ofMain.h"
/******************************************************************/
/**
 * @file	grabber.h
 * @brief	handle inputs 
 * @note
 * @todo
 * @bug
 *
 * @author	Nanaka Okamoto (Kirby)
 * @date	21. June, 2013
 */
 /******************************************************************/


static const int thresholdHead = 30; 
static const int thresholdX = 25;
static const int thresholdY = 25;
static const int thresholdZ = 28;
static const int handSaving = 10;
static const int headSaving = 4;
//TO DO: the user far away should not be ditected this can only be tested on site
static const int maxDistance = 800;  
#define HEAD_MOVED_LEFT 1
#define HEAD_MOVED_RIGHT 2
#define HEAD_MOVED_UNKNOWN 3
#define HEAD_MOVED_LESS 4
#define HEAD_MOVED 5
#define NOTHING 0
#define STRUCTUAL_AGENTS 1
#define MINILOGO 2
#define CLOGO 3


class Grabber
{
public:
	Grabber(void);
	~Grabber(void);

	void setUp(); 
	void flagChange(int);
	void patternChange(int, int); 
	void kinectPositionUpdateHead();
	void kinectPositionUpdateHands();


	ofPoint oldPointHead;
	ofPoint pointHead;
	ofPoint oldPointHand;
	ofPoint hand; 
	ofPoint oldHand;
	ofPoint pointHand;
	deque<ofPoint>      trailHead;
	deque<ofPoint>		oldTrailHead;
	deque<ofPoint>      trailHand;
	deque<ofPoint>		oldTrailHand;
	ofPoint head;
	ofPoint oldHead;
	int flag;  
	int	pattern; 
	bool zChanged;
	bool xChanged;
	bool yChanged;
	bool changeTriggerd; //when the pattern is just switched 

	bool buttonRed;
	bool buttonGreen;
	bool buttonBlue;
};

