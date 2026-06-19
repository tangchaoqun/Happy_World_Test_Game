#include "QuestPanel.h"

USING_NS_CC;

QuestPanel* QuestPanel::create() {
    auto ret = new QuestPanel();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool QuestPanel::init() {
    if (!Node::init()) return false;

    buildUI();
    this->setVisible(false);
    m_isVisible = false;

    return true;
}

void QuestPanel::buildUI() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 背景
    m_bgPanel = ui::Scale9Sprite::create("panel_bg.png");
    if (!m_bgPanel) {
        m_bgPanel = ui::Scale9Sprite::create();
    }
    m_bgPanel->setContentSize(Size(600, 500));
    m_bgPanel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    m_bgPanel->setColor(Color3B(25, 28, 40));
    m_bgPanel->setOpacity(240);
    this->addChild(m_bgPanel);

    // 标题
    m_titleLabel = Label::createWithSystemFont("📜 任务日志", "Arial", 22, Size(560, 0), TextHAlignment::CENTER);
    m_titleLabel->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 220));
    m_titleLabel->setTextColor(Color4B(255, 200, 100, 255));
    this->addChild(m_titleLabel, 1);

    // 标签页按钮
    float tabX = visibleSize.width / 2 - 180;
    m_tabMain = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    m_tabMain->setTitleText("主线");
    m_tabMain->setTitleFontSize(13);
    m_tabMain->setPosition(Vec2(tabX, visibleSize.height / 2 + 180));
    m_tabMain->addClickEventListener([this](Ref*) { m_currentTab = 1; refreshQuestList(); });
    this->addChild(m_tabMain, 1);

    m_tabSide = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    m_tabSide->setTitleText("支线");
    m_tabSide->setTitleFontSize(13);
    m_tabSide->setPosition(Vec2(tabX + 90, visibleSize.height / 2 + 180));
    m_tabSide->addClickEventListener([this](Ref*) { m_currentTab = 2; refreshQuestList(); });
    this->addChild(m_tabSide, 1);

    m_tabActive = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    m_tabActive->setTitleText("进行中");
    m_tabActive->setTitleFontSize(13);
    m_tabActive->setPosition(Vec2(tabX + 180, visibleSize.height / 2 + 180));
    m_tabActive->addClickEventListener([this](Ref*) { m_currentTab = 3; refreshQuestList(); });
    this->addChild(m_tabActive, 1);

    auto tabAll = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    tabAll->setTitleText("全部");
    tabAll->setTitleFontSize(13);
    tabAll->setPosition(Vec2(tabX - 90, visibleSize.height / 2 + 180));
    tabAll->addClickEventListener([this](Ref*) { m_currentTab = 0; refreshQuestList(); });
    this->addChild(tabAll, 1);

    // 任务列表区域
    m_questList = Node::create();
    m_questList->setPosition(Vec2(visibleSize.width / 2 - 140, visibleSize.height / 2 + 130));
    this->addChild(m_questList, 1);

    // 详情区域
    m_questDetail = Node::create();
    m_questDetail->setPosition(Vec2(visibleSize.width / 2 + 120, visibleSize.height / 2 + 100));
    this->addChild(m_questDetail, 1);

    m_detailTitle = Label::createWithSystemFont("", "Arial", 16, Size(250, 0), TextHAlignment::LEFT);
    m_detailTitle->setAnchorPoint(Vec2(0, 1));
    m_detailTitle->setPosition(Vec2(0, 180));
    m_detailTitle->setTextColor(Color4B(255, 200, 100, 255));
    m_questDetail->addChild(m_detailTitle);

    m_detailDesc = Label::createWithSystemFont("", "Arial", 13, Size(250, 0), TextHAlignment::LEFT);
    m_detailDesc->setAnchorPoint(Vec2(0, 1));
    m_detailDesc->setPosition(Vec2(0, 140));
    m_detailDesc->setTextColor(Color4B(220, 220, 220, 255));
    m_questDetail->addChild(m_detailDesc);

    m_detailProgress = Label::createWithSystemFont("", "Arial", 12, Size(250, 0), TextHAlignment::LEFT);
    m_detailProgress->setAnchorPoint(Vec2(0, 1));
    m_detailProgress->setPosition(Vec2(0, 60));
    m_detailProgress->setTextColor(Color4B(100, 255, 150, 255));
    m_questDetail->addChild(m_detailProgress);

    m_detailReward = Label::createWithSystemFont("", "Arial", 12, Size(250, 0), TextHAlignment::LEFT);
    m_detailReward->setAnchorPoint(Vec2(0, 1));
    m_detailReward->setPosition(Vec2(0, -10));
    m_detailReward->setTextColor(Color4B(255, 220, 100, 255));
    m_questDetail->addChild(m_detailReward);

    // 按钮
    m_acceptBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    m_acceptBtn->setTitleText("接受任务");
    m_acceptBtn->setTitleFontSize(14);
    m_acceptBtn->setPosition(Vec2(80, -60));
    m_acceptBtn->setVisible(false);
    m_acceptBtn->addClickEventListener([this](Ref*) {
        if (m_selectedQuestId > 0) {
            StoryManager::getInstance()->startQuest(m_selectedQuestId);
            refreshQuestList();
        }
    });
    m_questDetail->addChild(m_acceptBtn, 1);

    m_abandonBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    m_abandonBtn->setTitleText("放弃任务");
    m_abandonBtn->setTitleFontSize(14);
    m_abandonBtn->setPosition(Vec2(80, -60));
    m_abandonBtn->setVisible(false);
    m_abandonBtn->setColor(Color3B(180, 60, 60));
    m_abandonBtn->addClickEventListener([this](Ref*) {
        if (m_selectedQuestId > 0) {
            StoryManager::getInstance()->abandonQuest(m_selectedQuestId);
            refreshQuestList();
        }
    });
    m_questDetail->addChild(m_abandonBtn, 1);

    m_completeBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    m_completeBtn->setTitleText("完成任务");
    m_completeBtn->setTitleFontSize(14);
    m_completeBtn->setPosition(Vec2(80, -60));
    m_completeBtn->setVisible(false);
    m_completeBtn->setColor(Color3B(60, 180, 60));
    m_completeBtn->addClickEventListener([this](Ref*) {
        if (m_selectedQuestId > 0) {
            StoryManager::getInstance()->finishQuest(m_selectedQuestId);
            refreshQuestList();
        }
    });
    m_questDetail->addChild(m_completeBtn, 1);

    // 关闭按钮
    auto closeBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    closeBtn->setTitleText("X");
    closeBtn->setTitleFontSize(16);
    closeBtn->setPosition(Vec2(visibleSize.width / 2 + 270, visibleSize.height / 2 + 220));
    closeBtn->addClickEventListener([this](Ref*) { hide(); });
    this->addChild(closeBtn, 1);

    refreshQuestList();
}

