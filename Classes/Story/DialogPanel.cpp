#include "DialogPanel.h"

USING_NS_CC;

DialogPanel* DialogPanel::create() {
    auto ret = new DialogPanel();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool DialogPanel::init() {
    if (!Node::init()) return false;

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 黑色遮罩（半透明）
    auto overlay = LayerColor::create(Color4B(0, 0, 0, 160));
    overlay->setContentSize(visibleSize);
    overlay->setPosition(Vec2::ZERO);
    this->addChild(overlay, -1);

    // 对话框背景
    m_bgPanel = ui::Scale9Sprite::create("panel_bg.png");
    if (!m_bgPanel) {
        m_bgPanel = ui::Scale9Sprite::create();
    }
    m_bgPanel->setContentSize(Size(800, 240));
    m_bgPanel->setPosition(Vec2(visibleSize.width / 2, 160));
    m_bgPanel->setColor(Color3B(30, 32, 45));
    m_bgPanel->setOpacity(220);
    this->addChild(m_bgPanel, 0);

    // 说话者头像
    m_speakerIcon = Sprite::create("npc.png");
    if (!m_speakerIcon) {
        m_speakerIcon = Sprite::create();
        m_speakerIcon->setTextureRect(Rect(0, 0, 64, 64));
        m_speakerIcon->setColor(Color3B(180, 180, 200));
    }
    m_speakerIcon->setPosition(Vec2(visibleSize.width / 2 - 340, 220));
    m_speakerIcon->setScale(1.5f);
    this->addChild(m_speakerIcon, 1);

    // 说话者名字
    m_speakerName = Label::createWithSystemFont("???", "Arial", 16, Size(200, 0), TextHAlignment::LEFT);
    m_speakerName->setPosition(Vec2(visibleSize.width / 2 - 260, 240));
    m_speakerName->setTextColor(Color4B(255, 200, 100, 255));
    m_speakerName->setAnchorPoint(Vec2(0, 0.5f));
    this->addChild(m_speakerName, 1);

    // 对话文本
    m_dialogText = Label::createWithSystemFont("", "Arial", 15, Size(700, 0), TextHAlignment::LEFT);
    m_dialogText->setPosition(Vec2(visibleSize.width / 2 - 20, 150));
    m_dialogText->setTextColor(Color4B::WHITE);
    m_dialogText->setAnchorPoint(Vec2(0, 1.0f));
    this->addChild(m_dialogText, 1);

    // 下一句按钮
    m_nextBtn = ui::Button::create("btn_small.png", "btn_small_pressed.png");
    if (!m_nextBtn) {
        m_nextBtn = ui::Button::create();
    }
    m_nextBtn->setTitleText("下一步 ▶");
    m_nextBtn->setTitleFontSize(14);
    m_nextBtn->setPosition(Vec2(visibleSize.width / 2 + 350, 80));
    m_nextBtn->addClickEventListener([this](Ref*) {
        onNextClicked();
    });
    this->addChild(m_nextBtn, 1);

    // 选择按钮容器
    m_choiceContainer = Node::create();
    m_choiceContainer->setPosition(Vec2(visibleSize.width / 2, 50));
    this->addChild(m_choiceContainer, 1);

    // 点击背景也可以下一句
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this](Touch*, Event*) {
        if (!m_typingComplete) {
            // 如果正在打字，直接显示全部
            m_typingComplete = true;
            m_dialogText->setString(m_fullText);
            m_currentCharIndex = static_cast<int>(m_fullText.length());
            return true;
        }
        // 如果文字已显示完，点击下一句
        if (m_choiceButtons.empty()) {
            onNextClicked();
        }
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, overlay);

    this->scheduleUpdate();
    this->setVisible(false);
    m_isShowing = false;

    return true;
}

void DialogPanel::update(float dt) {
    // 打字机效果
    if (!m_typingComplete && !m_fullText.empty()) {
        m_typingTimer += dt;
        if (m_typingTimer >= m_typingSpeed) {
            m_typingTimer = 0;
            if (m_currentCharIndex < static_cast<int>(m_fullText.length())) {
                m_currentCharIndex++;
                m_dialogText->setString(m_fullText.substr(0, m_currentCharIndex));
            } else {
                m_typingComplete = true;
            }
        }
    }
}

