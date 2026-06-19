#include "SkillBar.h"
#include "../Player/Player.h"

USING_NS_CC;

SkillBar* SkillBar::create() {
    auto ret = new SkillBar();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool SkillBar::init() {
    if (!Node::init()) return false;

    auto bg = ui::Scale9Sprite::create();
    bg->setContentSize(Size(400, 60));
    bg->setColor(Color3B(20, 22, 35));
    bg->setOpacity(180);
    this->addChild(bg);

    m_skillSlots = Node::create();
    this->addChild(m_skillSlots);

    refresh();

    return true;
}

void SkillBar::refresh() {
    m_skillSlots->removeAllChildren();

    auto player = Player::getInstance();
    const auto& skills = player->getSkills();

    int slotSize = 50;
    int padding = 6;
    int startX = -((int)skills.size() * (slotSize + padding) - padding) / 2;

    for (size_t i = 0; i < skills.size(); ++i) {
        auto slot = ui::Button::create();
        slot->setContentSize(Size(slotSize, slotSize));
        slot->setPosition(Vec2(startX + i * (slotSize + padding), 0));
        slot->setColor(Color3B(50, 55, 75));
        slot->addClickEventListener([this, skillId = skills[i].id](Ref*) {
            onSkillClick(skillId);
        });

        // Skill name
        auto nameLabel = Label::createWithSystemFont(
            skills[i].name.substr(0, 4), "Arial", 11);
        nameLabel->setPosition(Vec2(slotSize / 2, slotSize / 2 + 4));
        nameLabel->setTextColor(Color4B::WHITE);
        slot->addChild(nameLabel);

        // MP cost
        if (skills[i].mpCost > 0) {
            char buf[16];
            snprintf(buf, sizeof(buf), "MP:%d", skills[i].mpCost);
            auto mpLabel = Label::createWithSystemFont(buf, "Arial", 8);
            mpLabel->setAnchorPoint(Vec2(0, 0));
            mpLabel->setPosition(Vec2(2, 2));
            mpLabel->setTextColor(Color4B(100, 180, 255, 200));
            slot->addChild(mpLabel);
        }

        // CD overlay
        if (skills[i].currentCd > 0) {
            auto cdOverlay = LayerColor::create(
                Color4B(0, 0, 0, 150), slotSize, slotSize);
            slot->addChild(cdOverlay);

            auto cdLabel = Label::createWithSystemFont(
                StringUtils::format("%.1f", skills[i].currentCd), "Arial", 16);
            cdLabel->setPosition(Vec2(slotSize / 2, slotSize / 2));
            cdLabel->setTextColor(Color4B::WHITE);
            slot->addChild(cdLabel);
        }

        // Hotkey hint
        auto hotkeyLabel = Label::createWithSystemFont(
            StringUtils::format("%d", (int)i + 1), "Arial", 9);
        hotkeyLabel->setAnchorPoint(Vec2(1, 1));
        hotkeyLabel->setPosition(Vec2(slotSize - 2, slotSize - 2));
        hotkeyLabel->setTextColor(Color4B(150, 150, 180, 200));
        slot->addChild(hotkeyLabel);

        m_skillSlots->addChild(slot);
    }
}

void SkillBar::onSkillClick(int skillId) {
    // 发送自定义事件，由 CityScene 处理技能效果
    EventCustom event("skill_use");
    event.setUserData(reinterpret_cast<void*>(static_cast<intptr_t>(skillId)));
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}
