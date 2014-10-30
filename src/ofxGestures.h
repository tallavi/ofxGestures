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
    
    bool isPanning() const {return m_isPanning;}
    ofVec2f getPanOrigin() const;
    ofVec2f getPanDelta() const;
    
    bool isPinching() const {return m_isPinching;}
    ofVec2f getPinchOrigin() const;
    ofVec2f getPinchDelta() const;
    double getPinchAngle() const;
    double getPinchScale() const;
};

