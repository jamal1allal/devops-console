#include "auth/password_hasher.h"

#include <sodium.h>

#include <array>
#include <stdexcept>

namespace devops::backend::auth {

namespace {
void ensureSodiumInitialized() {
    // sodium_init() peut être appelée plusieurs fois, mais on ne veut
    // l'exécuter qu'une fois ; l'initialisation d'une static est thread-safe.
    static const bool ok = (sodium_init() >= 0);
    if (!ok) throw std::runtime_error("libsodium initialization failed");
}
}  // namespace

std::string PasswordHasher::hash(const std::string& password) {
    ensureSodiumInitialized();

    std::array<char, crypto_pwhash_STRBYTES> out{};
    if (crypto_pwhash_str(out.data(), password.c_str(), password.size(),
                          crypto_pwhash_OPSLIMIT_INTERACTIVE,
                          crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
        throw std::runtime_error("password hashing failed (out of memory)");
    }
    return std::string(out.data());
}

bool PasswordHasher::verify(const std::string& hash, const std::string& password) {
    ensureSodiumInitialized();
    return crypto_pwhash_str_verify(hash.c_str(), password.c_str(), password.size()) == 0;
}

}  // namespace devops::backend::auth