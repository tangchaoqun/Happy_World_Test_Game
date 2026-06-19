#ifndef __QUEST_PANEL_H__
#define __QUEST_PANEL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "../Story/StoryManager.h"

// 任务面板 - 显示任务列表和详情
class QuestPanel : public cocos2d::Node {
public:
    static QuestPanel* create();
    virtual bool init();

    // 刷新任务列表
    void refreshQuestList();
    
    // 显示/隐藏
    void show();
    void hide();
    bool isVisible() const { return m_isVisible; }

    // 获取任务追踪信息（用于HUD显示）
    std::string getQuestTrackerText() const;

private:
    void buildUI();
    void showQuestDetails(const Quest* quest);
    void createQuestListItem(const Quest& quest, cocos2d::Node* container, int yPos);
    
    cocos2d::ui::Scale9Sprite* m_bgPanel = nullptr;
    cocos2d::Node* m_questList = nullptr;
    cocos2d::Node* m_questDetail = nullptr;
    cocos2d::Label* m_titleLabel = nullptr;
    cocos2d::Label* m_detailTitle = nullptr;
    cocos2d::Label* m_detailDesc = nullptr;
    cocos2d::Label* m_detailReward = nullptr;
    cocos2d::Label* m_detailProgress = nullptr;
    cocos2d::ui::Button* m_acceptBtn = nullptr;
    cocos2d::ui::Button* m_abandonBtn = nullptr;
    cocos2d::ui::Button* m_completeBtn = nullptr;
    
    cocos2d::ui::Button* m_tabMain = nullptr;
    cocos2d::ui::Button* m_tabSide = nullptr;
    cocos2d::ui::Button* m_tabActive = nullptr;
    
    int m_selectedQuestId = 0;
    int m_currentTab = 0; // 0=全部, 1=主线, 2=支线, 3=进行中
    bool m_isVisible = false;
};

#endif // __QUEST_PANEL_H__
