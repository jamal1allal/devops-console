#pragma once

#include <any>
#include <functional>
#include <mutex>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace devops::core {

/// Bus d'événements générique, découplé de Qt : il sert de colonne
/// vertébrale entre `network` (qui reçoit des données) et `ui-components`
/// (qui les affiche), sans que ces deux libs se connaissent directement.
///
/// Volontairement type-erased (std::any) plutôt que basé sur les Qt
/// signals/slots, pour rester réutilisable côté backend (qui n'a pas Qt).
class EventBus {
public:
    static EventBus& instance();

    template <typename Event>
    using Handler = std::function<void(const Event&)>;

    template <typename Event>
    void subscribe(Handler<Event> handler) {
        std::lock_guard lock(m_mutex);
        auto& handlers = m_handlers[std::type_index(typeid(Event))];
        handlers.push_back([handler = std::move(handler)](const std::any& payload) {
            handler(std::any_cast<const Event&>(payload));
        });
    }

    template <typename Event>
    void publish(const Event& event) {
        std::vector<std::function<void(const std::any&)>> handlersCopy;
        {
            std::lock_guard lock(m_mutex);
            auto it = m_handlers.find(std::type_index(typeid(Event)));
            if (it == m_handlers.end()) return;
            handlersCopy = it->second;
        }
        for (auto& handler : handlersCopy) {
            handler(std::any(event));
        }
    }

private:
    EventBus() = default;

    std::mutex m_mutex;
    std::unordered_map<std::type_index, std::vector<std::function<void(const std::any&)>>> m_handlers;
};

}  // namespace devops::core
