#ifndef __STORY_MANAGER_H__
#define __STORY_MANAGER_H__

#include "cocos2d.h"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

// 任务类型
enum class QuestType {
    Main,       // 主线任务
    Side,       // 支线任务
    Daily,      // 日常任务
    Event       // 活动任务
};

// 任务状态
enum class QuestState {
    NotStarted,     // 未开始
    Active,         // 进行中
    Completed,      // 已完成但未交付
    Finished        // 已交付完成
};

// 任务目标类型
enum class QuestObjectiveType {
    KillMonster,        // 击杀怪物
    CollectItem,        // 收集物品
    TalkToNPC,          // 与NPC对话
    ReachLocation,      // 到达地点
    DefeatBoss,         // 击败Boss
    UseSkill,           // 使用技能
    LevelUp             // 升级
};

// 任务目标
struct QuestObjective {
    int id = 0;
    std::string description;        // 目标描述
    QuestObjectiveType type;        // 目标类型
    int targetId = 0;               // 目标ID（怪物ID、物品ID、NPC ID等）
    int requiredCount = 1;          // 需要完成数量
    int currentCount = 0;           // 当前完成数量
    bool completed = false;         // 是否已完成
    std::string locationHint;       // 地点提示
};

// 任务奖励
struct QuestReward {
    int exp = 0;
    int gold = 0;
    std::vector<std::pair<int, int>> items; // <itemId, count>
    std::string rewardEquipment;    // 装备奖励名称
};

// 任务
struct Quest {
    int id = 0;
    std::string title;              // 任务标题
    std::string description;        // 任务描述
    std::string brief;              // 任务简介（列表中显示）
    QuestType type = QuestType::Main;
    QuestState state = QuestState::NotStarted;
    std::vector<QuestObjective> objectives; // 任务目标列表
    QuestReward reward;
    int requiredLevel = 1;          // 需求等级
    int preQuestId = 0;             // 前置任务ID
    std::string startNpc;           // 起始NPC
    std::string endNpc;             // 结束NPC
    bool isRepeatable = false;      // 是否可重复
};

// 剧情对话节点
struct DialogNode {
    int id = 0;
    std::string speaker;            // 说话者
    std::string text;               // 对话内容
    std::string speakerIcon;        // 头像图标
    std::vector<std::pair<int, std::string>> choices; // <nextNodeId, choiceText>
    bool isEnd = false;             // 是否结束
    std::function<void()> onStart;  // 开始回调
    std::function<void()> onEnd;    // 结束回调
    int questId = 0;                // 触发任务ID
    int completeQuestId = 0;        // 完成任务ID
};

// 剧情章节
struct StoryChapter {
    int id = 0;
    std::string title;              // 章节标题
    std::string description;        // 章节描述
    std::vector<DialogNode> dialogs; // 对话序列
    bool isCompleted = false;       // 是否完成
    int requiredChapterId = 0;      // 前置章节
};

// 剧情事件类型
enum class StoryEventType {
    QuestStart,         // 任务开始
    QuestComplete,      // 任务完成
    ChapterStart,       // 章节开始
    ChapterComplete,    // 章节完成
    DialogTrigger,      // 对话触发
    CutscenePlay        // 过场动画
};

// 故事管理器
class StoryManager {
public:
    static StoryManager* getInstance();
    
    void initialize();
    
    // 任务管理
    void addQuest(const Quest& quest);
    void startQuest(int questId);
    void completeQuestObjective(int questId, int objectiveId);
    void finishQuest(int questId);
    void abandonQuest(int questId);
    
    const std::vector<Quest>& getActiveQuests() const { return m_activeQuests; }
    const std::vector<Quest>& getCompletedQuests() const { return m_completedQuests; }
    const std::vector<Quest>& getAvailableQuests() const { return m_availableQuests; }
    
    Quest* getQuest(int questId);
    bool hasQuest(int questId) const;
    bool isQuestCompleted(int questId) const;
    bool isQuestActive(int questId) const;
    
    // 任务进度更新
    void updateKillObjective(int monsterId, int count = 1);
    void updateCollectObjective(int itemId, int count = 1);
    void updateTalkObjective(int npcId);
    void updateReachLocation(int locationId);
    void updateSkillUse(int skillId);
    void updateLevelUp(int level);
    
    // 对话系统
    void startDialog(int chapterId, int startNodeId = 0);
    void showNextDialog();
    void selectChoice(int choiceIndex);
    bool isDialogActive() const { return m_currentDialogIndex >= 0; }
    const DialogNode* getCurrentDialog() const;
    
    // 章节管理
    void startChapter(int chapterId);
    void completeChapter(int chapterId);
    bool isChapterCompleted(int chapterId) const;
    const StoryChapter* getChapter(int chapterId) const;
    int getCurrentChapterId() const { return m_currentChapterId; }
    
    // 事件监听
    using StoryEventCallback = std::function<void(StoryEventType, int)>;
    void addEventListener(const std::string& name, StoryEventCallback callback);
    void removeEventListener(const std::string& name);
    void notifyEvent(StoryEventType type, int id);
    
    // 保存/加载
    void saveProgress();
    void loadProgress();
    void resetProgress();
    
    // 获取剧情统计
    int getTotalQuestsCompleted() const { return static_cast<int>(m_completedQuests.size()); }
    int getCurrentChapterNumber() const;
    std::string getStorySummary() const;

private:
    StoryManager() = default;
    ~StoryManager() = default;
    
    void setupDefaultStory();
    void setupQuests();
    void setupChapters();
    void checkQuestCompletion(Quest& quest);
    void updateAvailableQuests();
    
    static StoryManager* s_instance;
    
    // 任务数据
    std::unordered_map<int, Quest> m_questDatabase;     // 所有任务数据库
    std::vector<Quest> m_activeQuests;                  // 进行中任务
    std::vector<Quest> m_completedQuests;               // 已完成任务
    std::vector<Quest> m_availableQuests;               // 可接任务
    
    // 剧情章节
    std::unordered_map<int, StoryChapter> m_chapters;
    int m_currentChapterId = 0;
    
    // 对话状态
    int m_currentDialogChapterId = -1;
    int m_currentDialogIndex = -1;
    std::vector<DialogNode> m_currentDialogSequence;
    
    // 事件监听
    std::unordered_map<std::string, StoryEventCallback> m_eventListeners;
    
    // 保存的进度
    std::string m_saveFile = "story_progress.json";
};

#endif // __STORY_MANAGER_H__