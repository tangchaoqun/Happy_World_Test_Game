#include "InventoryPanel.h"
#include "../Player/Player.h"

USING_NS_CC;

InventoryPanel* InventoryPanel::create() {
    auto ret = new InventoryPanel();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool InventoryPanel::init() {
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
        panel->setContentSize(Size(480, 400));
    }
    panel->setContentSize(Size(480, 400));
    panel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    panel->setColor(Color3B(30, 35, 50));
    this->addChild(panel);

    auto panelCenter = Vec2(visibleSize.width / 2, visibleSize.height / 2);

    // Title
    auto title = Label::createWithSystemFont("背包", "Arial", 22);
    title->setPosition(Vec2(panelCenter.x, panelCenter.y + 170));
    title->setTextColor(Color4B::WHITE);
    this->addChild(title);

    // Item grid area
    m_itemGrid = Node::create();
    m_itemGrid->setPosition(Vec2(panelCenter.x - 200, panelCenter.y - 20));
    this->addChild(m_itemGrid);

    // Info label
    m_infoLabel = Label::createWithSystemFont("点击物品查看详情", "Arial", 13);
    m_infoLabel->setPosition(Vec2(panelCenter.x, panelCenter.y - 150));
    m_infoLabel->setTextColor(Color4B(150, 150, 180, 255));
    this->addChild(m_infoLabel);

    // Close button
    m_closeBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    m_closeBtn->setTitleText("关闭 (B)");
    m_closeBtn->setTitleFontSize(16);
    m_closeBtn->setPosition(Vec2(panelCenter.x + 180, panelCenter.y + 160));
    m_closeBtn->addClickEventListener(CC_CALLBACK_1(InventoryPanel::onClose, this));
    this->addChild(m_closeBtn);

    return true;
}

void InventoryPanel::refresh() {
    m_itemGrid->removeAllChildren();

    auto player = Player::getInstance();
    const auto& inventory = player->getInventory();
    auto visibleSize = Director::getInstance()->getVisibleSize();

    int cols = 5;
    int cellSize = 64;
    int padding = 6;

    for (size_t i = 0; i < inventory.size() && i < 20; ++i) {
        int row = i / cols;
        int col = i % cols;

        // Item slot background
        auto slot = ui::Button::create();
        slot->setContentSize(Size(cellSize, cellSize));
        slot->setPosition(Vec2(col * (cellSize + padding), -row * (cellSize + padding)));
        slot->setColor(Color3B(50, 55, 70));
        slot->addClickEventListener([this, itemId = inventory[i].first.id](Ref*) {
            onItemClick(itemId);
        });

        // Item name label (placeholder icon)
        auto itemLabel = Label::createWithSystemFont(inventory[i].first.name.substr(0, 2), "Arial", 14);
        itemLabel->setPosition(Vec2(cellSize / 2, cellSize / 2 + 8));
        itemLabel->setTextColor(Color4B::WHITE);
        slot->addChild(itemLabel);

        // Stack count
        if (inventory[i].second > 1) {
            auto countLabel = Label::createWithSystemFont(
                std::to_string(inventory[i].second), "Arial", 10);
            countLabel->setAnchorPoint(Vec2(1, 0));
            countLabel->setPosition(Vec2(cellSize - 2, 2));
            countLabel->setTextColor(Color4B(200, 200, 200, 255));
            slot->addChild(countLabel);
        }

        m_itemGrid->addChild(slot);
    }

    if (inventory.empty()) {
        m_infoLabel->setString("背包是空的");
    } else {
        char buf[64];
        snprintf(buf, sizeof(buf), "共 %zu 件物品 (空格 %zu/%d)",
            inventory.size(), 20 - inventory.size(), 20);
        m_infoLabel->setString(buf);
    }
}

void InventoryPanel::onItemClick(int itemId) {
    auto player = Player::getInstance();
    auto count = player->getItemCount(itemId);
    
    char buf[128];
    snprintf(buf, sizeof(buf), "物品ID: %d | 数量: %d", itemId, count);
    m_infoLabel->setString(buf);
}

void InventoryPanel::onClose(Ref* sender) {
    this->setVisible(false);
}
