#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace devops::core {

/// Configuration simple (fichier .env-like ou variables d'environnement).
/// Sert par exemple à `network` pour connaître l'URL de base du backend,
/// sans que cette URL soit codée en dur dans l'app ni dans la lib.
class Config {
public:
    static Config& instance();

    /// Charge un fichier au format `CLE=valeur` (une entrée par ligne,
    /// lignes commençant par # ignorées).
    bool loadFromFile(const std::string& path);

    /// Priorité : valeur explicite chargée > variable d'environnement > défaut.
    std::string get(const std::string& key, const std::string& defaultValue = "") const;
    std::optional<int> getInt(const std::string& key) const;

    void set(const std::string& key, const std::string& value);

private:
    Config() = default;
    std::unordered_map<std::string, std::string> m_values;
};

}  // namespace devops::core
