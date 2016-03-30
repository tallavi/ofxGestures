//
//  ofxGesturesStates.hpp
//  TrailzeApp
//
//  Created by Artem Pyatishev on 3/27/16.
//
//

#pragma once

#include "ofxGestures.h"
#include <Poco/Timer.h>

class NoneState : public ofxGestures::BaseState{
public:
    NoneState();
    ~NoneState();
    
    virtual bool touchDown(ofTouchEventArgs & touch)override;
};

class FirstTouchState : public ofxGestures::BaseState{
public:
    FirstTouchState();
    ~FirstTouchState();
    
    virtual bool touchDown(ofTouchEventArgs & touch)override;
    virtual bool touchMoved(ofTouchEventArgs & touch)override;
    virtual bool touchUp(ofTouchEventArgs & touch)override;
private:
    void onTimer(Poco::Timer& timer);
    
    Poco::Timer m_tapTimer;
    Poco::Timestamp m_initialTime;
};

class PanState : public ofxGestures::BaseState{
public:
    PanState();
    ~PanState();
    
    virtual bool touchDown(ofTouchEventArgs & touch)override;
    virtual bool touchMoved(ofTouchEventArgs & touch)override;
    virtual bool touchUp(ofTouchEventArgs & touch)override;
    
    virtual Poco::Nullable<ofxGestures::PanEventArgs> getPanEventArgs()override;
protected:
    virtual void notifyStart()override;
private:
    Poco::Nullable<ofxGestures::PanEventArgs> m_panArgs;
};

class PinchState : public ofxGestures::BaseState{
public:
    PinchState();
    ~PinchState();
    
    bool touchDown(ofTouchEventArgs & touch)override;
    bool touchMoved(ofTouchEventArgs & touch)override;
    bool touchUp(ofTouchEventArgs & touch)override;
    
    virtual Poco::Nullable<ofxGestures::PinchEventArgs> getPinchEventArgs()override;
    
protected:
    virtual void notifyStart()override;
    
    bool notifyPinchEvent();
    bool notifyPinchEventEnded();
    
    Poco::Nullable<ofxGestures::PinchEventArgs> m_pinchArgs;
};

