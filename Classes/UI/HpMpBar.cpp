#include "HpMpBar.h"
#include "../Player/Player.h"

USING_NS_CC;

HpMpBar* HpMpBar::create(float width, float height, const Color3B& color) {
    auto ret = new HpMpBar();
    if (ret && ret->init(width, height, color)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool HpMpBar::init(float width, float height, const Color3B& color) {
    if (!Node::init()) return false;
    
    m_width = width;
    
    // HP Bar background
    m_hpBg = Sprite::create();
    m_hpBg->setTextureRect(Rect(0, 0, width, height));
    m_hpBg->setColor(Color3B(40, 40, 40));
    m_hpBg->setAnchorPoint(Vec2(0, 0.5f));
    this->addChild(m_hpBg);
    
    // HP Bar fill (red)
    m_hpFill = Sprite::create();
    m_hpFill->setTextureRect(Rect(0, 0, width, height));
    m_hpFill->setColor(Color3B(220, 40, 40));
    m_hpFill->setAnchorPoint(Vec2(0, 0.5f));
    m_hpFill->setPosition(Vec2(0, height / 2));
    this->addChild(m_hpFill);
    
    // HP Label
    m_hpLabel = Label::createWithSystemFont("HP: 100/100", "Arial", 11);
    m_hpLabel->setAnchorPoint(Vec2(0, 0.5f));
    m_hpLabel->setPosition(Vec2(width + 8, height / 2));
    m_hpLabel->setTextColor(Color4B::WHITE);
    this->addChild(m_hpLabel);
    
    // MP Bar background (below HP)
    m_mpBg = Sprite::create();
    m_mpBg->setTextureRect(Rect(0, 0, width, height));
    m_mpBg->setColor(Color3B(40, 40, 40));
    m_mpBg->setAnchorPoint(Vec2(0, 0.5f));
    m_mpBg->setPosition(Vec2(0, -height - 2));
    this->addChild(m_mpBg);
    
    // MP Bar fill (blue)
    m_mpFill = Sprite::create();
    m_mpFill->setTextureRect(Rect(0, 0, width, height));
    m_mpFill->setColor(Color3B(40, 100, 220));
    m_mpFill->setAnchorPoint(Vec2(0, 0.5f));
    m_mpFill->setPosition(Vec2(0, -height - 2 + height / 2));
    this->addChild(m_mpFill);
    
    // MP Label
    m_mpLabel = Label::createWithSystemFont("MP: 50/50", "Arial", 11);
    m_mpLabel->setAnchorPoint(Vec2(0, 0.5f));
    m_mpLabel->setPosition(Vec2(width + 8, -height - 2 + height / 2));
    m_mpLabel->setTextColor(Color4B(150, 200, 255, 255));
    this->addChild(m_mpLabel);
    
    return true;
}

void HpMpBar::setHpRatio(float ratio) {
    ratio = std::max(0.0f, std::min(1.0f, ratio));
    m_hpFill->setScaleX(ratio);
    
    // Only update label text when this is the player's HP/MP bar
    // (monster HP bars don't have a Player instance associated)
    auto player = Player::getInstance();
    if (player && m_hpLabel->isVisible()) {
        char buf[64];
        snprintf(buf, sizeof(buf), "HP: %d/%d", player->getHp(), player->getMaxHp());
        m_hpLabel->setString(buf);
    }
}

void HpMpBar::setMpRatio(float ratio) {
    ratio = std::max(0.0f, std::min(1.0f, ratio));
    m_mpFill->setScaleX(ratio);
    
    auto player = Player::getInstance();
    if (player && m_mpLabel->isVisible()) {
        char buf[64];
        snprintf(buf, sizeof(buf), "MP: %d/%d", player->getMp(), player->getMaxMp());
        m_mpLabel->setString(buf);
    }
}
