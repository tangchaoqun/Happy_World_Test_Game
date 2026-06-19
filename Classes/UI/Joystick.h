#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "cocos2d.h"

class Joystick : public cocos2d::Node {
public:
    static Joystick* create(float radius, const cocos2d::Vec2& defaultPos);
    virtual bool init(float radius, const cocos2d::Vec2& defaultPos);
    
    cocos2d::Vec2 getDirection() const { return m_direction; }
    bool isTouching() const { return m_isTouching; }

private:
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    cocos2d::Sprite* m_bg = nullptr;
    cocos2d::Sprite* m_thumb = nullptr;
    cocos2d::Vec2 m_direction;
    float m_radius = 0;
    bool m_isTouching = false;
};

#endif // __JOYSTICK_H__
