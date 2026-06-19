#include "PlayerInfoPanel.h"
#include "../Player/Player.h"

USING_NS_CC;

PlayerInfoPanel* PlayerInfoPanel::create() {
    auto ret = new PlayerInfoPanel();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool PlayerInfoPanel::init() {
    if (!Node::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // Semi-transparent overlay
    auto overlay = LayerColor::create(Color4B(0, 0, 0, 120));
    overlay->setContentSize(visibleSize);
    this->addChild(overlay);

    // Panel background
    auto panel = ui::Scale9Sprite::create("panel_bg.png");
    if (!panel) {
        panel = ui::Scale9Sprite::create();
        panel->setContentSize(Size(360, 480));
    }
    panel->setContentSize(Size(360, 480));
    panel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    panel->setColor(Color3B(30, 35, 50));
    this->addChild(panel);

    auto panelCenter = Vec2(visibleSize.width / 2, visibleSize.height / 2);

    // Title
    auto title = Label::createWithSystemFont("角色属性", "Arial", 22);
    title->setPosition(Vec2(panelCenter.x, panelCenter.y + 210));
    title->setTextColor(Color4B::WHITE);
    this->addChild(title);

    // Stats (left column)
    auto createStat = [&](const std::string& label, int yOffset, Color4B color) -> Label* {
        auto lbl = Label::createWithSystemFont(label, "Arial", 15);
        lbl->setAnchorPoint(Vec2(0, 0.5f));
        lbl->setPosition(Vec2(panelCenter.x - 150, panelCenter.y + yOffset));
        lbl->setTextColor(color);
        this->addChild(lbl);
        return lbl;
    };

    int y = 170;
    m_levelLabel = createStat("等级: 1", y, Color4B::WHITE); y -= 28;
    m_expLabel = createStat("经验: 0/100", y, Color4B(180, 220, 255, 255)); y -= 28;
    m_hpLabel = createStat("生命: 100", y, Color4B(255, 100, 100, 255)); y -= 28;
    m_mpLabel = createStat("法力: 50", y, Color4B(100, 180, 255, 255)); y -= 28;
    m_attackLabel = createStat("攻击: 10", y, Color4B(255, 200, 100, 255)); y -= 28;
    m_defenseLabel = createStat("防御: 5", y, Color4B(200, 200, 200, 255)); y -= 28;
    m_strengthLabel = createStat("力量: 5", y, Color4B(255, 150, 100, 255)); y -= 28;
    m_agilityLabel = createStat("敏捷: 5", y, Color4B(100, 255, 150, 255)); y -= 28;
    m_intelligenceLabel = createStat("智力: 5", y, Color4B(150, 150, 255, 255)); y -= 28;
    m_critLabel = createStat("暴击: 5.0%", y, Color4B(255, 100, 200, 255)); y -= 40;

    // Separator
    auto sep = Sprite::create();
    sep->setTextureRect(Rect(0, 0, 300, 1));
    sep->setColor(Color3B(80, 80, 100));
    sep->setPosition(Vec2(panelCenter.x, panelCenter.y - 60));
    this->addChild(sep);

    // Equipment info
    m_equipmentLabel = Label::createWithSystemFont("装备: 无", "Arial", 14);
    m_equipmentLabel->setAnchorPoint(Vec2(0, 0.5f));
    m_equipmentLabel->setPosition(Vec2(panelCenter.x - 150, panelCenter.y - 80));
    m_equipmentLabel->setTextColor(Color4B(180, 180, 200, 255));
    this->addChild(m_equipmentLabel);

    // Close button
    m_closeBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    m_closeBtn->setTitleText("关闭 (C)");
    m_closeBtn->setTitleFontSize(16);
    m_closeBtn->setPosition(Vec2(panelCenter.x, panelCenter.y - 210));
    m_closeBtn->addClickEventListener(CC_CALLBACK_1(PlayerInfoPanel::onClose, this));
    this->addChild(m_closeBtn);

    return true;
}

void PlayerInfoPanel::refresh() {
    auto player = Player::getInstance();

    char buf[128];
    snprintf(buf, sizeof(buf), "等级: %d", player->getLevel());
    m_levelLabel->setString(buf);

    snprintf(buf, sizeof(buf), "经验: %d/%d", player->getExp(), player->getMaxExp());
    m_expLabel->setString(buf);

    snprintf(buf, sizeof(buf), "生命: %d/%d", player->getHp(), player->getMaxHp());
    m_hpLabel->setString(buf);

    snprintf(buf, sizeof(buf), "法力: %d/%d", player->getMp(), player->getMaxMp());
    m_mpLabel->setString(buf);

    snprintf(buf, sizeof(buf), "攻击: %d", player->getAttack());
    m_attackLabel->setString(buf);

    snprintf(buf, sizeof(buf), "防御: %d", player->getDefense());
    m_defenseLabel->setString(buf);

    snprintf(buf, sizeof(buf), "力量: %d", player->getStrength());
    m_strengthLabel->setString(buf);

    snprintf(buf, sizeof(buf), "敏捷: %d", player->getAgility());
    m_agilityLabel->setString(buf);

    snprintf(buf, sizeof(buf), "智力: %d", player->getIntelligence());
    m_intelligenceLabel->setString(buf);

    snprintf(buf, sizeof(buf), "暴击: %.1f%%", player->getCritRate() / 100.0f);
    m_critLabel->setString(buf);

    // Equipment list
    auto equipped = player->getEquippedList();
    if (equipped.empty()) {
        m_equipmentLabel->setString("装备: 无");
    } else {
        std::string equipStr = "装备: ";
        for (size_t i = 0; i < equipped.size(); ++i) {
            if (i > 0) equipStr += ", ";
            equipStr += equipped[i].name;
        }
        m_equipmentLabel->setString(equipStr);
    }
}

void PlayerInfoPanel::onClose(Ref* sender) {
    this->setVisible(false);
}
