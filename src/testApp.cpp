#include "testApp.h"
#include "ofxKinectNuiDraw.h"

//--------------------------------------------------------------
void testApp::setup() {
	ofSetLogLevel(OF_LOG_VERBOSE);
	kinectInitializeationRelated();
	valid = 0; 
	debug  = false; 
	ofBackground(255,255,255);
	ofSetVerticalSync(true);
	pat.structuralSetup();
	pat.tragectorySetUp();
	stopStructural = false; 
	ard.connect(portNumber, portBPS);
	gr.setUp(); 
    ofAddListener(ard.EInitialized, this, &testApp::setupArduino);
    bSetupArduino = false;
}

//--------------------------------------------------------------
void testApp::update() {

	updateArduino();
	if(stopStructural || gr.changeTriggerd){
		if(gr.pattern != STRUCTUAL_AGENTS){
			pat.currentCount = 1;
		}
		stopStructural = false;
	}
	
	kinectSource->update();
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif
	if(bRecord){
		kinectRecorder.update();
	}

	kinectCheckHead(); //valid is changed here
	gr.kinectPositionUpdateHead();
	gr.flagChange(valid);
	if(gr.flag == HEAD_MOVED_UNKNOWN || gr.flag == HEAD_MOVED_LESS ){
		kinectCheckHand();
		gr.kinectPositionUpdateHands();
		gr.patternChange(kinect.getVideoResolutionWidth(), kinect.getVideoResolutionHeight());
	}
	if(gr.pattern == STRUCTUAL_AGENTS){
		stopStructural = pat.structualUpdate(gr.hand, gr.oldHand, gr.xChanged, gr.yChanged, gr.zChanged);
	}
	if(stopStructural){
		if(gr.pattern != STRUCTUAL_AGENTS){
			pat.currentCount = 1;
		}
		stopStructural = false;
	}
	
	if(gr.pattern == MINILOGO || gr.pattern == CLOGO){ 
		ofEnableAlphaBlending();
		//glEnable(GL_DEPTH_TEST);
		glDisable(GL_DEPTH_TEST);
		ofSetBackgroundAuto(false);
	} else {
		ofDisableAlphaBlending();
		//glEnable(GL_DEPTH_TEST);
		ofSetBackgroundAuto(true);
	}
}

//--------------------------------------------------------------
void testApp::kinectCheckHead(){
	ofPoint * pnts[kinectPoints];
	int x, y, z = 0;
	gr.oldPointHead = gr.pointHead; 
	valid = kinect.getSkeletonPoints(pnts);
	for(int i=0; i < kinectPoints; i++){    
		if(kinect.isTrackedSkeleton(i)){      
				gr.pointHead.x = pnts[i][NUI_SKELETON_POSITION_HEAD].x; 
				gr.pointHead.y = pnts[i][NUI_SKELETON_POSITION_HEAD].y;    
				gr.pointHead.z = pnts[i][NUI_SKELETON_POSITION_HEAD].z/25.4; // they say mm*(600)dpi/25.4 = px 
				break;
		}    
	}  
}

//--------------------------------------------------------------
void testApp::kinectCheckHand(){
	ofPoint * pnts[kinectPoints];
	int x, y, z = 0;
	gr.oldPointHand = gr.pointHand; 
	kinect.getSkeletonPoints(pnts);
	for(int i=0; i < kinectPoints; i++){
		if(kinect.isTrackedSkeleton(i)){      
				gr.pointHand.x = pnts[i][NUI_SKELETON_POSITION_HAND_RIGHT].x;
				gr.pointHand.y = pnts[i][NUI_SKELETON_POSITION_HAND_RIGHT].y;    
				gr.pointHand.z = pnts[i][NUI_SKELETON_POSITION_HAND_RIGHT].z/25.4; 
				break;
		}    
	}    
}

//--------------------------------------------------------------
void testApp::draw() {
	if(gr.changeTriggerd){
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glFlush();
		gr.changeTriggerd= false; 
	}
	if(debug){
		ofPushMatrix();
		ofSetColor(255, 255, 255);
		kinect.drawSkeleton(0, 0, kinectSize, kinectSize);
		kinect.drawVideo(0, 0, videoSize, videoSize);
		ofSetColor(255, 0, 0);
        stringstream reportStream;
		reportStream << 
		 "pattern is : " << ofToString(gr.pattern, 1) << " / "
		 << "changeTriggerd is : " << ofToString(gr.changeTriggerd, 1) << " / "
		 << "valid is : " << ofToString(valid, 1) << " / "
			<< "flag is " << ofToString(gr.flag, 1) << " / ";
			  ofDrawBitmapString(reportStream.str(), 20, 650);
		ofPopMatrix();
	}
	if (gr.pattern == STRUCTUAL_AGENTS){
		 ofPushMatrix();
			pat.structural();
		 ofPopMatrix();
	} else if (gr.pattern == MINILOGO || gr.pattern == CLOGO){
		ofPushMatrix();
			pat.tragectory(gr.hand, gr.oldHand, gr.xChanged, gr.yChanged, gr.pattern);
		ofPopMatrix();
	} 
}

