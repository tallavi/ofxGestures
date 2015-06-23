//
//  ofxGestures.cpp
//
//  Created by Tal Lavi on 10/21/14.
//
//

#include "ofxGestures.h"

const int PAN_DELTA_TOLLERANCE = 1;
const int PINCH_DELTA_TOLLERANCE = 4;

const double TAP_MAX_TIME = 0.5;
const int PINCH_ANGLE_TOLLERANCE = 30;

ofxGestures & ofxGestures::get(){
	static ofxGestures * instance = new ofxGestures();
	return *instance;
}

ofxGestures::ofxGestures()
{
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

bool ofxGestures::touchDown(ofTouchEventArgs & touch) {
    m_touches[touch.id] = touch;
    bool attended = false;
    
    //check if should stop panning
    if (!m_currentPanEvent.isNull() && m_touches.size() > 1)
    {
        try{
            panGestureEndedEvent.notify(this, m_currentPanEvent.value());
        }catch(...){
            attended = true;
        }
        
        m_currentPanEvent.clear();
    }
    
    //check if should start panning
    if (m_currentPanEvent.isNull() && m_touches.size() == 1 && touchExists(0))
    {
        PanGestureEventArgs newPanGestureEventArgs;
        
        newPanGestureEventArgs.origin = m_touches[0];
        newPanGestureEventArgs.current = m_touches[0];
        
        m_currentPanEvent.assign(newPanGestureEventArgs);

        try{
            panGestureEvent.notify(this, m_currentPanEvent.value());
        }catch(...){
            attended = true;
        }
    }
    
    //check if should start pinching
    if (m_currentPinchEvent.isNull() && touchExists(0) && touchExists(1))
    {
        PinchGestureEventArgs newPinchGestureEventArgs;
        
        newPinchGestureEventArgs.origin1 = m_touches[0];
        newPinchGestureEventArgs.origin2 = m_touches[1];
        newPinchGestureEventArgs.current1 = m_touches[0];
        newPinchGestureEventArgs.current2 = m_touches[1];
        newPinchGestureEventArgs.previous1 = m_touches[0];
        newPinchGestureEventArgs.previous2 = m_touches[1];
        
        m_currentPinchEvent.assign(newPinchGestureEventArgs);

        try{
        	pinchGestureEvent.notify(this, m_currentPinchEvent.value());
        }catch(...){
            attended = true;
        }
    }

    return attended;
};


bool ofxGestures::touchMoved(ofTouchEventArgs & touch) {
    m_touches[touch.id] = touch;
    bool attended = false;
    
    if (!m_currentPanEvent.isNull() && touch.id == 0)
    {
        m_currentPanEvent.value().current = touch;

        try{
            panGestureEvent.notify(this, m_currentPanEvent.value());
        }catch(...){
            attended = true;
        }
    }
    
    bool notifyPinch = false;
    
    if (!m_currentPinchEvent.isNull() && touch.id == 0)
    {
        m_currentPinchEvent.value().current1 = touch;
        
        notifyPinch = true;
    }
    
    if (!m_currentPinchEvent.isNull() && touch.id == 1)
    {
        m_currentPinchEvent.value().current2 = touch;
        
        notifyPinch = true;
    }
    
    if (notifyPinch) {
        
        if (!m_currentPinchEvent.value().isExtended) {
            
            //TALTODO:
            
            //if angle > tolerance, set extended true
            
            //else if delta > tolerance, set extended true
            
            m_currentPinchEvent.value().isExtended = true;
        }
        
        try{
            pinchGestureEvent.notify(this, m_currentPinchEvent.value());
        }catch(...){
            attended = true;
        }
        
        m_currentPinchEvent.value().previous1 = m_currentPinchEvent.value().current1;
        m_currentPinchEvent.value().previous2 = m_currentPinchEvent.value().current2;
    }
        
        
    return attended;
};

bool ofxGestures::touchUp(ofTouchEventArgs & touch) {
    m_touches.erase(touch.id);
    bool attended = false;
    
    if (!m_currentPanEvent.isNull())
    {
        if (!touchExists(0))   
        {
            try{
            	panGestureEndedEvent.notify(this, m_currentPanEvent);
            }catch(...){
                attended = true;
            }
            
            m_currentPanEvent.clear();
        }
    }
    
    if (!m_currentPinchEvent.isNull())
    {
        if (!touchExists(0) || !touchExists(1))
        {
            try{
            	pinchGestureEndedEvent.notify(this, m_currentPinchEvent);
            }catch(...){
                attended = true;
            }
            
            m_currentPinchEvent.clear();
        }
    }
    return attended;
};

ofVec2f ofxGestures::PanGestureEventArgs::getDelta() const {
    
    return current - origin;
}

ofVec2f ofxGestures::PinchGestureEventArgs::getOrigin() const {
    
    return (origin2 + origin1) / 2.0;
}

ofVec2f ofxGestures::PinchGestureEventArgs::getPrevious() const {
    return (previous2 + previous1) / 2.0;
}

ofVec2f ofxGestures::PinchGestureEventArgs::getCurrent() const {
    
    return (current2 + current1) / 2.0;
}

double ofxGestures::PinchGestureEventArgs::getScale() const {
    
    ofVec2f currentDelta = current2 - current1;
    ofVec2f originDelta = origin2 - origin1;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double originLength = sqrt(pow(originDelta.x, 2) + pow(originDelta.y, 2));
    
    return currentLength / originLength;
}

double ofxGestures::PinchGestureEventArgs::getRelativeScale() const {
    
    ofVec2f currentDelta = current2 - current1;
    ofVec2f previousDelta = previous2 - previous1;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double previousLength = sqrt(pow(previousDelta.x, 2) + pow(previousDelta.y, 2));
    
    return currentLength / previousLength;
}

ofVec2f ofxGestures::PinchGestureEventArgs::getDelta() const {
    
    if (!isExtended)
        return ofVec2f(0,0);
    
    return (current2 + current1 - origin2 - origin1) / 2.0;
}

ofVec2f ofxGestures::PinchGestureEventArgs::getRelativeDelta() const {
    
    if (!isExtended)
        return ofVec2f(0,0);
    
    return (current2 + current1 - previous2 - previous1) / 2.0;
}

double ofxGestures::PinchGestureEventArgs::getAngle() const {
 
    if (!isExtended)
        return 0.0;
    
    ofVec2f currentDelta = current2 - current1;
    ofVec2f originDelta = origin2 - origin1;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double originLength = sqrt(pow(originDelta.x, 2) + pow(originDelta.y, 2));
    
    double currentAngle = ofRadToDeg(asin(currentDelta.x / currentLength));
    double originAngle = ofRadToDeg(asin(originDelta.x / originLength));
    
    if (current2.y > current1.y) currentAngle = 180.0 - currentAngle;
    if ( origin2.y >  origin1.y) originAngle = 180.0 - originAngle;
    
    return currentAngle - originAngle;
}

double ofxGestures::PinchGestureEventArgs::getRelativeAngle() const {
    
    ofVec2f currentDelta = current2 - current1;
    ofVec2f previousDelta = previous2 - previous1;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double previousLength = sqrt(pow(previousDelta.x, 2) + pow(previousDelta.y, 2));
    
    double currentAngle = ofRadToDeg(asin(currentDelta.x / currentLength));
    double previousAngle = ofRadToDeg(asin(previousDelta.x / previousLength));
    
    if (current2.y > current1.y) currentAngle = 180.0 - currentAngle;
    if (previous2.y >  previous1.y) previousAngle = 180.0 - previousAngle;
    
    return currentAngle - previousAngle;
}



