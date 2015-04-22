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
public:
    class PanEvent{
    public:
        ofVec2f getOrigin() const;
        ofVec2f getDelta() const;
    };

    ofEvent<PanEvent> panGestureEvent;
    ofEvent<PanEvent> panGestureEndedEvent;

    class PinchEvent{
    public:
        ofVec2f getOrigin() const;
        ofVec3f getPrevious() const;
        ofVec2f getDelta() const;
        ofVec2f getRelativeDelta() const;
        double getAngle() const;
        double getScale() const;
        double getRelativeAngle() const;
        double getRelativeScale() const;
    };
    
    ofEvent<PinchEvent> pinchGestureEvent;
    ofEvent<PinchEvent> pinchGestureEndedEvent;
    
    static ofxGestures & get();
    bool isPanning() const {return m_isPanning;}
    ofVec2f getPanOrigin() const;
    ofVec2f getPanDelta() const;
    
    bool isPinching() const {return m_isPinching;}
    ofVec2f getPinchOrigin() const;
    ofVec2f getPinchPrevious() const;
    ofVec2f getPinchDelta() const;
    ofVec2f getPinchRelativeDelta() const;
    double getPinchAngle() const;
    double getPinchRelativeAngle() const;
    double getPinchScale() const;
    double getPinchRelativeScale() const;

private:
    bool touchDown(ofTouchEventArgs & touch);
    bool touchMoved(ofTouchEventArgs & touch);
    bool touchUp(ofTouchEventArgs & touch);

    bool touchExists(int touchNum){return (m_touches.find(touchNum) != m_touches.end());}

    std::map<int, ofTouchEventArgs> m_touches;

    bool m_isPanning;
    ofTouchEventArgs m_panOrigin;
    ofTouchEventArgs m_panCurrent;

    bool m_isPinching;
    ofTouchEventArgs m_pinchOrigin1;
    ofTouchEventArgs m_pinchOrigin2;
    ofTouchEventArgs m_pinchPrevious1;
    ofTouchEventArgs m_pinchPrevious2;
    ofTouchEventArgs m_pinchCurrent1;
    ofTouchEventArgs m_pinchCurrent2;

    PinchEvent pinch;
    PanEvent pan;

    ofxGestures();

public:
    ~ofxGestures();
};