//--------------------------------------------------------------
void testApp::setupArduino(const int & version) {
   ofRemoveListener(ard.EInitialized, this, &testApp::setupArduino);
   int pinCount = 13;
   for (int i = 0; i < pinCount; i++){
	   if(i == red){
		   ard.sendDigitalPinMode(red, ARD_INPUT);
	   } else if(i == green){
		   ard.sendDigitalPinMode(green, ARD_INPUT);
	   } else if(i == blue){
		   ard.sendDigitalPinMode(blue, ARD_INPUT);
	   }else {
		   ard.sendDigitalPinMode(i, ARD_INPUT);
	   }
   }
   ard.sendDigitalPinMode(flash, ARD_OUTPUT);
   bSetupArduino = true;

}

//--------------------------------------------------------------
void testApp::updateArduino(){
   ard.update();
   if(ard.getDigital(red) == ARD_HIGH ){
		gr.buttonRed = true;
		gr.buttonGreen = gr.buttonBlue = false;
		stopStructural = true;
		ofDisableAlphaBlending();
		glEnable(GL_DEPTH_TEST);
		ofSetBackgroundAuto(true);
   } 
   if(ard.getDigital(green) == ARD_HIGH ){
		gr.buttonGreen = true;
		gr.buttonRed = gr.buttonBlue = false;
		gr.changeTriggerd = true; 
   }
   if(ard.getDigital(blue) == ARD_HIGH ){
		gr.buttonBlue = true;
		gr.buttonRed = gr.buttonGreen = false;
		gr.changeTriggerd = true;
   }
}

//--------------------------------------------------------------
void testApp::drawCalibratedTexture(){
	//TODO move number to definition.
	int offsetX = -400;
	int offsetY = -300;
	float drawingSizeCoefficient = 2.5f; 
	glTranslatef(originX, originY, 0);
	calibratedTexture.loadData(kinect.getCalibratedVideoPixels());
	for(int y = 0; y < kinect.getDepthResolutionHeight(); y++){
		for(int x = 0; x < kinect.getDepthResolutionWidth(); x++){
			float distance = kinect.getDistanceAt(x, y);
			if(distance > minDistance && distance < maxDistance){
				glPushMatrix();
				float radius = (maxDistance - distance);
				ofSetColor(kinect.getCalibratedColorAt(x, y));
				ofRotateY(mRotationY);
				ofRotateX(mRotationX);
				glTranslatef(x * drawingSizeCoefficient + offsetX, y * drawingSizeCoefficient + offsetY, radius);
				ofBox(boxSize);
				glPopMatrix();
			}else{
				glPushMatrix();
				ofSetColor(kinect.getCalibratedColorAt(x, y));
				ofRotateY(mRotationY);
				ofRotateX(mRotationX);
				ofRect(x * drawingSizeCoefficient + offsetX, y * drawingSizeCoefficient + offsetY, boxSize, boxSize);
				glPopMatrix();
			}
		}
	}
}

