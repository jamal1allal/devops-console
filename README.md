# DevOps Console

[![CI](https://github.com/jamal1allal/devops-console/actions/workflows/ci.yml/badge.svg)](https://github.com/jamal1allal/devops-console/actions/workflows/ci.yml)

[![CI](https://github.com/jamal1allal/devops-console/actions/workflows/ci.yml/badge.svg)](https://github.com/jamal1allal/devops-console/actions/workflows/ci.yml)

Console de supervision interne : suivi de l'état de services/pipelines,
historique en base, packagée comme un ensemble de libs C++ versionnées
via Conan, publiées sur un dépôt privé (Artifactory / Conan Server).

## Pourquoi ce projet

Ce n'est pas juste "une app Qt qui appelle une API". L'objectif est de
reproduire, en solo, ce qu'un tech lead doit poser dans une vraie équipe :

- une architecture en modules indépendants, versionnés séparément
- un dépôt de packages privé (comme s'il y avait plusieurs équipes qui
  consomment les mêmes libs)
- un contrat d'API REST clair entre client et backend
- un pipeline CI/CD qui build/teste/publie chaque brique
- des décisions d'architecture documentées (ADR) et pas juste du code

## Vue d'ensemble

```
                    ┌─────────────────────┐
                    │   app/ (Qt/QML)      │   Client desktop
                    │   MVVM, dashboard    │
                    └──────────┬───────────┘
                               │ HTTP/JSON (REST)
                    ┌──────────▼───────────┐
                    │  backend/ (Drogon)    │   Service C++
                    │  API REST + auth JWT  │
                    └──────────┬───────────┘
                               │ SQL
                    ┌──────────▼───────────┐
                    │   PostgreSQL          │   Historique, users, services
                    └───────────────────────┘

  libs/core            -> logging, config, event bus (dépendance commune)
  libs/protocol        -> DTOs + (dé)sérialisation JSON, partagé client/backend
  libs/network         -> client HTTP générique (au-dessus de core)
  libs/ui-components   -> composants QML réutilisables (au-dessus de core)
```

Chaque lib sous `libs/` est un **package Conan indépendant**, versionné et
publié séparément sur le dépôt privé. `app/` et `backend/` les consomment
comme des dépendances externes, pas comme du code partagé en local — c'est
ce qui force à respecter des contrats d'interface propres.

## Prérequis

- CMake ≥ 3.21, Ninja
- Conan 2.x (`pip install conan`)
- Qt 6.5+ (Widgets, Qml, Quick, Network)
- Docker (pour PostgreSQL + Conan Server / Artifactory local)

## Démarrer le dépôt Conan privé en local

Voir `docs/adr/0002-conan-artifactory.md` pour le détail. En résumé :

```bash
docker run -d -p 9300:9300 -p 8081:8081 conanio/conan-server
conan remote add devops-local http://localhost:9300
```

## Ordre de build (respecte les dépendances entre modules)

```bash
# 1. Build + publie chaque lib comme package Conan
cd libs/core     && conan create . --version=0.1.0
cd libs/protocol && conan create . --version=0.1.0
cd libs/network  && conan create . --version=0.1.0
cd libs/ui-components && conan create . --version=0.1.0

# 2. Publie vers le dépôt privé
conan upload "*/0.1.0" -r devops-local --confirm

# 3. Build backend et app, qui consomment ces packages
cd backend && conan install . --build=missing && cmake --preset conan-release && cmake --build --preset conan-release
cd app     && conan install . --build=missing && cmake --preset conan-release && cmake --build --preset conan-release
```

## Roadmap

Voir `docs/roadmap.md` pour le détail des 6 mois. En bref :

1. **Mois 1** — architecture, CMake modulaire, Conan, dépôt privé (ce commit)
2. **Mois 2** — backend REST + PostgreSQL + auth JWT
3. **Mois 3** — client Qt/QML MVVM consommant l'API
4. **Mois 4** — CI/CD complet (build/test/package/publish)
5. **Mois 5** — offline cache SQLite, plugins, WebSocket temps réel
6. **Mois 6** — perf, sécurité, documentation, guidelines de code review

## Documentation d'architecture

Voir `docs/adr/` — chaque décision structurante est tracée sous forme
d'ADR (Architecture Decision Record), à la manière dont on le ferait
dans une vraie équipe.
