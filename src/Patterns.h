/******************************************************************/
/**
 * @file	pattern.h
 * @brief	this handles te drawing patterns
 * @note	
 * @todo
 * @bug
 *
 * @author	Nanaka Okamoto
 * @date	21. June, 2013
 */
 /******************************************************************/
#pragma once

#include "ofMain.h"
#include <string>
using namespace std;

//used in structural()
const float initRadius = 150;
const int maxCount = 400;
const float minRadius = 10;
const float maxRadius = 60;
const float maxRectLength = 350;
const int rectRate = 6; 

//path definition
const string cLogoPath = "C:/Users/User1/Desktop/of_v0.7.4_vs2010_release/apps/myApps/miniProjectProjection/bin/data/pictures/circusLogo.png";
const string logoPath = "C:/Users/User1/Desktop/of_v0.7.4_vs2010_release/apps/myApps/miniProjectProjection/bin/data/pictures/logo.png");

const float mouseRectSize = 30;
//first circle size
const float firstCircleX = 200;
const float firstCircleY = 100;
const float firstCircleRadius = 50;

const float cLogSizeX = 120;
const float cLogoSizeY = 120;
const float logoSizeX = 240;
const float logoSizeY = 120;

class Patterns
{
public:
	Patterns(void);
	~Patterns(void); 

////////used in structural()
	void structuralSetup();
	bool structualUpdate(ofPoint, ofPoint, bool, bool, bool);
	void structural();
	int currentCount;
	ofColor cc[maxCount];
	float xc[maxCount];
	float yc[maxCount];
	float r[maxCount];
	int closestIndex[maxCount];
	float mouseRect;
	bool xChanged;
	bool yChanged;
	bool zChanged;
	ofPoint point;
	ofPoint oldPoint;
	int previousMouseRect;

////////used in tragectory()
	void tragectorySetUp();
	void tragectory(ofPoint, ofPoint, bool, bool, int);
	ofImage logo;
	ofImage cLogo; 
};

