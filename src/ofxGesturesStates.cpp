//
//  ofxGesturesStates.cpp
//  TrailzeApp
//
//  Created by Artem Pyatishev on 3/27/16.
//
//

#include "ofxGesturesStates.h"
#include "EnvironmentHelper.h"

const int PAN_DELTA_TOLLERANCE = 1;
const int PINCH_DELTA_TOLLERANCE = 4;

const double TAP_MAX_TIME = 0.5;
const int PINCH_ANGLE_TOLLERANCE = 30;

const double PRESS_LONG_TIME(1.0);

float getPixelsByPercent(int percent){
    return ofGetWindowWidth()*percent / 100.0;
}

//================================ None State ==================================//
NoneState::NoneState(){
    ofLogNotice("NoneState")<<"created";
}

NoneState::~NoneState(){
    ofLogNotice("NoneState")<<"destroy";
}

bool NoneState::touchDown(ofTouchEventArgs &touch){
    if(ofxGestures::get().m_touches.size() == 1)
        setNextState<FirstTouchState>();
    return false;
}

//================================ FirstTouch State ==================================//
FirstTouchState::FirstTouchState(){
    ofLogNotice("FirstTouchState")<<"created";
    m_tapTimer.setStartInterval(PRESS_LONG_TIME * 1000);
    m_tapTimer.start(Poco::TimerCallback<FirstTouchState>(*this, &FirstTouchState::onTimer));
}

FirstTouchState::~FirstTouchState(){
    ofLogNotice("FirstTouchState")<<"destroy";
}

bool FirstTouchState::touchDown(ofTouchEventArgs & touch){
    m_tapTimer.stop();
    setNextState<PinchState>();
    return false;
}

bool FirstTouchState::touchMoved(ofTouchEventArgs & touch){
    ofxGestures::Touch &current_touch = ofxGestures::get().m_touches[0];
    if((current_touch.m_origin - current_touch.m_current).length() > getPixelsByPercent(PAN_DELTA_TOLLERANCE)){
        m_tapTimer.stop();
        setNextState<PanState>();
    }
    return false;
}

bool FirstTouchState::touchUp(ofTouchEventArgs & touch){
    bool attended = false;
    m_tapTimer.stop();
    Poco::Timestamp::TimeDiff deltaTime = Poco::Timestamp() - m_initialTime;
    if(deltaTime < TAP_MAX_TIME * 1000)
        attended = ofxGestures::get().notifyTapEvent(ofxGestures::get().m_touches[0].m_origin);
    setNextState<NoneState>();
    return attended;
}

void FirstTouchState::onTimer(Poco::Timer& timer){
    ofxGestures::get().notifyLongPressEvent(ofxGestures::get().m_touches[0].m_origin);
    setNextState<NoneState>();
}

//================================ Pan State ==================================//
PanState::PanState(){
    ofLogNotice("PanState")<<"created";
    m_panArgs.assign(ofxGestures::PanEventArgs(ofxGestures::get().m_touches[0]));
    ofxGestures::get().notifyPanEvent(m_panArgs);
}

PanState::~PanState(){
    ofLogNotice("PanState")<<"destroy";
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
PinchState::PinchState(){
    ofLogNotice("PinchState")<<"created";
    m_pinchArgs.assign(ofxGestures::PinchEventArgs(ofxGestures::get().m_touches[0], ofxGestures::get().m_touches[1]));
    notifyPinchEvent();
}

PinchState::~PinchState(){
    ofLogNotice("PinchState")<<"destroy";
}

bool PinchState::innerMove(ofTouchEventArgs &touch){
    if(!m_pinchArgs.isNull() && (touch.id == 1 || touch.id == 0)){
        m_pinchArgs.value().setCurrentTouch(touch);
        return true;
    }
    return false;
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
    if(innerMove(touch)){
        if(isExtended())
            setNextState<PinchExtendedState>();
        else
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

bool PinchState::isExtended(){
    if(getPixelsByPercent(PINCH_DELTA_TOLLERANCE) < m_pinchArgs.value().getInnerDelta().length())
        return true;
    if (m_pinchArgs.value().getInnerAngle() > PINCH_ANGLE_TOLLERANCE)
        return true;
    return false;
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

//================================ PinchExtended State ==================================//
PinchExtendedState::PinchExtendedState():PinchState(){
    ofLogNotice("PinchExtendedState")<<"created";
}

PinchExtendedState::~PinchExtendedState(){
    ofLogNotice("PinchExtendedState")<<"destroy";
}

bool PinchExtendedState::touchDown(ofTouchEventArgs & touch){
    return PinchState::touchDown(touch);
}

bool PinchExtendedState::touchMoved(ofTouchEventArgs & touch){
    if(innerMove(touch))
        return notifyPinchEvent();
    return false;
}

bool PinchExtendedState::touchUp(ofTouchEventArgs & touch){
    return PinchState::touchUp(touch);
}

bool PinchExtendedState::notifyPinchEvent(){
    m_pinchArgs.value().enabledExtended();
    return PinchState::notifyPinchEvent();
}

bool PinchExtendedState::notifyPinchEventEnded(){
    m_pinchArgs.value().enabledExtended();
    return PinchState::notifyPinchEventEnded();
}
