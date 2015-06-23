//
//  ofxGestures.h
//
//  Created by Tal Lavi on 10/21/14.
//
//

#pragma once

#include "ofMain.h"
#include <Poco/Nullable.h>

class ofxGestures
{
public:
    
    class PanGestureEventArgs{
    public:
        
        ofTouchEventArgs origin;
        ofTouchEventArgs current;

        ofVec2f getDelta() const;
    };
    
    ofEvent<const PanGestureEventArgs> panGestureEvent;
    ofEvent<const PanGestureEventArgs> panGestureEndedEvent;
    
    class PinchGestureEventArgs{
    public:
        ofTouchEventArgs origin1;
        ofTouchEventArgs origin2;
        ofTouchEventArgs previous1;
        ofTouchEventArgs previous2;
        ofTouchEventArgs current1;
        ofTouchEventArgs current2;
        
        ofVec2f getOrigin() const;
        ofVec2f getPrevious() const;
        ofVec2f getCurrent() const;
        
        double getScale() const;
        double getRelativeScale() const;
        
        bool isExtended;
        
        ofVec2f getDelta() const;
        ofVec2f getRelativeDelta() const;
        double getAngle() const;
        double getRelativeAngle() const;
    };

    ofEvent<const PinchGestureEventArgs> pinchGestureEvent;
    ofEvent<const PinchGestureEventArgs> pinchGestureEndedEvent;
    
    const Poco::Nullable<PanGestureEventArgs>& getCurrentPanEvent(){return m_currentPanEvent;}
    const Poco::Nullable<PinchGestureEventArgs>& getCurrentPinchEvent(){return m_currentPinchEvent;}
    
    class TapGestureEventArgs{
    public:
        ofVec2f origin;
    };
    
    ofEvent<const TapGestureEventArgs> tapGestureEvent;
    
    static ofxGestures & get();

private:
    bool touchDown(ofTouchEventArgs & touch);
    bool touchMoved(ofTouchEventArgs & touch);
    bool touchUp(ofTouchEventArgs & touch);
    
    bool touchExists(int touchNum){return (m_touches.find(touchNum) != m_touches.end());}

    std::map<int, ofTouchEventArgs> m_touches;
    
    Poco::Nullable<PanGestureEventArgs> m_currentPanEvent;
    Poco::Nullable<PinchGestureEventArgs> m_currentPinchEvent;

    ofxGestures();

public:
    ~ofxGestures();
};

