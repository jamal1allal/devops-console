#include <gtest/gtest.h>

#include "core/logger.h"

using namespace devops::core;

TEST(Logger, SingletonReturnsSameInstance) {
    EXPECT_EQ(&Logger::instance(), &Logger::instance());
}

TEST(Logger, DoesNotThrowOnAnyLevel) {
    auto& logger = Logger::instance();
    EXPECT_NO_THROW(logger.debug("test", "debug message"));
    EXPECT_NO_THROW(logger.info("test", "info message"));
    EXPECT_NO_THROW(logger.warning("test", "warning message"));
    EXPECT_NO_THROW(logger.error("test", "error message"));
}
