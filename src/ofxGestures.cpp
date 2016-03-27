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

ofxGestures::ofxGestures()
{
    m_state = std::make_shared<NoneState>();
    ofAddListener(ofEvents().touchDown, this, &ofxGestures::touchDown, OF_EVENT_ORDER_BEFORE_APP);
	ofAddListener(ofEvents().touchMoved, this, &ofxGestures::touchMoved, OF_EVENT_ORDER_BEFORE_APP);
    ofAddListener(ofEvents().touchUp, this, &ofxGestures::touchUp, OF_EVENT_ORDER_BEFORE_APP);
}

ofxGestures::~ofxGestures()
{
    ofRemoveListener(ofEvents().touchDown, this, &ofxGestures::touchDown, OF_EVENT_ORDER_BEFORE_APP);
	ofRemoveListener(ofEvents().touchMoved, this, &ofxGestures::touchMoved, OF_EVENT_ORDER_BEFORE_APP);
    ofRemoveListener(ofEvents().touchUp, this, &ofxGestures::touchUp, OF_EVENT_ORDER_BEFORE_APP);
}

bool ofxGestures::touchDown(ofTouchEventArgs & touchEventArgs) {
    Touch touch(touchEventArgs);
    m_touches[touchEventArgs.id] = touch;
    bool attended = m_state->touchDown(touchEventArgs);
    
    //check if should stop panning
//    if (!m_currentPanEvent.isNull() && m_touches.size() > 1)
//    {
//        try{
//            panEndedEvent.notify(this, m_currentPanEvent.value());
//        }catch(...){
//            attended = true;
//        }
//        
//        m_currentPanEvent.clear();
//    }
//    
//    //check if should start panning
//    if (m_currentPanEvent.isNull() && m_touches.size() == 1 && touchExists(0))
//    {
//        PanEventArgs newPanEventArgs;
//        
//        newPanEventArgs.origin = m_touches[0].origin;
//        newPanEventArgs.current = m_touches[0].current;
//        
//        m_currentPanEvent.assign(newPanEventArgs);
//
//        try{
//            panEvent.notify(this, m_currentPanEvent.value());
//        }catch(...){
//            attended = true;
//        }
//    }
//    
//    //check if should start pinching
//    if (m_currentPinchEvent.isNull() && touchExists(0) && touchExists(1))
//    {
//        PinchEventArgs newPinchEventArgs;
//        
//        newPinchEventArgs.origin1 = m_touches[0].origin;
//        newPinchEventArgs.origin2 = m_touches[1].origin;
//        newPinchEventArgs.current1 = m_touches[0].current;
//        newPinchEventArgs.current2 = m_touches[1].current;
//        newPinchEventArgs.previous1 = m_touches[0].origin;
//        newPinchEventArgs.previous2 = m_touches[1].origin;
//        
//        m_currentPinchEvent.assign(newPinchEventArgs);
//
//        try{
//        	pinchEvent.notify(this, m_currentPinchEvent.value());
//        }catch(...){
//            attended = true;
//        }
//    }

    return attended;
};


bool ofxGestures::touchMoved(ofTouchEventArgs & touch) {
    m_touches[touch.id].setCurrent(touch);
    bool attended = m_state->touchMoved(touch);
    
//    if (!m_currentPanEvent.isNull() && touch.id == 0)
//    {
//        m_currentPanEvent.value().current = touch;
//
//        try{
//            panEvent.notify(this, m_currentPanEvent.value());
//        }catch(...){
//            attended = true;
//        }
//    }
//    
//    bool notifyPinch = false;
//    
//    if (!m_currentPinchEvent.isNull() && touch.id == 0)
//    {
//        m_currentPinchEvent.value().current1 = touch;
//        
//        notifyPinch = true;
//    }
//    
//    if (!m_currentPinchEvent.isNull() && touch.id == 1)
//    {
//        m_currentPinchEvent.value().current2 = touch;
//        
//        notifyPinch = true;
//    }
//    
//    if (notifyPinch) {
//        
//        if (!m_currentPinchEvent.value().isExtended) {
//            
//            //TALTODO:
//            
//            //if angle > tolerance, set extended true
//            
//            //else if delta > tolerance, set extended true
//            
//            m_currentPinchEvent.value().isExtended = true;
//        }
//        
//        try{
//            pinchEvent.notify(this, m_currentPinchEvent.value());
//        }catch(...){
//            attended = true;
//        }
//        
//        m_currentPinchEvent.value().previous1 = m_currentPinchEvent.value().current1;
//        m_currentPinchEvent.value().previous2 = m_currentPinchEvent.value().current2;
//    }
    
    return attended;
};

