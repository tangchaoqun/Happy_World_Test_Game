#ifndef __DIALOG_PANEL_H__
#define __DIALOG_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "StoryManager.h"

// 对话面板 - 显示NPC对话和剧情
class DialogPanel : public cocos2d::Node {
public:
    static DialogPanel* create();
    
    bool init() override;
    
    // 显示对话
    void showDialog(const DialogNode* node);
    void showDialog(const std::string& speaker, const std::string& text, const std::string& icon = "npc.png");
    
    // 隐藏/清理
    void hide();
    bool isShowing() const { return m_isShowing; }
    
    // 设置选择回调
    void setChoiceCallback(std::function<void(int)> callback) { m_choiceCallback = callback; }
    void setCloseCallback(std::function<void()> callback) { m_closeCallback = callback; }
    
    // 外部调用
    void onNextClicked();
    void onChoiceClicked(int choiceIndex);
    void update(float dt) override;

private:
    void buildUI();
    void updateDialogText(const std::string& text);
    void clearChoices();
    void createChoices(const std::vector<std::pair<int, std::string>>& choices);
    
    cocos2d::ui::Scale9Sprite* m_bgPanel = nullptr;
    cocos2d::Sprite* m_speakerIcon = nullptr;
    cocos2d::Label* m_speakerName = nullptr;
    cocos2d::Label* m_dialogText = nullptr;
    cocos2d::ui::Button* m_nextBtn = nullptr;
    
    cocos2d::Node* m_choiceContainer = nullptr;
    std::vector<cocos2d::ui::Button*> m_choiceButtons;
    
    bool m_isShowing = false;
    std::function<void(int)> m_choiceCallback = nullptr;
    std::function<void()> m_closeCallback = nullptr;
    
    float m_typingSpeed = 0.03f; // 打字机速度
    std::string m_fullText;
    int m_currentCharIndex = 0;
    float m_typingTimer = 0;
    bool m_typingComplete = false;
};

#endif // __DIALOG_PANEL_H__