void QuestPanel::refreshQuestList() {
    m_questList->removeAllChildren();

    auto* storyMgr = StoryManager::getInstance();
    std::vector<const Quest*> questsToShow;

    // 收集要显示的任务
    switch (m_currentTab) {
        case 1: // 主线
            for (const auto& q : storyMgr->getAvailableQuests()) {
                if (q.type == QuestType::Main) questsToShow.push_back(&q);
            }
            for (const auto& q : storyMgr->getActiveQuests()) {
                if (q.type == QuestType::Main) questsToShow.push_back(&q);
            }
            break;
        case 2: // 支线
            for (const auto& q : storyMgr->getAvailableQuests()) {
                if (q.type == QuestType::Side || q.type == QuestType::Daily) questsToShow.push_back(&q);
            }
            for (const auto& q : storyMgr->getActiveQuests()) {
                if (q.type == QuestType::Side || q.type == QuestType::Daily) questsToShow.push_back(&q);
            }
            break;
        case 3: // 进行中
            for (const auto& q : storyMgr->getActiveQuests()) {
                questsToShow.push_back(&q);
            }
            break;
        default: // 全部
            for (const auto& q : storyMgr->getAvailableQuests()) {
                questsToShow.push_back(&q);
            }
            for (const auto& q : storyMgr->getActiveQuests()) {
                questsToShow.push_back(&q);
            }
            break;
    }

    // 绘制列表
    int y = 0;
    for (size_t i = 0; i < questsToShow.size(); ++i) {
        createQuestListItem(*questsToShow[i], m_questList, y);
        y -= 50;
    }

    if (questsToShow.empty()) {
        auto emptyLabel = Label::createWithSystemFont("(暂无任务)", "Arial", 14);
        emptyLabel->setPosition(Vec2(0, 0));
        emptyLabel->setTextColor(Color4B(150, 150, 150, 255));
        m_questList->addChild(emptyLabel);
    }

    // 更新按钮颜色
    m_tabMain->setColor(m_currentTab == 1 ? Color3B(80, 100, 160) : Color3B(50, 55, 80));
    m_tabSide->setColor(m_currentTab == 2 ? Color3B(80, 100, 160) : Color3B(50, 55, 80));
    m_tabActive->setColor(m_currentTab == 3 ? Color3B(80, 100, 160) : Color3B(50, 55, 80));
}

