#include "Grabber.h"


Grabber::Grabber(void)
{
}


Grabber::~Grabber(void)
{
}

//-------------------------------------------------------------
void Grabber::setUp(){
	pointHead.set(0,0,0);
	oldPointHead.set(0,0,0);
	hand.set(0,0,0);
	oldHand.set(0,0,0);
	flag = NOTHING;
	pattern = NOTHING; 
	xChanged = yChanged = zChanged = false;
	changeTriggerd = false; 
	buttonRed = true;
	buttonGreen = buttonBlue = false;

}
//-------------------------------------------------------------
void Grabber::flagChange(int valid) {
	if(valid > 0 && maxDistance > pointHead.z){
		if(abs(oldPointHead.x-pointHead.x) > thresholdHead){
			if(pointHead.x < oldPointHead.x){
				flag = HEAD_MOVED_LEFT; 
				if(pattern != NOTHING){
					changeTriggerd = true;
					pattern = NOTHING;
				}
			} else if(pointHead.x > oldPointHead.x){
				flag = HEAD_MOVED_RIGHT;
				if(pattern != NOTHING){
					changeTriggerd = true;
					pattern = NOTHING;
				}			
			} else {
				flag = HEAD_MOVED_UNKNOWN; 
			 }
		} else {
			flag = HEAD_MOVED_LESS;
		}
	}else {
		flag = HEAD_MOVED;
		if(pattern != NOTHING){
			changeTriggerd = true;
			pattern = NOTHING;
		}
	}
}

//-------------------------------------------------------------
void Grabber:: patternChange(int kinectWidth, int kinectHeight){
	if(buttonRed){
		if(pattern != STRUCTUAL_AGENTS){
			changeTriggerd = true; 
			pattern = STRUCTUAL_AGENTS;
		}			
	} else if (buttonGreen) {
		if(pattern != MINILOGO){
			pattern = MINILOGO;	
			changeTriggerd = true; 
		}
	} else if (buttonBlue){
		if(pattern != CLOGO){
			pattern = CLOGO;
			changeTriggerd = true; 
		}
	} 
	if(ofGetWindowMode() == OF_WINDOW){
			hand.x = pointHand.x*(ofGetWidth()/kinectWidth)*2;
			hand.y = pointHand.y*(ofGetHeight()/kinectHeight)*2;
			oldHand.x = oldPointHand.x*(ofGetWidth()/kinectWidth)*2;
			oldHand.y = oldPointHand.y*(ofGetHeight()/kinectHeight)*2;
	}
	if(ofGetWindowMode() == OF_FULLSCREEN){
			hand.x = pointHand.x*(ofGetScreenWidth()/kinectWidth)*2;
			hand.y = pointHand.y*(ofGetScreenHeight()/kinectHeight)*2;
			oldHand.x = oldPointHand.x*(ofGetScreenWidth()/kinectWidth)*2;
			oldHand.y = oldPointHand.y*(ofGetScreenHeight()/kinectHeight)*2;
	}
	hand.z = pointHand.z;
	oldHand.z = oldPointHand.z;

	if(abs(oldPointHand.z - pointHand.z) >= thresholdZ){
		zChanged = true;			 
	} else {
		zChanged = false;
	} 
	if(abs(oldPointHand.x - pointHand.x) >= thresholdX){
		xChanged = true;		
	} else {
		xChanged = false;
	}
	if(abs(oldPointHand.y - pointHand.y) >= thresholdY){
		yChanged = true;	  
	} else {
		yChanged = false;
	}
}

//-------------------------------------------------------------
void Grabber::kinectPositionUpdateHead(){ 
	int dispWidth = ofGetWidth();
	int dispHeight = ofGetHeight();
	if (int n = trailHead.size()) {
		pointHead.x *= n+1;
		pointHead.y *= n+1;
		pointHead.z *= n+1;
		for (int i = 0; i < n; i++) {
			pointHead.x += trailHead[i].x * (i+1);
			pointHead.y += trailHead[i].y * (i+1);
			pointHead.z += trailHead[i].z * (i+1);
		}
		pointHead.x /= (n+1)*(n+2)/2;
		pointHead.y /= (n+1)*(n+2)/2;
		pointHead.z /= (n+1)*(n+2)/2;
	}
	trailHead.push_back(pointHead);
	if (trailHead.size() > headSaving) {
		oldPointHead.x = trailHead.at(0).x;
		oldPointHead.y = trailHead.at(0).y;
		oldPointHead.z = trailHead.at(0).z;
		trailHead.pop_front();
	}
	if (int n = oldTrailHead.size()) {
		oldPointHead.x *= n+1;
		oldPointHead.y *= n+1;
		oldPointHead.z *= n+1;
		for (int i = 0; i < n; i++) {
			oldPointHead.x += oldTrailHead[i].x * (i+1);
			oldPointHead.y += oldTrailHead[i].y * (i+1);
			oldPointHead.z += oldTrailHead[i].z * (i+1);
		}
		oldPointHead.x /= (n+1)*(n+2)/2;
		oldPointHead.y /= (n+1)*(n+2)/2;
		oldPointHead.z /= (n+1)*(n+2)/2;
	}
	oldTrailHead.push_back(oldPointHead);
	if (oldTrailHead.size() > headSaving) {
		oldTrailHead.pop_front();
	}
}

//--------------------------------------------------------------
void Grabber::kinectPositionUpdateHands() {	
	int dispWidth = ofGetWidth();
	int dispHeight = ofGetHeight();
	if (int n = trailHand.size()) {
		pointHand.x *= n+1;
		pointHand.y *= n+1;
		pointHand.z *= n+1;
		for (int i = 0; i < n; i++) {
			pointHand.x += trailHand[i].x * (i+1);
			pointHand.y += trailHand[i].y * (i+1);
			pointHand.z += trailHand[i].z * (i+1);
		}
		pointHand.x /= (n+1)*(n+2)/2;
		pointHand.y /= (n+1)*(n+2)/2;
		pointHand.z /= (n+1)*(n+2)/2;
	}
	trailHand.push_back(pointHand);
	if (trailHand.size() > handSaving) {
		oldPointHand.x = trailHand.at(0).x;
		oldPointHand.y = trailHand.at(0).y;
		oldPointHand.z = trailHand.at(0).z;
		trailHand.pop_front();
	}
	if (int n = oldTrailHand.size()) {
		oldPointHand.x *= n+1;
		oldPointHand.y *= n+1;
		oldPointHand.z *= n+1;
		for (int i = 0; i < n; i++) {
			oldPointHand.x += oldTrailHand[i].x * (i+1);
			oldPointHand.y += oldTrailHand[i].y * (i+1);
			oldPointHand.z += oldTrailHand[i].z * (i+1);
		}
		oldPointHand.x /= (n+1)*(n+2)/2;
		oldPointHand.y /= (n+1)*(n+2)/2;
		oldPointHand.z /= (n+1)*(n+2)/2;
	}
	oldTrailHand.push_back(oldPointHand);
	if (oldTrailHand.size() > handSaving) {
		oldTrailHand.pop_front();
	}
}
