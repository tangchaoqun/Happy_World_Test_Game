#ifndef __PLAYER_INFO_PANEL_H__
#define __PLAYER_INFO_PANEL_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

class PlayerInfoPanel : public cocos2d::Node {
public:
    static PlayerInfoPanel* create();
    virtual bool init();
    
    void refresh();

private:
    void onClose(cocos2d::Ref* sender);
    
    cocos2d::Label* m_levelLabel = nullptr;
    cocos2d::Label* m_expLabel = nullptr;
    cocos2d::Label* m_hpLabel = nullptr;
    cocos2d::Label* m_mpLabel = nullptr;
    cocos2d::Label* m_attackLabel = nullptr;
    cocos2d::Label* m_defenseLabel = nullptr;
    cocos2d::Label* m_strengthLabel = nullptr;
    cocos2d::Label* m_agilityLabel = nullptr;
    cocos2d::Label* m_intelligenceLabel = nullptr;
    cocos2d::Label* m_critLabel = nullptr;
    cocos2d::Label* m_equipmentLabel = nullptr;
    cocos2d::ui::Button* m_closeBtn = nullptr;
};

#endif // __PLAYER_INFO_PANEL_H__
