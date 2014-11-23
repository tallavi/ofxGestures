//
//  ofxGestures.h
//
//  Created by Tal Lavi on 10/21/14.
//
//

#pragma once

#include "ofMain.h"

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
    
    bool touchExists(int touchNum){return (m_touches.find(touchNum) != m_touches.end());}

public:
    
    ofEvent<void> panGestureEvent;
    ofEvent<void> panGestureEndedEvent;
    
    ofEvent<void> pinchGestureEvent;
    ofEvent<void> pinchGestureEndedEvent;
    
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

