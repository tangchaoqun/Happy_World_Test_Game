#include "Joystick.h"

USING_NS_CC;

Joystick* Joystick::create(float radius, const Vec2& defaultPos) {
    auto ret = new Joystick();
    if (ret && ret->init(radius, defaultPos)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Joystick::init(float radius, const Vec2& defaultPos) {
    if (!Node::init()) return false;
    
    m_radius = radius;
    m_direction = Vec2::ZERO;
    
    // Background circle
    m_bg = Sprite::create();
    m_bg->setTextureRect(Rect(0, 0, radius * 2, radius * 2));
    m_bg->setColor(Color3B(60, 60, 80));
    m_bg->setOpacity(100);
    this->addChild(m_bg);
    
    // Thumb
    m_thumb = Sprite::create();
    m_thumb->setTextureRect(Rect(0, 0, radius * 0.4f, radius * 0.4f));
    m_thumb->setColor(Color3B(150, 150, 200));
    m_thumb->setOpacity(180);
    this->addChild(m_thumb);
    
    // Touch listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(Joystick::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Joystick::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Joystick::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

bool Joystick::onTouchBegan(Touch* touch, Event* event) {
    auto touchPos = this->convertToNodeSpace(touch->getLocation());
    if (touchPos.getDistance(Vec2::ZERO) <= m_radius) {
        m_isTouching = true;
        return true;
    }
    return false;
}

void Joystick::onTouchMoved(Touch* touch, Event* event) {
    if (!m_isTouching) return;
    
    auto touchPos = this->convertToNodeSpace(touch->getLocation());
    float dist = touchPos.getDistance(Vec2::ZERO);
    
    if (dist <= m_radius) {
        m_thumb->setPosition(touchPos);
        m_direction = touchPos / m_radius;
    } else {
        Vec2 dir = touchPos.getNormalized();
        m_thumb->setPosition(dir * m_radius);
        m_direction = dir;
    }
}

void Joystick::onTouchEnded(Touch* touch, Event* event) {
    m_isTouching = false;
    m_thumb->setPosition(Vec2::ZERO);
    m_direction = Vec2::ZERO;
}
