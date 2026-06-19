#ifndef __CITY_SCENE_H__
#define __CITY_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../UI/Joystick.h"
#include "../UI/HpMpBar.h"
#include "../UI/PlayerInfoPanel.h"
#include "../UI/InventoryPanel.h"
#include "../UI/SkillBar.h"
#include "../UI/QuestPanel.h"
#include "../Story/DialogPanel.h"
#include "../Story/StoryManager.h"

// NPC信息结构
struct NPCInfo {
    std::string name;
    std::string role;
    cocos2d::Vec2 position;
    int chapterId;          // 关联的章节ID
    cocos2d::Sprite* sprite = nullptr;
    cocos2d::Label* nameLabel = nullptr;
};

class CityScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(CityScene);
    
    virtual void update(float dt) override;

private:
    void setupMap();
    void setupUI();
    void setupPlayer();
    void setupMonsters();
    void setupSkills();
    void setupNPCs();           // 设置NPC
    void setupStory();          // 初始化剧情系统
    
    void onAttack(const cocos2d::Vec2& direction);
    void onUseSkill(int skillId);
    void onToggleInventory();
    void onTogglePlayerInfo();
    void onToggleQuestPanel();  // 打开任务面板
    
    // NPC交互
    void checkNPCInteraction(); // 检查是否靠近NPC
    void interactWithNPC(const std::string& npcName);
    void showNPCDialog(const std::string& npcName);
    
    // 剧情相关
    void onDialogChoice(int choiceIndex);
    void onDialogClose();
    void showChapterDialog(int chapterId);
    void showQuestNotification(const std::string& title, const std::string& text);
    void updateQuestTracker();  // 更新任务追踪显示
    
    // 技能效果
    void performNormalAttack();
    void performFireballSkill();
    void performHealSkill();
    void spawnDamageNumber(const cocos2d::Vec2& pos, int damage, const cocos2d::Color4B& color);

    cocos2d::Sprite* m_playerSprite = nullptr;
    cocos2d::Sprite* m_mapLayer = nullptr;
    
    Joystick* m_joystick = nullptr;
    HpMpBar* m_hpMpBar = nullptr;
    PlayerInfoPanel* m_playerInfoPanel = nullptr;
    InventoryPanel* m_inventoryPanel = nullptr;
    SkillBar* m_skillBar = nullptr;
    QuestPanel* m_questPanel = nullptr;     // 任务面板
    DialogPanel* m_dialogPanel = nullptr;     // 对话面板
    
    // 任务追踪显示
    cocos2d::Label* m_questTrackerLabel = nullptr;
    
    // NPC
    std::vector<NPCInfo> m_npcs;
    cocos2d::Label* m_interactionHint = nullptr; // 交互提示
    
    bool m_inventoryVisible = false;
    bool m_playerInfoVisible = false;
    bool m_dialogActive = false;            // 是否在对话中
    
    float m_moveSpeed = 200.0f;
    float m_attackCooldown = 0.0f;
    static constexpr float ATTACK_CD = 0.5f;
    static constexpr float NPC_INTERACT_DISTANCE = 100.0f; // NPC交互距离

    // 键盘移动
    bool m_keyUp = false;
    bool m_keyDown = false;
    bool m_keyLeft = false;
    bool m_keyRight = false;
};

#endif // __CITY_SCENE_H__
