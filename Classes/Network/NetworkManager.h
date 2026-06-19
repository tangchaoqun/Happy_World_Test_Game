#ifndef __NETWORK_MANAGER_H__
#define __NETWORK_MANAGER_H__

#include "cocos2d.h"
#include "network/WebSocket.h"
#include <functional>
#include <string>

class NetworkManager {
public:
    static NetworkManager* getInstance();
    
    void connect(const std::string& url);
    void disconnect();
    bool isConnected() const { return m_connected; }
    
    // Auth
    using AuthCallback = std::function<void(bool success, const std::string& msg)>;
    void sendLogin(const std::string& account, const std::string& password, AuthCallback callback);
    void sendRegister(const std::string& account, const std::string& password, AuthCallback callback);
    
    // Game data
    void sendPlayerData();
    void sendChat(const std::string& message);
    
    // Message handling
    void onMessage(const std::string& data);

private:
    NetworkManager();
    ~NetworkManager();
    
    static NetworkManager* s_instance;
    
    bool m_connected = false;
    std::string m_serverUrl;
    
    // WebSocket instance (using cocos2d-x network)
    cocos2d::network::WebSocket* m_ws = nullptr;
    
    // Pending callbacks
    AuthCallback m_loginCallback = nullptr;
    AuthCallback m_registerCallback = nullptr;
};

#endif // __NETWORK_MANAGER_H__