//--------------------------------------------------------------
void testApp::exit() {
	std :: cout << "closing application...." << endl; 
	if(calibratedTexture.bAllocated()){
		calibratedTexture.clear();
	}

	if(videoDraw_) {
		videoDraw_->destroy();
		videoDraw_ = NULL;
	}
	if(depthDraw_) {
		depthDraw_->destroy();
		depthDraw_ = NULL;
	}
	if(labelDraw_) {
		labelDraw_->destroy();
		labelDraw_ = NULL;
	}
	if(skeletonDraw_) {
		delete skeletonDraw_;
		skeletonDraw_ = NULL;
	}

	kinect.setAngle(0);
	kinect.close();
	kinect.removeKinectListener(this);
	kinectPlayer.close();
	kinectRecorder.close();

#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch(key){
	case 'v': // draw video only
	case 'V':
		bDrawVideo = !bDrawVideo;
		if(bDrawVideo){
			bDrawCalibratedTexture = false;
			bDrawSkeleton = false;
			bDrawDepthLabel = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 'd': // draw depth + users label only
	case 'D':
		bDrawDepthLabel = !bDrawDepthLabel;
		if(bDrawDepthLabel){
			bDrawCalibratedTexture = false;
			bDrawVideo = false;
			bDrawSkeleton = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 's': // draw skeleton only
	case 'S':
		bDrawSkeleton = !bDrawSkeleton;
		if(bDrawSkeleton){
			bDrawCalibratedTexture = false;
			bDrawVideo = false;
			bDrawDepthLabel = false;
			glDisable(GL_DEPTH_TEST);
		}
		break;
	case 'q': // draw point cloud example
	case 'Q':
		bDrawCalibratedTexture = !bDrawCalibratedTexture;
		if(bDrawCalibratedTexture){
			bDrawVideo = false;
			bDrawDepthLabel = false;
			bDrawSkeleton = false;
			glEnable(GL_DEPTH_TEST);
		}
		break;
	case 'o': // open stream
	case 'O':
		kinect.open();
		break;
	case 'c': // close stream
	case 'C':
		kinect.close();
		break;
	case 'r': // record stream
	case 'R':
		if(!bRecord){
			startRecording();
		}else{
			stopRecording();
		}
		break;
	case 'p': // playback recorded stream
	case 'P':
		if(!bPlayback){
			startPlayback();
		}else{
			stopPlayback();
		}
		break;
	case OF_KEY_UP: // up the kinect angle
		angle++;
		if(angle > 27){
			angle = 27;
		}
		kinect.setAngle(angle);
		break;
	case OF_KEY_DOWN: // down the kinect angle
		angle--;
		if(angle < -27){
			angle = -27;
		}
		kinect.setAngle(angle);
		break;
	case OF_KEY_LEFT: // increase the far clipping distance
		if(farClipping > nearClipping + 10){
			farClipping -= 10;
			kinectSource->setFarClippingDistance(farClipping);
		}
		break;
	case OF_KEY_RIGHT: // decrease the far clipping distance
		if(farClipping < 4000){
			farClipping += 10;
			kinectSource->setFarClippingDistance(farClipping);
		}
		break;
	case '+': // increase the near clipping distance
		if(nearClipping < farClipping - 10){
			nearClipping += 10;
			kinectSource->setNearClippingDistance(nearClipping);
		}
		break;
	case '-': // decrease the near clipping distance
		if(nearClipping >= 10){
			nearClipping -= 10;
			kinectSource->setNearClippingDistance(nearClipping);
		}
		break;
	case '1':
		if(debug){
			debug = false;
		} else {
			debug = true;
		}   
		break;
	case 'f':
		ofToggleFullscreen();
		gr.changeTriggerd = true; 
         break;
	case '2':
		gr.pattern = MINILOGO; 
		break;
	case '3':
		gr.pattern = CLOGO; 
		break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
	mRotationY = (x - originX) / boxSize;
	mRotationX = (originY - y) / boxSize;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void testApp::kinectPlugged(){
	bPlugged = true;
}

//--------------------------------------------------------------
void testApp::kinectUnplugged(){
	bPlugged = false;
}

//--------------------------------------------------------------
void testApp::startRecording(){
	if(!bRecord){
		// stop playback if running
		stopPlayback();

		kinectRecorder.setup(kinect, recordFileName);
		bRecord = true;
	}
}

//--------------------------------------------------------------
void testApp::stopRecording(){
	if(bRecord){
		kinectRecorder.close();
		bRecord = false;
	}
}

//--------------------------------------------------------------
void testApp::startPlayback(){
	if(!bPlayback){
		stopRecording();
		kinect.close();
		// set record file and source
		kinectPlayer.setup(recordFileName);
		kinectPlayer.loop();
		kinectPlayer.play();
		kinectSource = &kinectPlayer;
		bPlayback = true;
	}
}

//--------------------------------------------------------------
void testApp::stopPlayback(){
	if(bPlayback){
		kinectPlayer.close();
		kinect.open();
		kinectSource = &kinect;
		bPlayback = false;
	}
}

//--------------------------------------------------------------
void testApp::kinectInitializeationRelated(){
	ofxKinectNui::InitSetting initSetting;
	initSetting.grabVideo = true;
	initSetting.grabDepth = true;
	initSetting.grabAudio = true;
	initSetting.grabLabel = true;
	initSetting.grabSkeleton = true;
	initSetting.grabCalibratedVideo = true;
	initSetting.grabLabelCv = true;
	initSetting.videoResolution = NUI_IMAGE_RESOLUTION_640x480;
	initSetting.depthResolution = NUI_IMAGE_RESOLUTION_320x240;
	kinect.init(initSetting);
	kinect.open();
//	kinect.open(true); // when you want to use near mode (default is false)
	kinect.addKinectListener(this, &testApp::kinectPlugged, &testApp::kinectUnplugged);
#ifdef USE_TWO_KINECTS
	// watch out that only the first kinect can grab label and skeleton.
	kinect2.init(true, true, false, false, false, false, false, true);
	kinect2.open();
#endif
	kinectSource = &kinect;
	angle = kinect.getCurrentAngle();
	bRecord = false;
	bPlayback = false;
	bPlugged = kinect.isConnected();
	nearClipping = kinect.getNearClippingDistance();
	farClipping = kinect.getFarClippingDistance();
	
	bDrawVideo = false;
	bDrawDepthLabel = false;
	bDrawSkeleton = false;
	bDrawCalibratedTexture = false;
	ofSetFrameRate(60);
	calibratedTexture.allocate(kinect.getDepthResolutionWidth(), kinect.getDepthResolutionHeight(), GL_RGB);
	videoDraw_ = ofxKinectNuiDrawTexture::createTextureForVideo(kinect.getVideoResolution());
	depthDraw_ = ofxKinectNuiDrawTexture::createTextureForDepth(kinect.getDepthResolution());
	labelDraw_ = ofxKinectNuiDrawTexture::createTextureForLabel(kinect.getDepthResolution());
	skeletonDraw_ = new ofxKinectNuiDrawSkeleton();
	kinect.setVideoDrawer(videoDraw_);
	kinect.setDepthDrawer(depthDraw_);
	kinect.setLabelDrawer(labelDraw_);
	kinect.setSkeletonDrawer(skeletonDraw_);
}