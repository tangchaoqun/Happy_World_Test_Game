#ifndef __GAME_DATA_MANAGER_H__
#define __GAME_DATA_MANAGER_H__

#include "cocos2d.h"
#include <unordered_map>
#include <string>
#include <functional>

class GameDataManager {
public:
    static GameDataManager* getInstance();
    
    void initialize();
    
    // Configuration
    void setConfig(const std::string& key, const std::string& value);
    std::string getConfig(const std::string& key, const std::string& defaultVal = "");
    void saveConfig();
    void loadConfig();
    
    // Time
    uint64_t getCurrentTimeMs() const;
    
private:
    GameDataManager() = default;
    ~GameDataManager() = default;
    
    static GameDataManager* s_instance;
    std::unordered_map<std::string, std::string> m_config;
};

// Scene manager for navigation
class SceneManager {
public:
    static SceneManager* getInstance();
    
    void registerScene(const std::string& name, cocos2d::Scene* scene);
    void switchTo(const std::string& name);
    void switchTo(const std::string& name, cocos2d::TransitionScene* transition);

private:
    SceneManager() = default;
    static SceneManager* s_instance;
    std::unordered_map<std::string, cocos2d::Scene*> m_scenes;
};

#endif // __GAME_DATA_MANAGER_H__
