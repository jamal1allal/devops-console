# ADR 0001 — Découpage en modules indépendants (core / protocol / network / ui-components)

## Statut
Accepté

## Contexte
Le projet doit servir de terrain d'entraînement à des pratiques d'équipe :
gestion de dépendances versionnées, contrats d'interface explicites,
publication vers un dépôt privé. Un unique gros projet CMake monolithique
n'aurait pas forcé ces décisions.

## Décision
Le code est découpé en 4 libs indépendantes, chacune publiable comme
package Conan séparé :

- **core** — logging, config, event bus. Aucune dépendance externe.
  C'est la seule lib que toutes les autres peuvent utiliser.
- **protocol** — DTOs (`ServiceStatus`, `DeploymentEvent`) et leur
  (dé)sérialisation JSON. Dépend de `core` et `nlohmann_json`. Ce module
  matérialise le **contrat d'API** entre `backend` et `app` : toute
  évolution de contrat REST doit passer par un bump de version ici.
- **network** — client REST générique (`RestClient`) + API métier
  (`ServicesApi`). Dépend de `core` et `protocol`. Ne connaît rien à Qt :
  réutilisable côté tests/CLI si besoin un jour.
- **ui-components** — ViewModels Qt (`ServicesViewModel`) et composants
  QML réutilisables. Dépend de `core` et `network`.

`backend/` et `app/` sont des **applications**, pas des libs : elles
consomment ces 4 packages comme des dépendances externes, jamais en
`add_subdirectory` en production (seulement en mode dev via l'option
`DEVOPS_BUILD_LIBS`).

## Conséquences
- Chaque lib a son propre cycle de version (semver) et son propre
  changelog implicite via les tags Conan.
- Un changement dans `protocol` casse potentiellement `backend` ET `app`
  en même temps si mal versionné : ça force à réfléchir aux breaking
  changes dès ce stade du projet, comme dans une vraie équipe multi-repo.
- Coût : plus de fichiers `conanfile.py`/`CMakeLists.txt` à maintenir
  qu'un monolithe. Accepté car c'est l'objectif pédagogique du projet.

## Alternative rejetée
Un unique `CMakeLists.txt` avec des `add_subdirectory` partout et pas de
Conan interne. Plus simple à court terme, mais n'entraîne à rien de ce
qu'un tech lead doit arbitrer (versioning, publication, contrats d'API).
