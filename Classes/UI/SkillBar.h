#ifndef __SKILL_BAR_H__
#define __SKILL_BAR_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

class SkillBar : public cocos2d::Node {
public:
    static SkillBar* create();
    virtual bool init();
    
    void refresh();

private:
    void onSkillClick(int skillId);
    
    cocos2d::Node* m_skillSlots = nullptr;
};

#endif // __SKILL_BAR_H__
