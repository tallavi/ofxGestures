//
//  ofxGestures.cpp
//
//  Created by Tal Lavi on 10/21/14.
//
//

#include "ofxGestures.h"
#include "MathUtils.h"

ofxGestures::ofxGestures():
m_isPanning(false),
m_isPinching(false)
{
    ofAddListener(ofEvents().touchDown, this, &ofxGestures::touchDown);
	ofAddListener(ofEvents().touchMoved, this, &ofxGestures::touchMoved);
    ofAddListener(ofEvents().touchUp, this, &ofxGestures::touchUp);
}

ofxGestures::~ofxGestures()
{
    ofRemoveListener(ofEvents().touchDown, this, &ofxGestures::touchDown);
	ofRemoveListener(ofEvents().touchMoved, this, &ofxGestures::touchMoved);
    ofRemoveListener(ofEvents().touchUp, this, &ofxGestures::touchUp);
}

void ofxGestures::touchDown(ofTouchEventArgs & touch) {
    m_touches[touch.id] = touch;
    
    //check if should stop panning
    if (m_isPanning && m_touches.size() > 1)
    {
        ofEventArgs args;
        ofNotifyEvent(panGestureEndedEvent, args);
        
        m_isPanning = false;
    }
    
    //check if should start panning
    if (!m_isPanning && m_touches.size() == 1 && touchExists(0))
    {
        m_isPanning = true;
        
        m_panOrigin = m_touches[0];
        m_panCurrent = m_touches[0];
        
        ofEventArgs args;
        ofNotifyEvent(panGestureEvent, args);
    }
    
    //check if should start pinching
    if (!m_isPinching && touchExists(0) && touchExists(1))
    {
        m_isPinching = true;
        
        m_pinchOrigin1 = m_touches[0];
        m_pinchOrigin2 = m_touches[1];
        m_pinchCurrent1 = m_touches[0];
        m_pinchCurrent2 = m_touches[1];
        
        ofEventArgs args;
        ofNotifyEvent(pinchGestureEvent, args);
    }
};

void ofxGestures::touchMoved(ofTouchEventArgs & touch) {
    m_touches[touch.id] = touch;
    
    if (m_isPanning && touch.id == 0)
    {
        m_panCurrent = touch;
        
        ofEventArgs args;
        ofNotifyEvent(panGestureEvent, args);
    }
    
    if (m_isPinching && touch.id == 0)
    {
        m_pinchCurrent1 = touch;
        
        ofEventArgs args;
        ofNotifyEvent(pinchGestureEvent, args);
    }
    
    if (m_isPinching && touch.id == 1)
    {
        m_pinchCurrent2 = touch;
        
        ofEventArgs args;
        ofNotifyEvent(pinchGestureEvent, args);
    }
};

void ofxGestures::touchUp(ofTouchEventArgs & touch) {
    m_touches.erase(touch.id);
    
    if (m_isPanning)
    {
        if (!touchExists(0))   
        {
            ofEventArgs args;
            
            ofNotifyEvent(panGestureEndedEvent, args);
            
            m_isPanning = false;
        }
    }
    
    if (m_isPinching)
    {
        if (!touchExists(0) || !touchExists(1))
        {
            ofEventArgs args;
            
            ofNotifyEvent(pinchGestureEndedEvent, args);
            
            m_isPinching = false;
        }
    }
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
    
    double currentAngle = rad2deg(asin(currentDelta.x / currentLength));
    double originAngle = rad2deg(asin(originDelta.x / originLength));
    
    if (m_pinchCurrent2.y > m_pinchCurrent1.y) currentAngle = 180.0 - currentAngle;
    if ( m_pinchOrigin2.y >  m_pinchOrigin1.y)  originAngle = 180.0 - originAngle;
    
    return currentAngle - originAngle;
}

double ofxGestures::getPinchScale() const
{
    ofVec2f currentDelta = m_pinchCurrent2 - m_pinchCurrent1;
    ofVec2f originDelta = m_pinchOrigin2 - m_pinchOrigin1;
    
    double currentLength = sqrt(pow(currentDelta.x, 2) + pow(currentDelta.y, 2));
    double originLength = sqrt(pow(originDelta.x, 2) + pow(originDelta.y, 2));
    
    return originLength / currentLength;
}