#include "Patterns.h"
#include "Grabber.h"

Patterns::Patterns(void)
{
}


Patterns::~Patterns(void)
{
}

void Patterns:: structuralSetup(){
	ofSetVerticalSync(true);
	currentCount = 1;
	mouseRect = mouseRectSize;
	ofEnableSmoothing(); 
	//first circle
	xc[0] = firstCircleX;
	yc[0] = firstCircleY;
	cc[0].set(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
	r[0] = firstCircleRadius;
	closestIndex[0] = 0;
	closestIndex[1] = 0;
	previousMouseRect = 0; 
	xChanged = yChanged = zChanged = false; 
	point.set(0,0,0);
	oldPoint.set(0,0,0);
}

void Patterns:: tragectorySetUp(){
	logo.loadImage(cLogoPath);
	cLogo.loadImage(logoPath);
}

bool Patterns:: structualUpdate(ofPoint p, ofPoint oP, bool x, bool y, bool z){
	xChanged = x;
	yChanged = y;
	zChanged = z;
	point = p;
	oldPoint = oP; 
	if(zChanged){
		previousMouseRect = mouseRect;
		if( oldPoint.z < point.z  && mouseRect < maxRectLength ||  mouseRect <= 0){
			mouseRect += rectRate; 
		}
		if( oldPoint.z > point.z  && mouseRect > 0 || mouseRect >= maxRectLength){
			mouseRect -= rectRate;
		}
	}else {
		mouseRect = previousMouseRect;
	}
	if (xChanged == true || yChanged == true || zChanged == true) {
		// create a random position
		float newX = ofRandom(point.x-mouseRect/2, point.x+mouseRect/2);
		float newY = ofRandom(point.y-mouseRect/2, point.y+mouseRect/2);
		float newR = 1;
		bool intersection = false;
		// find out, if new circle intersects with one of the others
		for(int i=0; i < currentCount; i++) {
			float d = ofDist(newX,newY, xc[i],yc[i]);
			if (d < (newR + r[i])) {
				intersection = true;
				break;
			}
		}
		// no intersection ... add a new circle
		if (intersection == false) {
			// get closest neighbour and closest possible radius
			float newRadius = ofGetWidth();
			for(int i=0; i < currentCount; i++) {
				float d = ofDist(newX,newY, xc[i], yc[i]);
				if (newRadius > d-r[i]) {
					newRadius = d-r[i];
					closestIndex[currentCount] = i;
				}
			}
			if (newRadius > maxRadius) {
				newRadius = maxRadius;
			}
			xc[currentCount] = newX;
			yc[currentCount] = newY;
			cc[currentCount].set(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
			r[currentCount] = newRadius;
			currentCount++;
		}
	}

	if (currentCount >= maxCount) {
		for (int i=0 ; i < currentCount; i++) {
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glFlush();
		}
		currentCount = 0;
		return true;
	} else {
		return false; 
	}
}

void Patterns:: structural(){
	// draw them //
	//visualize the range of the new positions
	if (xChanged == true || yChanged == true || zChanged == true) {
		ofNoFill();
		ofSetColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
		ofSetLineWidth(3.5);
		ofRect(point.x-mouseRect/2, point.y-mouseRect/2, mouseRect, mouseRect);
	}
	for (int i=0 ; i < currentCount; i++) {
		ofFill();
		ofSetColor(cc[i]);
		ofEllipse(xc[i],yc[i], r[i]*2,r[i]*2);
		//ofSetColor(226, 185, 0);
		//ofSetLineWidth(0.75);
		//int n = closestIndex[i];
		//ofLine(xc[i],yc[i], xc[n],yc[n]);
	}	
}

void Patterns:: tragectory(ofPoint point, ofPoint oldPoint, bool xChanged, bool yChanged, int pattern){
	if(pattern == MINILOGO){
		ofSetColor(255,255,255);
		//ofRect(0, 0, point.z, ofGetWidth(), ofGetHeight());
		logo.draw(point.x, point.y, point.z, logoSizeX, logoSizeY);
	}
	if(pattern == CLOGO){
		ofSetColor(255,255,255);
		//ofRect(0, 0, point.z, ofGetWidth(), ofGetHeight());
		cLogo.draw(point.x, point.y, point.z, cLogSizeX, cLogoSizeY);
	}
}

