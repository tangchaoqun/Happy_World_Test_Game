#ifndef __INVENTORY_PANEL_H__
#define __INVENTORY_PANEL_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/CocosGUI.h"

class InventoryPanel : public cocos2d::Node {
public:
    static InventoryPanel* create();
    virtual bool init();
    
    void refresh();

private:
    void onClose(cocos2d::Ref* sender);
    void onItemClick(int itemId);
    
    cocos2d::Node* m_itemGrid = nullptr;
    cocos2d::Label* m_infoLabel = nullptr;
    cocos2d::ui::Button* m_closeBtn = nullptr;
};

#endif // __INVENTORY_PANEL_H__
