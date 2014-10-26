//
//  ofxGestures.h
//  Trailze
//
//  Created by Tal Lavi on 10/21/14.
//
//

#pragma once

#include "ofMain.h"

//TALTODO: helper method for "isTouchExist"

//TALTODO: allow pan to continue when there's only one finger touching again?

class ofxGestures
{
    
private:
    
    std::map<int, ofTouchEventArgs> m_touches;
    
    bool m_isPanning;
    ofTouchEventArgs m_panOrigin;
    ofTouchEventArgs m_panCurrent;
    
    bool m_isPinching;
    ofTouchEventArgs m_pinchOrigin1;
    ofTouchEventArgs m_pinchOrigin2;
    ofTouchEventArgs m_pinchCurrent1;
    ofTouchEventArgs m_pinchCurrent2;
    
public:
    
    ofxGestures();
    ~ofxGestures();

private:
    
    void touchDown(ofTouchEventArgs & touch);
    void touchMoved(ofTouchEventArgs & touch);
    void touchUp(ofTouchEventArgs & touch);

public:
    
    ofEvent<ofEventArgs> panGestureEvent;
    ofEvent<ofEventArgs> panGestureEndedEvent;
    
    ofEvent<ofEventArgs> pinchGestureEvent;
    ofEvent<ofEventArgs> pinchGestureEndedEvent;
    
public:
    
    bool isPanning(){return m_isPanning;}
    ofVec2f getPanOrigin();
    ofVec2f getPanDelta();
    
    bool isPinching(){return m_isPinching;}
    ofVec2f getPinchOrigin();
    ofVec2f getPinchDelta();
    double getPinchAngle();
    double getPinchScale();
};

