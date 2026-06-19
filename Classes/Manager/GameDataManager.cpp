#include "GameDataManager.h"

USING_NS_CC;

// ---- GameDataManager ----

GameDataManager* GameDataManager::s_instance = nullptr;

GameDataManager* GameDataManager::getInstance() {
    if (!s_instance) {
        s_instance = new GameDataManager();
    }
    return s_instance;
}

void GameDataManager::initialize() {
    // Load saved game data
    auto path = FileUtils::getInstance()->getWritablePath() + "game_config.json";
    if (FileUtils::getInstance()->isFileExist(path)) {
        auto data = FileUtils::getInstance()->getStringFromFile(path);
        // Parse JSON and populate m_config
        log("[GameData] Config loaded from %s", path.c_str());
    }
    
    log("[GameData] Initialized");
}

void GameDataManager::setConfig(const std::string& key, const std::string& value) {
    m_config[key] = value;
}

std::string GameDataManager::getConfig(const std::string& key, const std::string& defaultVal) {
    auto it = m_config.find(key);
    return it != m_config.end() ? it->second : defaultVal;
}

void GameDataManager::saveConfig() {
    auto path = FileUtils::getInstance()->getWritablePath() + "game_config.json";
    std::string json = "{";
    bool first = true;
    for (const auto& pair : m_config) {
        if (!first) json += ",";
        json += "\"" + pair.first + "\":\"" + pair.second + "\"";
        first = false;
    }
    json += "}";
    FileUtils::getInstance()->writeStringToFile(json, path);
    log("[GameData] Config saved to %s", path.c_str());
}

void GameDataManager::loadConfig() {
    auto path = FileUtils::getInstance()->getWritablePath() + "game_config.json";
    if (FileUtils::getInstance()->isFileExist(path)) {
        auto data = FileUtils::getInstance()->getStringFromFile(path);
        // Simple JSON parse for flat key-value
        size_t pos = 1;
        while (pos < data.size()) {
            auto keyStart = data.find('"', pos);
            if (keyStart == std::string::npos) break;
            auto keyEnd = data.find('"', keyStart + 1);
            if (keyEnd == std::string::npos) break;
            auto valStart = data.find('"', keyEnd + 2);
            if (valStart == std::string::npos) break;
            auto valEnd = data.find('"', valStart + 1);
            if (valEnd == std::string::npos) break;
            std::string key = data.substr(keyStart + 1, keyEnd - keyStart - 1);
            std::string val = data.substr(valStart + 1, valEnd - valStart - 1);
            m_config[key] = val;
            pos = valEnd + 1;
        }
        log("[GameData] Config loaded from %s", path.c_str());
    }
}

uint64_t GameDataManager::getCurrentTimeMs() const {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return ms.count();
}

// ---- SceneManager ----

SceneManager* SceneManager::s_instance = nullptr;

SceneManager* SceneManager::getInstance() {
    if (!s_instance) {
        s_instance = new SceneManager();
    }
    return s_instance;
}

void SceneManager::registerScene(const std::string& name, Scene* scene) {
    m_scenes[name] = scene;
}

void SceneManager::switchTo(const std::string& name) {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end()) {
        Director::getInstance()->replaceScene(it->second);
    }
}

void SceneManager::switchTo(const std::string& name, TransitionScene* transition) {
    auto it = m_scenes.find(name);
    if (it != m_scenes.end()) {
        Director::getInstance()->replaceScene(transition);
    }
}
