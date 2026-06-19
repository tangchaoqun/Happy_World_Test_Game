#include "NetworkManager.h"
#include "cocos2d.h"
#include "network/WebSocket.h"

USING_NS_CC;

NetworkManager* NetworkManager::s_instance = nullptr;

NetworkManager* NetworkManager::getInstance() {
    if (!s_instance) {
        s_instance = new NetworkManager();
    }
    return s_instance;
}

NetworkManager::NetworkManager() {}
NetworkManager::~NetworkManager() {
    disconnect();
}

void NetworkManager::connect(const std::string& url) {
    if (m_connected) return;
    
    m_serverUrl = url;
    
    // WebSocket implementation using cocos2d-x network module
    class WSDelegate : public cocos2d::network::WebSocket::Delegate {
    public:
        WSDelegate(NetworkManager* mgr) : m_mgr(mgr) {}
        
        virtual void onOpen(cocos2d::network::WebSocket* ws) override {
            m_mgr->m_connected = true;
            log("[Network] WebSocket connected to %s", m_mgr->m_serverUrl.c_str());
        }
        
        virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override {
            if (data.isBinary) {
                m_mgr->onMessage(std::string(data.bytes, data.len));
            } else {
                m_mgr->onMessage(std::string(data.bytes, data.len));
            }
        }
        
        virtual void onClose(cocos2d::network::WebSocket* ws) override {
            m_mgr->m_connected = false;
            log("[Network] WebSocket disconnected");
        }
        
        virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override {
            log("[Network] WebSocket error: %d", error);
            m_mgr->m_connected = false;
        }
        
    private:
        NetworkManager* m_mgr;
    };
    
    static WSDelegate delegate(this);
    m_ws = new cocos2d::network::WebSocket();
    m_ws->init(delegate, url);
}

void NetworkManager::disconnect() {
    if (m_ws) {
        m_ws->close();
        delete m_ws;
        m_ws = nullptr;
    }
    m_connected = false;
}

void NetworkManager::sendLogin(const std::string& account, const std::string& password, AuthCallback callback) {
    m_loginCallback = callback;
    
    if (!m_connected) {
        if (m_loginCallback) {
            m_loginCallback(false, "未连接到服务器");
            m_loginCallback = nullptr;
        }
        return;
    }
    
    // Send login message
    std::string msg = "{\"type\":\"login\",\"account\":\"" + account + "\",\"password\":\"" + password + "\"}";
    if (m_ws) {
        m_ws->send(msg);
    }
    
    // Simulate server response for offline development
    // In production, this would be handled by onMessage callback
    if (m_loginCallback) {
        m_loginCallback(true, "登录成功（离线模式）");
        m_loginCallback = nullptr;
    }
}

void NetworkManager::sendRegister(const std::string& account, const std::string& password, AuthCallback callback) {
    m_registerCallback = callback;
    
    if (!m_connected) {
        if (m_registerCallback) {
            m_registerCallback(false, "未连接到服务器");
            m_registerCallback = nullptr;
        }
        return;
    }
    
    std::string msg = "{\"type\":\"register\",\"account\":\"" + account + "\",\"password\":\"" + password + "\"}";
    if (m_ws) {
        m_ws->send(msg);
    }
    
    // Simulate server response
    if (m_registerCallback) {
        m_registerCallback(true, "注册成功（离线模式）");
        m_registerCallback = nullptr;
    }
}

void NetworkManager::sendPlayerData() {
    if (!m_connected || !m_ws) return;
    std::string msg = "{\"type\":\"player_data\"}";
    m_ws->send(msg);
}

void NetworkManager::sendChat(const std::string& message) {
    if (!m_connected || !m_ws) return;
    std::string msg = "{\"type\":\"chat\",\"content\":\"" + message + "\"}";
    m_ws->send(msg);
}

void NetworkManager::onMessage(const std::string& data) {
    // Parse JSON response and dispatch to handlers
    // This would parse messages like login/register results, game state updates, etc.
    log("[Network] Received: %s", data.c_str());
    
    // Simple JSON parsing (in production use rapidjson or nlohmann/json)
    if (data.find("\"type\":\"login_result\"") != std::string::npos) {
        bool success = data.find("\"success\":true") != std::string::npos;
        if (m_loginCallback) {
            m_loginCallback(success, success ? "登录成功" : "登录失败");
            m_loginCallback = nullptr;
        }
    } else if (data.find("\"type\":\"register_result\"") != std::string::npos) {
        bool success = data.find("\"success\":true") != std::string::npos;
        if (m_registerCallback) {
            m_registerCallback(success, success ? "注册成功" : "注册失败");
            m_registerCallback = nullptr;
        }
    }
}
