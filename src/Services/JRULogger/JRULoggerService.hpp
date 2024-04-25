/** @file JRULoggerService.hpp
 *
 *  Component   | Subset version
 *  :---------: | :-----------:
 *  ORC         | 2.3.0
 *
 *  ### Description
 *  Logger service for logging intern state of ORC to JRU module.
 *
 *  ### Contributors
 *  caslama1
 *  maresj39
 *  hamaljan
 */

#pragma once

#include <string>
 #include "MessageID.hpp"
 #include "InternalStateMessage.hpp"
 #include "ServiceContainer.hpp"
 #include "IInitializable.hpp"
 #include "IService.hpp"
 #include "IMqttPublisherService.hpp"
/**
 * JRULoggerService is a service, which is responsible for logging internal state
 * of the ORC component to JRU. It is basically a wrapper, so you are not copying
 * message id and type of message. For understanding how this work, you
 * should read description of InternalStateMessage.
 * @see InternalStateMessage
 * @see MessageID
 * @see MessageType
 */
class JRULoggerService : public IService, public IInitializable {
public:
    void Initialize(ServiceContainer& container) override {
        publisher = container.FetchService<IMqttPublisherService>().get();
    }

    static constexpr ServiceType Type = ServiceType::Logger;

    void Log(MessageType msgType, const std::string& text) const { Log(false, msgType, text); }

    template <typename... Args>
    void Log(MessageType msgType, const std::string& text, const Args&... args) const {
        Log(false, msgType, text, args...);
    }

    void Log(bool toConsole, MessageType msgType, const std::string& text) const {
        auto msg = std::make_shared<InternalStateMessage>(id, msgType, text);
#ifdef DEBUG
        if (toConsole) {
            InternalStateMessage toConsoleMsg;
            toConsoleMsg.from_json(msg.get()->to_json());
            std::cout << GetColor(msgType) << toConsoleMsg.GetMessage() << resetColor << std::endl;
        }
#endif
        publisher->Publish(msg);
    }

    template <typename... Args>
    void Log(bool toConsole, MessageType msgType, const std::string& text, const Args&... args) const {
        auto msg = std::make_shared<InternalStateMessage>(id, msgType, text, args...);
#ifdef DEBUG
        if (toConsole) {
            InternalStateMessage toConsoleMsg;
            toConsoleMsg.from_json(msg.get()->to_json());
            std::cout << GetColor(msgType) << toConsoleMsg.GetMessage() << resetColor << std::endl;
        }
#endif
        publisher->Publish(msg);
    }

private:
    MessageID id = MessageID::EVCtoJRU;     // TODO: change to ORCtoJRU
    IMqttPublisherService* publisher;
    std::string resetColor = "\033[0m";

    std::string GetColor(MessageType type) const {
        switch (type) {
            case MessageType::Fatal:
                return "\033[30;41m";
            case MessageType::Error:
                return "\033[0;31m";
            case MessageType::Warning:
                return "\033[1;33m";
            case MessageType::Info:
                return "\033[0;32m";
            case MessageType::Debug:
                return "\033[0;34m";
            default:
                break;
        }
        return resetColor;
    }
};