void QuestPanel::createQuestListItem(const Quest& quest, Node* container, int yPos) {
    auto item = Node::create();
    item->setPosition(Vec2(0, yPos));

    // 背景条
    auto bg = Sprite::create();
    bg->setTextureRect(Rect(0, 0, 260, 42));
    bg->setColor(quest.id == m_selectedQuestId ? Color3B(60, 70, 100) : Color3B(40, 45, 60));
    bg->setOpacity(180);
    item->addChild(bg);

    // 类型标记
    std::string typeMark = "◆";
    Color4B typeColor = Color4B(255, 200, 50, 255);
    if (quest.type == QuestType::Side) { typeMark = "◇"; typeColor = Color4B(100, 200, 255, 255); }
    else if (quest.type == QuestType::Daily) { typeMark = "◈"; typeColor = Color4B(100, 255, 150, 255); }

    auto typeLabel = Label::createWithSystemFont(typeMark, "Arial", 14);
    typeLabel->setPosition(Vec2(-115, 0));
    typeLabel->setTextColor(typeColor);
    item->addChild(typeLabel);

    // 标题
    auto titleLabel = Label::createWithSystemFont(quest.title, "Arial", 13, Size(200, 0), TextHAlignment::LEFT);
    titleLabel->setPosition(Vec2(-15, 5));
    titleLabel->setTextColor(Color4B::WHITE);
    titleLabel->setAnchorPoint(Vec2(0, 0.5f));
    item->addChild(titleLabel);

    // 状态
    std::string statusText = "可接";
    Color4B statusColor = Color4B(100, 255, 100, 255);
    if (quest.state == QuestState::Active) { statusText = "进行中"; statusColor = Color4B(255, 200, 50, 255); }
    else if (quest.state == QuestState::Completed) { statusText = "可交付"; statusColor = Color4B(50, 200, 255, 255); }
    else if (quest.state == QuestState::Finished) { statusText = "已完成"; statusColor = Color4B(150, 150, 150, 255); }

    auto statusLabel = Label::createWithSystemFont(statusText, "Arial", 11);
    statusLabel->setPosition(Vec2(-15, -12));
    statusLabel->setTextColor(statusColor);
    statusLabel->setAnchorPoint(Vec2(0, 0.5f));
    item->addChild(statusLabel);

    // 等级要求
    char levelBuf[32];
    snprintf(levelBuf, sizeof(levelBuf), "Lv.%d", quest.requiredLevel);
    auto levelLabel = Label::createWithSystemFont(levelBuf, "Arial", 10);
    levelLabel->setPosition(Vec2(110, 0));
    levelLabel->setTextColor(Color4B(150, 150, 150, 255));
    item->addChild(levelLabel);

    // 点击事件
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(false);
    touchListener->onTouchBegan = [this, questId = quest.id](Touch* touch, Event*) {
        m_selectedQuestId = questId;
        auto* q = StoryManager::getInstance()->getQuest(questId);
        if (q) {
            showQuestDetails(q);
        }
        refreshQuestList();
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, bg);

    container->addChild(item);
}

void QuestPanel::showQuestDetails(const Quest* quest) {
    if (!quest) return;

    m_detailTitle->setString(quest->title);
    m_detailDesc->setString(quest->description);

    // 进度
    std::string progressStr = "";
    for (const auto& obj : quest->objectives) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s: %d/%d %s\n", 
            obj.description.c_str(), obj.currentCount, obj.requiredCount,
            obj.completed ? "✓" : "");
        progressStr += buf;
    }
    m_detailProgress->setString(progressStr.empty() ? "" : "进度:\n" + progressStr);

    // 奖励
    char rewardBuf[256];
    snprintf(rewardBuf, sizeof(rewardBuf), "奖励: %d经验 %d金币", quest->reward.exp, quest->reward.gold);
    if (!quest->reward.rewardEquipment.empty()) {
        strcat(rewardBuf, "\n装备: ");
        strcat(rewardBuf, quest->reward.rewardEquipment.c_str());
    }
    if (!quest->reward.items.empty()) {
        strcat(rewardBuf, "\n物品: ");
        for (const auto& item : quest->reward.items) {
            char itemBuf[32];
            snprintf(itemBuf, sizeof(itemBuf), "%dx? ", item.second);
            strcat(rewardBuf, itemBuf);
        }
    }
    m_detailReward->setString(rewardBuf);

    // 按钮显示控制
    m_acceptBtn->setVisible(quest->state == QuestState::NotStarted);
    m_abandonBtn->setVisible(quest->state == QuestState::Active);
    m_completeBtn->setVisible(quest->state == QuestState::Completed);
}

void QuestPanel::show() {
    this->setVisible(true);
    m_isVisible = true;
    m_selectedQuestId = 0;
    refreshQuestList();

    m_detailTitle->setString("选择任务查看详情");
    m_detailDesc->setString("");
    m_detailProgress->setString("");
    m_detailReward->setString("");
    m_acceptBtn->setVisible(false);
    m_abandonBtn->setVisible(false);
    m_completeBtn->setVisible(false);
}

void QuestPanel::hide() {
    this->setVisible(false);
    m_isVisible = false;
}

std::string QuestPanel::getQuestTrackerText() const {
    auto* storyMgr = StoryManager::getInstance();
    const auto& activeQuests = storyMgr->getActiveQuests();

    if (activeQuests.empty()) return "";

    // 只显示第一个进行中任务
    const auto& quest = activeQuests[0];
    std::string text = quest.title + "\n";
    for (const auto& obj : quest.objectives) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s %d/%d\n", 
            obj.description.c_str(), obj.currentCount, obj.requiredCount);
        text += buf;
    }
    return text;
}
