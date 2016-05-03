//
//  ofxGesturesStates.cpp
//  TrailzeApp
//
//  Created by Artem Pyatishev on 3/27/16.
//
//

#include "ofxGesturesStates.h"

float getPixelsByPercent(int percent){
    return ofGetWindowWidth()*percent / 100.0;
}

const int& PAN_DELTA_TOLLERANCE(){
    static const int pan_delta_tollerance = getPixelsByPercent(5);
    return pan_delta_tollerance;
}

const double TAP_MAX_DURATION_MICROSECOND(0.5 * 1000 * 1000);
const double LONG_PRESS_DURATION_MILLISECOND(0.75 * 1000);

//================================ None State ==================================//
NoneState::NoneState(){
    ofLogNotice("ofxGestures")<<"NoneState";
}

NoneState::~NoneState(){
}

bool NoneState::touchDown(ofTouchEventArgs &touch){
    if(ofxGestures::get().m_touches.size() == 1)
        setNextState<FirstTouchState>();
    return false;
}

//================================ FirstTouch State ==================================//
FirstTouchState::FirstTouchState(){
    ofLogNotice("ofxGestures")<<"FirstTouchState";
    m_tapTimer.setStartInterval(LONG_PRESS_DURATION_MILLISECOND);
    m_tapTimer.start(Poco::TimerCallback<FirstTouchState>(*this, &FirstTouchState::onTimer));
}

FirstTouchState::~FirstTouchState(){
}

bool FirstTouchState::touchDown(ofTouchEventArgs & touch){
    if(m_isStateInvalide){
        setNextState<NoneState>();
        return false;
    }
    m_tapTimer.stop();
    setNextState<PinchState>();
    return false;
}

bool FirstTouchState::touchMoved(ofTouchEventArgs & touch){
    if(m_isStateInvalide){
        setNextState<NoneState>();
        return false;
    }
    ofxGestures::Touch &current_touch = ofxGestures::get().m_touches[0];
    if((current_touch.m_origin - current_touch.m_current).length() > PAN_DELTA_TOLLERANCE()){
        m_tapTimer.stop();
        setNextState<PanState>();
    }
    return false;
}

bool FirstTouchState::touchUp(ofTouchEventArgs & touch){
    bool attended = false;
    if(!m_isStateInvalide)
    {
        m_tapTimer.stop();
        Poco::Timestamp::TimeDiff deltaTime = Poco::Timestamp() - m_initialTime;
        if(deltaTime < TAP_MAX_DURATION_MICROSECOND){
            attended = ofxGestures::get().notifyTapEvent(ofxGestures::get().m_touches[0].m_origin);
        }
    }
    setNextState<NoneState>();
    return attended;
}

void FirstTouchState::onTimer(Poco::Timer& timer){
    ofxGestures::get().notifyLongPressEvent(ofxGestures::get().m_touches[0].m_origin);
    m_isStateInvalide = true;
}

//================================ Pan State ==================================//
PanState::PanState(){
    ofLogNotice("ofxGestures")<<"PanState";
    m_panArgs.assign(ofxGestures::PanEventArgs(ofxGestures::get().m_touches[0]));
}

PanState::~PanState(){
}

void PanState::notifyStart(){
    ofxGestures::get().notifyPanEvent(m_panArgs);
}

bool PanState::touchDown(ofTouchEventArgs & touch){
    m_panArgs.value().setCurrentTouch(touch);
    bool attended = ofxGestures::get().notifyPanEventEnded(m_panArgs);
    setNextState<PinchState>();
    return attended;
}

bool PanState::touchMoved(ofTouchEventArgs & touch){
    m_panArgs.value().setCurrentTouch(touch);
    return ofxGestures::get().notifyPanEvent(m_panArgs);
}

bool PanState::touchUp(ofTouchEventArgs & touch){
    m_panArgs.value().setCurrentTouch(touch);
    bool attended = ofxGestures::get().notifyPanEventEnded(m_panArgs);
    setNextState<NoneState>();
    return attended;
}

Poco::Nullable<ofxGestures::PanEventArgs> PanState::getPanEventArgs(){
    return m_panArgs;
}

//================================ Pinch State ==================================//

PinchState::PinchState() {
    ofLogNotice("ofxGestures")<<"PinchState";
    m_pinchArgs.assign(ofxGestures::PinchEventArgs(ofxGestures::get().m_touches[0], ofxGestures::get().m_touches[1]));
}

PinchState::~PinchState(){
    ofLogNotice("PinchState")<<"destroy";
}

void PinchState::notifyStart(){
    notifyPinchEvent();
}

bool PinchState::touchDown(ofTouchEventArgs & touch){
    bool attended = false;
    if(touch.id == 1 || touch.id == 0){
        m_pinchArgs.assign(ofxGestures::PinchEventArgs(ofxGestures::get().m_touches[0], ofxGestures::get().m_touches[1]));
        attended = notifyPinchEvent();
    }
    return attended;
}

bool PinchState::touchMoved(ofTouchEventArgs & touch){
    bool attended = false;
    
    if(!m_pinchArgs.isNull() && (touch.id == 1 || touch.id == 0)){
        m_pinchArgs.value().setCurrentTouch(touch);
        attended = notifyPinchEvent();
    }
    return attended;
}

bool PinchState::touchUp(ofTouchEventArgs & touch){
    bool attended = false;
    if(touch.id == 1 || touch.id == 0){
        if(!m_pinchArgs.isNull()){
            m_pinchArgs.value().setCurrentTouch(touch);
            attended = notifyPinchEventEnded();
            m_pinchArgs.clear();
        }else{
            setNextState<NoneState>();
        }
    }
    return attended;
}

bool PinchState::notifyPinchEvent(){
    return ofxGestures::get().notifyPinchEvent(m_pinchArgs);
}

bool PinchState::notifyPinchEventEnded(){
    return ofxGestures::get().notifyPinchEventEnded(m_pinchArgs);
}

Poco::Nullable<ofxGestures::PinchEventArgs> PinchState::getPinchEventArgs(){
    return m_pinchArgs;
}


