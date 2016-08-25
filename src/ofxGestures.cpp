//
//  ofxGestures.cpp
//
//  Created by Tal Lavi on 10/21/14.
//
//

#include "ofxGestures.h"
#include "ofxGesturesStates.h"

//====================== ofxGestures ==================//

ofxGestures & ofxGestures::get(){
	static ofxGestures * instance = new ofxGestures();
	return *instance;
}

void ofxGestures::start(){
    
    m_touches.clear();
    m_state = std::make_shared<NoneState>();
    
    ofAddListener(ofEvents().touchDown, this, &ofxGestures::touchDown, OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(ofEvents().touchMoved, this, &ofxGestures::touchMoved, OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(ofEvents().touchUp, this, &ofxGestures::touchUp, OF_EVENT_ORDER_BEFORE_APP);
//    ofAddListener(ofEvents().touchCancelled, this,  &ofxGestures::touchCanceled, OF_EVENT_ORDER_BEFORE_APP);
}

void ofxGestures::stop(){
    ofRemoveListener(ofEvents().touchDown, this, &ofxGestures::touchDown, OF_EVENT_ORDER_BEFORE_APP);
    ofRemoveListener(ofEvents().touchMoved, this, &ofxGestures::touchMoved, OF_EVENT_ORDER_BEFORE_APP);
    ofRemoveListener(ofEvents().touchUp, this, &ofxGestures::touchUp, OF_EVENT_ORDER_BEFORE_APP);
//    ofRemoveListener(ofEvents().touchCancelled, this,  &ofxGestures::touchCanceled, OF_EVENT_ORDER_BEFORE_APP);
}

ofxGestures::ofxGestures()
{
    m_state = std::make_shared<NoneState>();
}

ofxGestures::~ofxGestures()
{}

bool ofxGestures::touchDown(ofTouchEventArgs & touchEventArgs) {
    Touch touch(touchEventArgs);
    m_touches[touchEventArgs.id] = touch;
    ofLogNotice("ofxGestures")<<"touchDown id: "<<touchEventArgs.id;
    return m_state->touchDown(touchEventArgs);
};


bool ofxGestures::touchMoved(ofTouchEventArgs & touch) {
    ofLogNotice("ofxGestures")<<"touchMoved id: "<<touch.id;
    if(m_touches.empty()){
        ofLogNotice("ofxGestures")<<"touchMoved empty";
        return false;
    }
    m_touches[touch.id].setCurrent(touch);
    return m_state->touchMoved(touch);};

bool ofxGestures::touchUp(ofTouchEventArgs &touch) {
    ofLogNotice("ofxGestures")<<"touchUp id: "<<touch.id;
    if(m_touches.empty())
        return false;
    bool result =    m_state->touchUp(touch);
    m_touches.erase(touch.id);
    return result;
};

bool ofxGestures::touchCanceled(ofTouchEventArgs & touch){
    ofLogNotice("ofxGestures")<<"touchCanceled id: "<<touch.id;
    return false;
}

bool ofxGestures::notifyTapEvent(const ofVec2f &tapArg){
    bool attended = false;
    try{
        tapEvent.notify(this, tapArg);
    }catch(...){
        ofLogError("ofxGestures")<<"notifyTapEvent catch exception";
        attended = true;
    }
    return attended;
}

bool ofxGestures::notifyLongPressEvent(const ofVec2f &longPressArg){
    bool attended = false;
    try{
        longPressEvent.notify(this, longPressArg);
    }catch(...){
        ofLogError("ofxGestures")<<"notifyLongPressEvent catch exception";
        attended = true;
    }
    return attended;
}

bool ofxGestures::notifyPanEvent(const ofxGestures::PanEventArgs &panArgs){
    bool attended = false;
    try{
        panEvent.notify(this, panArgs);
    }catch(...){
        ofLogError("ofxGestures")<<"notifyPanEvent catch exception";
        attended = true;
    }
    return attended;
}

bool ofxGestures::notifyPanEventEnded(const ofxGestures::PanEventArgs &panArgs){
    bool attended = false;
    try{
        panEndedEvent.notify(this, panArgs);
    }catch(...){
        ofLogError("ofxGestures")<<"notifyPanEventEnded catch exception";
        attended = true;
    }
    return attended;
}

bool ofxGestures::notifyPinchEvent(const ofxGestures::PinchEventArgs &pinchArgs){
    bool attended = false;
    try{
        pinchEvent.notify(this, pinchArgs);
    }catch(...){
        ofLogError("ofxGestures")<<"notifyPinchEvent catch exception";
        attended = true;
    }
    return attended;
}

bool ofxGestures::notifyPinchEventEnded(const ofxGestures::PinchEventArgs &pinchArgs){
    bool attended = false;
    try{
        pinchEndedEvent.notify(this, pinchArgs);
    }catch(...){
        ofLogError("ofxGestures")<<"notifyPinchEventEnded catch exception";
        attended = true;
    }
    return attended;
}

//================================= Touch ================================//
ofxGestures::Touch::Touch(){}

ofxGestures::Touch::Touch(const ofTouchEventArgs &origin){
    m_previous = origin;
    this->m_origin = origin;
    m_current = origin;
}
void ofxGestures::Touch::setCurrent(const ofTouchEventArgs &current){
    m_previous = this->m_current;
    this->m_current = current;
}

void ofxGestures::Touch::resetOrigin() {
    m_origin = m_previous = m_current;
}

//================================= PanEventArgs ================================//
ofxGestures::PanEventArgs::PanEventArgs(const Touch &touch):m_touch(touch){

}

void ofxGestures::PanEventArgs::setCurrentTouch(const ofTouchEventArgs &touch){
    m_touch.setCurrent(touch);
}

ofVec2f ofxGestures::PanEventArgs::getCurrent()const{
    return m_touch.m_current;
}

ofVec2f ofxGestures::PanEventArgs::getOrigin()const{
    return m_touch.m_origin;
}

ofVec2f ofxGestures::PanEventArgs::getDelta() const {
    
    return m_touch.m_current - m_touch.m_origin;
}

//================================= PinchEventArgs ================================//

ofxGestures::PinchEventArgs::PinchEventArgs(const Touch &firstTouch, const Touch &secondTouch):m_firstTouch(firstTouch), m_secondTouch(secondTouch){

}

void ofxGestures::PinchEventArgs::setCurrentTouch(const ofTouchEventArgs &touch){
    if(touch.id == 0)
        setFirstCurrentTouch(touch);
    else if(touch.id == 1)
        setSecondCurrentTouch(touch);
}

void ofxGestures::PinchEventArgs::setFirstTouch(const Touch &touch){
    m_firstTouch = touch;
}

void ofxGestures::PinchEventArgs::setSecondTouch(const Touch &touch){
    m_secondTouch = touch;
}

void ofxGestures::PinchEventArgs::setFirstCurrentTouch(const ofTouchEventArgs &touch){
    m_firstTouch.setCurrent(touch);
}

void ofxGestures::PinchEventArgs::setSecondCurrentTouch(const ofTouchEventArgs &touch){
    m_secondTouch.setCurrent(touch);
}

ofVec2f ofxGestures::PinchEventArgs::getOrigin() const {
    
    return (m_secondTouch.m_origin + m_firstTouch.m_origin) / 2.0;
}

ofVec2f ofxGestures::PinchEventArgs::getPrevious() const {
    return (m_secondTouch.m_previous + m_firstTouch.m_previous) / 2.0;
}

ofVec2f ofxGestures::PinchEventArgs::getCurrent() const {
    
    return (m_secondTouch.m_current + m_firstTouch.m_current) / 2.0;
}

double ofxGestures::PinchEventArgs::getScale() const {
    
    ofVec2f currentDelta = m_secondTouch.m_current - m_firstTouch.m_current;
    ofVec2f originDelta = m_secondTouch.m_origin - m_firstTouch.m_origin;
    
    double currentLength = currentDelta.length();
    double originLength = originDelta.length();
    
    return currentLength / originLength;
}

double ofxGestures::PinchEventArgs::getRelativeScale() const {
    
    ofVec2f currentDelta = m_secondTouch.m_current - m_firstTouch.m_current;
    ofVec2f previousDelta = m_secondTouch.m_previous - m_firstTouch.m_previous;
    
    double currentLength = currentDelta.length();
    double previousLength = previousDelta.length();
    
    return currentLength / previousLength;
}

ofVec2f ofxGestures::PinchEventArgs::getDelta() const {
    
    return (m_secondTouch.m_current + m_firstTouch.m_current - m_secondTouch.m_origin - m_firstTouch.m_origin) / 2.0;
}

ofVec2f ofxGestures::PinchEventArgs::getRelativeDelta() const {
    
    return (m_secondTouch.m_current + m_firstTouch.m_current - m_secondTouch.m_previous - m_firstTouch.m_previous) / 2.0;
}

double ofxGestures::PinchEventArgs::getAngle() const {
 
    return getAngle(m_firstTouch.m_origin, m_secondTouch.m_origin);
}

double ofxGestures::PinchEventArgs::getRelativeAngle() const {

    return getAngle(m_firstTouch.m_previous, m_secondTouch.m_previous);
}

double ofxGestures::PinchEventArgs::getAngle(const ofTouchEventArgs &first, const ofTouchEventArgs &second)const{
    ofVec2f currentDelta = m_secondTouch.m_current - m_firstTouch.m_current;
    ofVec2f originDelta = second - first;
    
    double currentLength = currentDelta.length();
    double originLength = originDelta.length();
    
    double currentAngle = ofRadToDeg(asin(currentDelta.x / currentLength));
    double originAngle = ofRadToDeg(asin(originDelta.x / originLength));
    
    if (m_secondTouch.m_current.y > m_firstTouch.m_current.y) currentAngle = 180.0 - currentAngle;
    if (second.y >  first.y) originAngle = 180.0 - originAngle;
    
    return currentAngle - originAngle;
}

//========================= Base state =========================//
ofxGestures::BaseState::BaseState(){}
ofxGestures::BaseState::~BaseState(){}

bool ofxGestures::BaseState::touchDown(ofTouchEventArgs & touch){
    return false;
}

bool ofxGestures::BaseState::touchMoved(ofTouchEventArgs & touch){
    return false;
}

bool ofxGestures::BaseState::touchUp(ofTouchEventArgs & touch){
    return false;
}

Poco::Nullable<ofxGestures::PanEventArgs> ofxGestures::BaseState::getPanEventArgs(){
    return Poco::Nullable<ofxGestures::PanEventArgs>();
}
Poco::Nullable<ofxGestures::PinchEventArgs> ofxGestures::BaseState::getPinchEventArgs(){
    return Poco::Nullable<ofxGestures::PinchEventArgs>();
}



