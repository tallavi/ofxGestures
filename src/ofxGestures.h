//
//  ofxGestures.h
//
//  Created by Tal Lavi on 10/21/14.
//
//

#pragma once

#include "ofMain.h"
#include <Poco/Nullable.h>
#include <Poco/Timestamp.h>

class NoneState;
class FirstTouchState;
class PanState;
class PinchState;
class PinchExtendedState;

class ofxGestures
{
public:
    
    friend class NoneState;
    friend class FirstTouchState;
    friend class PanState;
    friend class PinchState;
    friend class PinchExtendedState;
    
    struct Touch{
        ofTouchEventArgs m_origin;
        ofTouchEventArgs m_current;
        ofTouchEventArgs m_previous;
        
        Touch();
        Touch(const ofTouchEventArgs &origin);
        void setCurrent(const ofTouchEventArgs &current);
    };
    
    class PanEventArgs{
    public:
        PanEventArgs(){};
        PanEventArgs(const Touch &touch);
        
        ofVec2f getDelta() const;
        ofVec2f getCurrent()const;
        ofVec2f getOrigin()const;
        
        void setCurrentTouch(const ofTouchEventArgs &touch);
        
    private:
        Touch m_touch;
    };
    
    class PinchEventArgs{
        friend class PinchState;
    public:
        PinchEventArgs(){};
        PinchEventArgs(const Touch &firstTouch, const Touch &secondTouch);
        
        void setFirstTouch(const Touch &touch);
        void setSecondTouch(const Touch &touch);
        
        void setCurrentTouch(const ofTouchEventArgs &touch);
        
        void setFirstCurrentTouch(const ofTouchEventArgs &touch);
        void setSecondCurrentTouch(const ofTouchEventArgs &touch);
        
        ofVec2f getOrigin() const;
        ofVec2f getPrevious() const;
        ofVec2f getCurrent() const;
        
        double getScale() const;
        double getRelativeScale() const;
        
        ofVec2f getDelta() const;
        ofVec2f getRelativeDelta() const;
        double getAngle() const;
        double getRelativeAngle() const;
        
    private:
        double getAngle(const ofTouchEventArgs &first, const ofTouchEventArgs &second) const;
        
        Touch m_firstTouch;
        Touch m_secondTouch;
    };
    
    class BaseState{
    public:
        BaseState();
        ~BaseState();
        
        virtual bool touchDown(ofTouchEventArgs & touch);
        virtual bool touchMoved(ofTouchEventArgs & touch);
        virtual bool touchUp(ofTouchEventArgs & touch);
        
        virtual Poco::Nullable<PanEventArgs> getPanEventArgs();
        virtual Poco::Nullable<PinchEventArgs> getPinchEventArgs();
        
    protected:
        virtual void notifyStart(){};
        
        template<class S>
        void setNextState(){
            ofxGestures::get().m_state.reset(new S());
            ofxGestures::get().m_state->notifyStart();
        }
    };
    
    Poco::Nullable<PanEventArgs> getPanEventArgs(){return m_state->getPanEventArgs();}
    Poco::Nullable<PinchEventArgs> getPinchEventArgs(){return m_state->getPinchEventArgs();}
    
    static ofxGestures & get();

private:
    bool touchDown(ofTouchEventArgs & touch);
    bool touchMoved(ofTouchEventArgs & touch);
    bool touchUp(ofTouchEventArgs & touch);
    
    bool touchExists(int touchNum){return (m_touches.find(touchNum) != m_touches.end());}
    
    bool notifyTapEvent(const ofVec2f &);
    bool notifyLongPressEvent(const ofVec2f &);
    bool notifyPanEvent(const PanEventArgs &);
    bool notifyPanEventEnded(const PanEventArgs &);
    bool notifyPinchEvent(const PinchEventArgs &);
    bool notifyPinchEventEnded(const PinchEventArgs &);

    std::map<int, Touch> m_touches;
    
    std::shared_ptr<BaseState> m_state;

    ofxGestures();

public:
    ~ofxGestures();
    
    ofEvent<const ofVec2f> tapEvent;
    
    ofEvent<const ofVec2f> longPressEvent;
    
    ofEvent<const PanEventArgs> panEvent;
    ofEvent<const PanEventArgs> panEndedEvent;
    
    ofEvent<const PinchEventArgs> pinchEvent;
    ofEvent<const PinchEventArgs> pinchEndedEvent;

};

