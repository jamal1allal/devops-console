#include <gtest/gtest.h>

#include "auth/password_hasher.h"

using devops::backend::auth::PasswordHasher;

TEST(PasswordHasher, VerifiesCorrectPassword) {
    const auto hash = PasswordHasher::hash("s3cret-password");
    EXPECT_NE(hash, "s3cret-password");
    EXPECT_TRUE(PasswordHasher::verify(hash, "s3cret-password"));
}

TEST(PasswordHasher, RejectsWrongPassword) {
    const auto hash = PasswordHasher::hash("s3cret-password");
    EXPECT_FALSE(PasswordHasher::verify(hash, "wrong-password"));
}

TEST(PasswordHasher, SameInputGivesDifferentHashes) {
    // Chaque hash utilise un sel aléatoire différent.
    EXPECT_NE(PasswordHasher::hash("same"), PasswordHasher::hash("same"));
}