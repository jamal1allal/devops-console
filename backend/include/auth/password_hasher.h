#pragma once

#include <string>

namespace devops::backend::auth {

/// Hash et vérification de mots de passe (argon2id via libsodium).
/// Le hash produit contient son propre sel et ses paramètres : on ne stocke
/// que cette chaîne dans la colonne `users.password_hash`.
class PasswordHasher {
public:
    static std::string hash(const std::string& password);
    static bool verify(const std::string& hash, const std::string& password);
};

}  // namespace devops::backend::auth