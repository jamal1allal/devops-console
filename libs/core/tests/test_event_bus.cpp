#include <gtest/gtest.h>

#include "core/event_bus.h"

using namespace devops::core;

struct ServiceStatusChanged {
    std::string serviceName;
    bool healthy;
};

TEST(EventBus, PublishesToSubscriber) {
    auto& bus = EventBus::instance();

    ServiceStatusChanged received{};
    bool called = false;

    bus.subscribe<ServiceStatusChanged>([&](const ServiceStatusChanged& event) {
        received = event;
        called = true;
    });

    bus.publish(ServiceStatusChanged{"backend-api", false});

    EXPECT_TRUE(called);
    EXPECT_EQ(received.serviceName, "backend-api");
    EXPECT_FALSE(received.healthy);
}

TEST(EventBus, DoesNotCrashWithNoSubscriber) {
    struct UnhandledEvent {};
    EXPECT_NO_THROW(EventBus::instance().publish(UnhandledEvent{}));
}
