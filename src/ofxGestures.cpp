//
//  ofxGestures.cpp
//
//  Created by Tal Lavi on 10/21/14.
//
//

#include "ofxGestures.h"

ofxGestures & ofxGestures::get(){
	static ofxGestures * instance = new ofxGestures();
	return *instance;
}

ofxGestures::ofxGestures():
m_isPanning(false),
m_isPinching(false)
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
    if (m_isPanning && m_touches.size() > 1)
    {
        try{
        	panGestureEndedEvent.notify(this,pan);
        }catch(...){
            attended = true;
        }
        
        m_isPanning = false;
    }
    
    //check if should start panning
    if (!m_isPanning && m_touches.size() == 1 && touchExists(0))
    {
        m_isPanning = true;
        
        m_panOrigin = m_touches[0];
        m_panCurrent = m_touches[0];

        try{
        	panGestureEvent.notify(this,pan);
        }catch(...){
            attended = true;
        }
    }
    
    //check if should start pinching
    if (!m_isPinching && touchExists(0) && touchExists(1))
    {
        m_isPinching = true;
        
        m_pinchOrigin1 = m_touches[0];
        m_pinchOrigin2 = m_touches[1];
        m_pinchCurrent1 = m_touches[0];
        m_pinchCurrent2 = m_touches[1];
        m_pinchPrevious1 = m_touches[0];
        m_pinchPrevious2 = m_touches[1];

        try{
        	pinchGestureEvent.notify(this,pinch);
        }catch(...){
            attended = true;
        }
    }

    return attended;
};

bool ofxGestures::touchMoved(ofTouchEventArgs & touch) {
    m_touches[touch.id] = touch;
    bool attended = false;
    
    if (m_isPanning && touch.id == 0)
    {
        m_panCurrent = touch;

        try{
        	panGestureEvent.notify(this,pan);
        }catch(...){
            attended = true;
        }
    }
    
    if (m_isPinching && touch.id == 0)
    {
        m_pinchCurrent1 = touch;

        try{
        	pinchGestureEvent.notify(this,pinch);
        }catch(...){
            attended = true;
        }

        m_pinchPrevious1 = m_pinchCurrent1;
    }
    
    if (m_isPinching && touch.id == 1)
    {
        m_pinchCurrent2 = touch;

        try{
        	pinchGestureEvent.notify(this,pinch);
        }catch(...){
            attended = true;
        }

        m_pinchPrevious2 = m_pinchCurrent2;
    }
    return attended;
};

bool ofxGestures::touchUp(ofTouchEventArgs & touch) {
    m_touches.erase(touch.id);
    bool attended = false;
    
    if (m_isPanning)
    {
        if (!touchExists(0))   
        {
            try{
            	panGestureEndedEvent.notify(this,pan);
            }catch(...){
                attended = true;
            }
            m_isPanning = false;
        }
    }
    
    if (m_isPinching)
    {
        if (!touchExists(0) || !touchExists(1))
        {
            try{
            	pinchGestureEndedEvent.notify(this,pinch);
            }catch(...){
                attended = true;
            }
            m_isPinching = false;
        }
    }
    return attended;
};

ofVec2f ofxGestures::getPanOrigin() const
{
    return m_panOrigin;
}

ofVec2f ofxGestures::getPanDelta() const
{
    return m_panCurrent - m_panOrigin;
}

ofVec2f ofxGestures::getPinchOrigin() const
{
    return (m_pinchOrigin2 + m_pinchOrigin1) / 2.0;
}

ofVec2f ofxGestures::getPinchDelta() const
{
    return (m_pinchCurrent2 + m_pinchCurrent1 - m_pinchOrigin2 - m_pinchOrigin1) / 2.0;
}

double ofxGestures::getPinchAngle() const
{
    ofVec2f currentDelta = m_pinchCurrent2 - m_pinchCurrent1;
    ofVec2f originDelta = m_pinchOrigin2 - m_pinchOrigin1;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double originLength = sqrt(pow(originDelta.x, 2) + pow(originDelta.y, 2));
    
    double currentAngle = ofRadToDeg(asin(currentDelta.x / currentLength));
    double originAngle = ofRadToDeg(asin(originDelta.x / originLength));
    
    if (m_pinchCurrent2.y > m_pinchCurrent1.y) currentAngle = 180.0 - currentAngle;
    if ( m_pinchOrigin2.y >  m_pinchOrigin1.y) originAngle = 180.0 - originAngle;
    
    return currentAngle - originAngle;
}

double ofxGestures::getPinchScale() const
{
    ofVec2f currentDelta = m_pinchCurrent2 - m_pinchCurrent1;
    ofVec2f originDelta = m_pinchOrigin2 - m_pinchOrigin1;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double originLength = sqrt(pow(originDelta.x, 2) + pow(originDelta.y, 2));
    
    return currentLength / originLength;
}


ofVec2f ofxGestures::getPinchPrevious() const{
    return (m_pinchPrevious2 + m_pinchPrevious1) / 2.0;
}

ofVec2f ofxGestures::getPinchRelativeDelta() const{
    return (m_pinchCurrent2 + m_pinchCurrent1 - m_pinchPrevious2 - m_pinchPrevious1) / 2.0;
}

double ofxGestures::getPinchRelativeAngle() const{
    ofVec2f currentDelta = m_pinchCurrent2 - m_pinchCurrent1;
    ofVec2f previousDelta = m_pinchPrevious2 - m_pinchPrevious1;

    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double previousLength = sqrt(pow(previousDelta.x, 2) + pow(previousDelta.y, 2));

    double currentAngle = ofRadToDeg(asin(currentDelta.x / currentLength));
    double previousAngle = ofRadToDeg(asin(previousDelta.x / previousLength));

    if (m_pinchCurrent2.y > m_pinchCurrent1.y) currentAngle = 180.0 - currentAngle;
    if (m_pinchPrevious2.y >  m_pinchPrevious1.y) previousAngle = 180.0 - previousAngle;

    return currentAngle - previousAngle;
}

double ofxGestures::getPinchRelativeScale() const{
    ofVec2f currentDelta = m_pinchCurrent2 - m_pinchCurrent1;
    ofVec2f previousDelta = m_pinchPrevious2 - m_pinchPrevious1;

    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double previousLength = sqrt(pow(previousDelta.x, 2) + pow(previousDelta.y, 2));

    return currentLength / previousLength;
}

ofVec2f ofxGestures::PinchEvent::getOrigin() const{
	return ofxGestures::get().getPinchOrigin();
}

ofVec3f ofxGestures::PinchEvent::getPrevious() const{
	return ofxGestures::get().getPinchPrevious();
}

ofVec2f ofxGestures::PinchEvent::getDelta() const{
	return ofxGestures::get().getPinchDelta();
}

ofVec2f ofxGestures::PinchEvent::getRelativeDelta() const{
	return ofxGestures::get().getPinchRelativeDelta();
}

double ofxGestures::PinchEvent::getAngle() const{
	return ofxGestures::get().getPinchAngle();
}

double ofxGestures::PinchEvent::getScale() const{
	return ofxGestures::get().getPinchScale();
}

double ofxGestures::PinchEvent::getRelativeAngle() const{
	return ofxGestures::get().getPinchRelativeAngle();
}

double ofxGestures::PinchEvent::getRelativeScale() const{
	return ofxGestures::get().getPinchRelativeScale();
}

ofVec2f ofxGestures::PanEvent::getOrigin() const{
	return ofxGestures::get().getPanOrigin();
}

ofVec2f ofxGestures::PanEvent::getDelta() const{
	return ofxGestures::get().getPanDelta();
}
