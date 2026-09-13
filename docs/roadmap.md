# Roadmap — 6 mois vers tech lead

Chaque mois se termine par un livrable concret et, idéalement, une ADR
si des décisions structurantes ont été prises.

## Mois 1 — Architecture & fondations ✅ (ce commit)
- [x] Découpage modulaire (core, protocol, network, ui-components)
- [x] Conanfile.py pour chaque lib
- [x] Backend Drogon + repository PostgreSQL + filtre JWT
- [x] Client Qt/QML minimal (dashboard, ServicesViewModel)
- [x] ADR 0001, 0002, 0003
- [ ] Monter le Conan Server / Artifactory local et publier une première
      version 0.1.0 de chaque lib (à faire toi-même, en local)
- [ ] Premier commit + repo git initialisé

## Mois 2 — Backend solide
- [ ] Vraie authentification (endpoint `/auth/login`, génération JWT)
- [ ] Tests d'intégration du backend (démarrer Drogon en test, requêtes HTTP réelles)
- [ ] Pagination sur `GET /api/v1/services` si la liste grossit
- [ ] Gestion d'erreurs cohérente (format d'erreur JSON standardisé)
- [ ] Documentation de l'API (OpenAPI/Swagger, généré ou écrit à la main)

## Mois 3 — Client Qt/QML enrichi
- [ ] Écran de détail d'un service (historique de déploiement, graphe de santé dans le temps)
- [ ] Gestion des erreurs réseau dans l'UI (retry, message clair)
- [ ] Auto-refresh périodique (QTimer) configurable
- [ ] Tests QML (Qt Quick Test) sur StatusBadge/ServiceCard
- [ ] Packaging Qt (windeployqt/macdeployqt/linuxdeployqt) pour distribuer un binaire

## Mois 4 — CI/CD complet
- [ ] Pipeline CI qui build/teste réellement tout (ce commit pose la base)
- [ ] Migration Conan Server → Artifactory CE (voir ADR 0002)
- [ ] Versioning sémantique strict + changelog par lib
- [ ] Publication automatique sur tag git (`v0.2.0` -> bump toutes les libs concernées)
- [ ] Analyse statique (clang-tidy) intégrée au pipeline

## Mois 5 — Fonctionnalités avancées
- [ ] Cache local SQLite côté client (fonctionnement offline, sync au retour réseau)
- [ ] WebSocket pour recevoir les mises à jour de statut en temps réel (au lieu du polling)
- [ ] Architecture à plugins : un service tiers (ex. GitHub Actions status) branché
      comme plugin Qt chargé dynamiquement, sans toucher au cœur de l'app
- [ ] Observabilité : logs structurés (JSON), métriques exposées (Prometheus format)

## Mois 6 — Polish & posture tech lead
- [ ] Profiling perf (Valgrind/Massif côté backend, Qt Creator profiler côté client)
- [ ] Revue de sécurité (injections SQL — déjà mitigées par requêtes paramétrées,
      mais à auditer ; secrets JWT en variable d'env, jamais commit)
- [ ] Documentation d'architecture complète (schémas, ADR à jour, README par module)
- [ ] Rédiger des guidelines de code review comme si tu onboardais une équipe
      (checklist PR, conventions de nommage, structure des commits)
- [ ] Rétrospective écrite : ce que tu referais différemment avec le recul —
      exercice que tout tech lead pratique régulièrement

## Comment mesurer ta progression
À chaque mois, pose-toi ces 3 questions (posture tech lead, pas juste dev) :
1. Est-ce que quelqu'un d'autre pourrait reprendre ce module avec juste
   le README + l'ADR, sans me poser de question ?
2. Si je devais faire un breaking change ici, est-ce que je saurais
   comment le communiquer et le versionner proprement ?
3. Qu'est-ce que j'ai dû arbitrer ce mois-ci, et est-ce que je l'ai documenté ?
