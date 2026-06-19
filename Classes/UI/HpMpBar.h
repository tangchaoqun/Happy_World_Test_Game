#ifndef __HP_MP_BAR_H__
#define __HP_MP_BAR_H__

#include "cocos2d.h"

class HpMpBar : public cocos2d::Node {
public:
    static HpMpBar* create(float width, float height, const cocos2d::Color3B& color);
    virtual bool init(float width, float height, const cocos2d::Color3B& color);
    
    void setHpRatio(float ratio);
    void setMpRatio(float ratio);

private:
    cocos2d::Sprite* m_hpBg = nullptr;
    cocos2d::Sprite* m_hpFill = nullptr;
    cocos2d::Sprite* m_mpBg = nullptr;
    cocos2d::Sprite* m_mpFill = nullptr;
    cocos2d::Label* m_hpLabel = nullptr;
    cocos2d::Label* m_mpLabel = nullptr;
    float m_width = 0;
};

#endif // __HP_MP_BAR_H__
