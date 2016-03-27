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
//    virtual bool touchMoved(ofTouchEventArgs & touch)override;
//    virtual bool touchUp(ofTouchEventArgs & touch)override;
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
    
private:
    Poco::Nullable<ofxGestures::PanEventArgs> m_panArgs;
};

class PinchState : public ofxGestures::BaseState{
public:
    PinchState();
    ~PinchState();
    
    virtual bool touchDown(ofTouchEventArgs &touch)override;
    virtual bool touchMoved(ofTouchEventArgs &touch)override;
    virtual bool touchUp(ofTouchEventArgs &touch)override;
    
    virtual Poco::Nullable<ofxGestures::PinchEventArgs> getPinchEventArgs()override;
    
protected:
    bool innerMove(ofTouchEventArgs &touch);
    bool isExtended();
    
    virtual bool notifyPinchEvent();
    virtual bool notifyPinchEventEnded();
    
    Poco::Nullable<ofxGestures::PinchEventArgs> m_pinchArgs;
};

class PinchExtendedState : public PinchState{
public:
    PinchExtendedState();
    ~PinchExtendedState();
    
    virtual bool touchDown(ofTouchEventArgs & touch)override;
    virtual bool touchMoved(ofTouchEventArgs & touch)override;
    virtual bool touchUp(ofTouchEventArgs & touch)override;
    
protected:
    virtual bool notifyPinchEvent()override;
    virtual bool notifyPinchEventEnded()override;
};