void DialogPanel::showDialog(const DialogNode* node) {
    if (!node) return;

    this->setVisible(true);
    m_isShowing = true;

    // 更新头像
    auto texCache = Director::getInstance()->getTextureCache();
    auto tex = texCache->addImage(node->speakerIcon);
    if (tex) {
        m_speakerIcon->setTexture(tex);
    } else {
        m_speakerIcon->setTextureRect(Rect(0, 0, 64, 64));
        m_speakerIcon->setColor(node->speakerIcon == "player.png" ? 
            Color3B(100, 180, 255) : Color3B(180, 180, 200));
    }

    // 更新名字
    m_speakerName->setString(node->speaker);

    // 设置文本（打字机效果）
    m_fullText = node->text;
    m_currentCharIndex = 0;
    m_typingTimer = 0;
    m_typingComplete = false;
    m_dialogText->setString("");

    // 显示/隐藏选择按钮
    clearChoices();
    if (!node->choices.empty()) {
        m_nextBtn->setVisible(false);
        createChoices(node->choices);
    } else {
        m_nextBtn->setVisible(true);
    }

    // 如果是对话结束节点，改变按钮文字
    if (node->isEnd) {
        m_nextBtn->setTitleText("结束对话");
    } else {
        m_nextBtn->setTitleText("下一步 ▶");
    }
}

void DialogPanel::showDialog(const std::string& speaker, const std::string& text, const std::string& icon) {
    DialogNode node;
    node.speaker = speaker;
    node.text = text;
    node.speakerIcon = icon;
    node.isEnd = true;
    showDialog(&node);
}

void DialogPanel::hide() {
    this->setVisible(false);
    m_isShowing = false;
    m_fullText.clear();
    m_currentCharIndex = 0;
    clearChoices();
    if (m_closeCallback) {
        m_closeCallback();
    }
}

void DialogPanel::onNextClicked() {
    if (!m_typingComplete) {
        // 跳过打字动画
        m_typingComplete = true;
        m_dialogText->setString(m_fullText);
        m_currentCharIndex = static_cast<int>(m_fullText.length());
        return;
    }

    if (m_choiceCallback) {
        m_choiceCallback(-1); // -1 表示继续
    }
}

void DialogPanel::onChoiceClicked(int choiceIndex) {
    if (!m_typingComplete) {
        m_typingComplete = true;
        m_dialogText->setString(m_fullText);
        m_currentCharIndex = static_cast<int>(m_fullText.length());
        return;
    }

    if (m_choiceCallback) {
        m_choiceCallback(choiceIndex);
    }
}

void DialogPanel::clearChoices() {
    m_choiceContainer->removeAllChildren();
    m_choiceButtons.clear();
}

void DialogPanel::createChoices(const std::vector<std::pair<int, std::string>>& choices) {
    clearChoices();

    float btnWidth = 200;
    float btnHeight = 40;
    float spacing = 10;
    float startY = (choices.size() - 1) * (btnHeight + spacing) / 2;

    for (size_t i = 0; i < choices.size(); ++i) {
        auto btn = ui::Button::create("btn_normal.png", "btn_pressed.png");
        if (!btn) {
            btn = ui::Button::create();
        }
        btn->setContentSize(Size(btnWidth, btnHeight));
        btn->setTitleText(choices[i].second);
        btn->setTitleFontSize(14);
        btn->setPosition(Vec2(0, startY - i * (btnHeight + spacing)));
        btn->setColor(Color3B(50, 55, 80));
        btn->addClickEventListener([this, i](Ref*) {
            onChoiceClicked(static_cast<int>(i));
        });

        m_choiceContainer->addChild(btn);
        m_choiceButtons.push_back(btn);
    }
}

void DialogPanel::updateDialogText(const std::string& text) {
    m_fullText = text;
    m_currentCharIndex = 0;
    m_typingTimer = 0;
    m_typingComplete = false;
    m_dialogText->setString("");
}
