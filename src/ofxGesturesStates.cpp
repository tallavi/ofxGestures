//
//  ofxGesturesStates.cpp
//  TrailzeApp
//
//  Created by Artem Pyatishev on 3/27/16.
//
//

#include "ofxGesturesStates.h"
#include "EnvironmentHelper.h"

//const int PAN_DELTA_TOLLERANCE = 1;
//const int PINCH_DELTA_TOLLERANCE = 4;

const double TAP_MAX_TIME = 0.5;
const int PINCH_ANGLE_TOLLERANCE = 30;

const double PRESS_LONG_TIME(0.75);

float getPixelsByPercent(int percent){
    return ofGetWindowWidth()*percent / 100.0;
}

const int PAN_DELTA_TOLLERANCE(){
    static const int pan_delta_tollerance = getPixelsByPercent(5);
    return pan_delta_tollerance;
}

const int PINCH_DELTA_TOLLERANCE(){
    static const int pinch_delta_tollerance = getPixelsByPercent(30);
    return pinch_delta_tollerance;
}

const double TAP_MAX_TIME_MICROSECOND(){
    static const double tap_max_time(TAP_MAX_TIME * 1000 * 1000);
    return tap_max_time;
}

const double PRESS_LONG_TIME_MILLISECOND(){
    static const double press_long_time(PRESS_LONG_TIME * 1000);
    return press_long_time;
}

//================================ None State ==================================//
NoneState::NoneState(){
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
    m_tapTimer.setStartInterval(PRESS_LONG_TIME_MILLISECOND());
    m_tapTimer.start(Poco::TimerCallback<FirstTouchState>(*this, &FirstTouchState::onTimer));
}

FirstTouchState::~FirstTouchState(){
}

bool FirstTouchState::touchDown(ofTouchEventArgs & touch){
    m_tapTimer.stop();
    setNextState<PinchState>();
    return false;
}

bool FirstTouchState::touchMoved(ofTouchEventArgs & touch){
    ofxGestures::Touch &current_touch = ofxGestures::get().m_touches[0];
    if((current_touch.m_origin - current_touch.m_current).length() > PAN_DELTA_TOLLERANCE()){
        m_tapTimer.stop();
        setNextState<PanState>();
    }
    return false;
}

bool FirstTouchState::touchUp(ofTouchEventArgs & touch){
    bool attended = false;
    m_tapTimer.stop();
    Poco::Timestamp::TimeDiff deltaTime = Poco::Timestamp() - m_initialTime;
    if(deltaTime < TAP_MAX_TIME_MICROSECOND()){
        attended = ofxGestures::get().notifyTapEvent(ofxGestures::get().m_touches[0].m_origin);
    }
    setNextState<NoneState>();
    return attended;
}

void FirstTouchState::onTimer(Poco::Timer& timer){
    ofxGestures::get().notifyLongPressEvent(ofxGestures::get().m_touches[0].m_origin);
    setNextState<NoneState>();
}

//================================ Pan State ==================================//
PanState::PanState(){
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

//================================ PinchExtended State ==================================//
PinchExtendedState::PinchExtendedState(){
    ofLogNotice("PinchExtendedState")<<"created";
    initialPinchArgs();
}

//PinchExtendedState::PinchExtendedState(bool isNotify){
//    
//    if(isNotify)
//        notifyPinchEvent();
//}

PinchExtendedState::~PinchExtendedState(){
    ofLogNotice("PinchExtendedState")<<"destroy";
}

void PinchExtendedState::notifyStart(){
    notifyPinchEvent();
}

void PinchExtendedState::initialPinchArgs(){
    ofLogNotice("PinchExtendedState")<<"initialPinchArgs";
    m_pinchArgs.assign(ofxGestures::PinchEventArgs(ofxGestures::get().m_touches[0], ofxGestures::get().m_touches[1]));
}

bool PinchExtendedState::innerMove(ofTouchEventArgs & touch){
    if(!m_pinchArgs.isNull() && (touch.id == 1 || touch.id == 0)){
        m_pinchArgs.value().setCurrentTouch(touch);
        return true;
    }
    return false;
}

bool PinchExtendedState::touchDown(ofTouchEventArgs & touch){
    bool attended = false;
    if(touch.id == 1 || touch.id == 0){
        initialPinchArgs();
        attended = notifyPinchEvent();
    }
    return attended;
}

bool PinchExtendedState::touchMoved(ofTouchEventArgs & touch){
    bool attended = false;
    if(innerMove(touch)){
        attended = notifyPinchEvent();
    }
    return attended;
}

bool PinchExtendedState::touchUp(ofTouchEventArgs & touch){
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

bool PinchExtendedState::notifyPinchEvent(){
    return ofxGestures::get().notifyPinchEvent(m_pinchArgs);
}

bool PinchExtendedState::notifyPinchEventEnded(){
    return ofxGestures::get().notifyPinchEventEnded(m_pinchArgs);
}

Poco::Nullable<ofxGestures::PinchEventArgs> PinchExtendedState::getPinchEventArgs(){
    return m_pinchArgs;
}

//================================ Pinch State ==================================//
PinchState::PinchState():PinchExtendedState(){
    ofLogNotice("PinchState")<<"created";
    m_pinchArgs.value().disableExtended();
//    notifyPinchEvent();
}

PinchState::~PinchState(){
    ofLogNotice("PinchState")<<"destroy";
}

void PinchState::initialPinchArgs(){
    PinchExtendedState::initialPinchArgs();
    m_pinchArgs.value().disableExtended();
    ofLogNotice("PinchState")<<"initialPinchArgs";
}

bool PinchState::isExtended(){
    if(PINCH_DELTA_TOLLERANCE() < m_pinchArgs.value().getInnerDelta().length())
        return true;
    if (std::abs(m_pinchArgs.value().getInnerAngle()) > PINCH_ANGLE_TOLLERANCE)
        return true;
    return false;
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
