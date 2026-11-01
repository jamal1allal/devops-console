#include <iostream>
#include <string>

#include "auth/password_hasher.h"

// Outil d'administration : lit un mot de passe sur l'entrée standard et
// affiche son hash. Lire sur stdin évite de laisser le mot de passe dans
// l'historique du shell ou dans la liste des processus.
int main() {
    std::string password;
    std::getline(std::cin, password);
    if (password.empty()) {
        std::cerr << "empty password\n";
        return 1;
    }
    std::cout << devops::backend::auth::PasswordHasher::hash(password) << '\n';
    return 0;
}