bool ofxGestures::touchUp(ofTouchEventArgs &touch) {
    m_touches.erase(touch.id);
    bool attended = m_state->touchUp(touch);//false;
    
//    if (!m_currentPanEvent.isNull())
//    {
//        if (!touchExists(0))   
//        {
//            try{
//            	panEndedEvent.notify(this, m_currentPanEvent);
//            }catch(...){
//                attended = true;
//            }
//            
//            m_currentPanEvent.clear();
//        }
//    }
//    
//    if (!m_currentPinchEvent.isNull())
//    {
//        if (!touchExists(0) || !touchExists(1))
//        {
//            try{
//            	pinchEndedEvent.notify(this, m_currentPinchEvent);
//            }catch(...){
//                attended = true;
//            }
//            
//            m_currentPinchEvent.clear();
//        }
//    }
    return attended;
};

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
    m_firstTouch.setCurrent(touch);
}

bool ofxGestures::PinchEventArgs::isExtended()const{
    return m_extended;
}

void ofxGestures::PinchEventArgs::enabledExtended(){
    m_extended = true;
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
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double originLength = sqrt(pow(originDelta.x, 2) + pow(originDelta.y, 2));
    
    return currentLength / originLength;
}

double ofxGestures::PinchEventArgs::getRelativeScale() const {
    
    ofVec2f currentDelta = m_secondTouch.m_current - m_firstTouch.m_current;
    ofVec2f previousDelta = m_secondTouch.m_previous - m_firstTouch.m_previous;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double previousLength = sqrt(pow(previousDelta.x, 2) + pow(previousDelta.y, 2));
    
    return currentLength / previousLength;
}

ofVec2f ofxGestures::PinchEventArgs::getDelta() const {
    
    if (!m_extended)
        return ofVec2f(0,0);
    
    return getInnerDelta();
}

ofVec2f ofxGestures::PinchEventArgs::getRelativeDelta() const {
    
    if (!m_extended)
        return ofVec2f(0,0);
    
    return (m_secondTouch.m_current + m_firstTouch.m_current - m_secondTouch.m_previous - m_firstTouch.m_previous) / 2.0;
}

double ofxGestures::PinchEventArgs::getAngle() const {
 
    if (!m_extended)
        return 0.0;
    return getInnerAngle();
}

double ofxGestures::PinchEventArgs::getRelativeAngle() const {
    if (!m_extended)
        return 0.0;
    return getAngle(m_firstTouch.m_previous, m_secondTouch.m_previous);
}

ofVec2f ofxGestures::PinchEventArgs::getInnerDelta()const{
    return (m_secondTouch.m_current + m_firstTouch.m_current - m_secondTouch.m_origin - m_firstTouch.m_origin) / 2.0;
}

double ofxGestures::PinchEventArgs::getInnerAngle()const{
    return getAngle(m_firstTouch.m_origin, m_secondTouch.m_origin);
}

double ofxGestures::PinchEventArgs::getAngle(const ofTouchEventArgs &first, const ofTouchEventArgs &second)const{
    ofVec2f currentDelta = m_secondTouch.m_current - m_firstTouch.m_current;
    ofVec2f originDelta = second - first;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double originLength = sqrt(pow(originDelta.x, 2) + pow(originDelta.y, 2));
    
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



