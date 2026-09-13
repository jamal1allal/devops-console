#include "core/event_bus.h"

namespace devops::core {

EventBus& EventBus::instance() {
    static EventBus bus;
    return bus;
}

}  // namespace devops::core
