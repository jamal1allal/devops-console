# Roadmap : 8 mois, orientée CI/CD, DevOps et cloud

Règle de travail : j'écris le code, la review est faite sur chaque ticket.
Chaque mois se termine par un livrable qui tourne, au moins une ADR rédigée
par moi, et un `git status` propre.

Principe : le pipeline grandit avec le projet. Chaque mois ajoute un étage
au lieu d'attendre un "mois DevOps" à la fin.

## Mois 1 : architecture et fondations (TERMINÉ)

- [x] Découpage modulaire : core, protocol, network, ui-components
- [x] Packages Conan indépendants, publiés sur un dépôt privé (conan_server en Docker)
- [x] Backend Drogon + PostgreSQL, API REST fonctionnelle
- [x] Client Qt/QML qui affiche les données du backend
- [x] ADR 0001, 0002, 0003

Dettes :
- [x] Token GitHub absent de l'historique (vérifié)
- [ ] Révoquer l'ancien token sur github.com/settings/tokens
- Abandonnées volontairement : ADR 0004 (modules QML statiques), tests de
  `libs/protocol`, note ADR 0003 sur `with_postgres`

## Mois 2 : auth minimale, première CI, premier Docker (EN COURS)

Choix assumé : le backend reste simple (pas de pagination, pas d'OpenAPI,
pas de format d'erreur raffiné). Le temps gagné va au DevOps.

Semaine 1 : auth minimale (TICKET-001) : TERMINÉE
- [x] Migration `0002_users.sql` : table `users` (`id`, `username` unique,
      `password_hash`, `created_at`)
- [x] Code d'auth isolé dans `backend/auth/`, séparé de `backend/services/`
      (pour l'extraction en service au Mois 4)
- [x] `POST /api/v1/auth/login` : JWT avec `sub`, `iss`, `exp` (24 h), hash argon2id
      (libsodium ; pas de paquet bcrypt sur Conan Center)
- [x] `AuthFilter` branché sur `/api/v1/services*` (401 sans token, 200 avec)
- [x] Tests : hash/vérification (gtest) + `auth_smoke.sh` de bout en bout
- [x] `JWT_SECRET` obligatoire : le backend refuse de démarrer sans lui
- [x] ADR 0005 : choix du hachage de mot de passe (argon2id)

Semaine 2 : CI GitHub Actions verte : TERMINÉE
- [x] Build des libs (core, protocol), tests gtest, build du backend à chaque push
- [x] Badge vert dans le README
- Leçons : un job = une machine vide (pas de cache partagé sans `actions/cache`) ;
  `libpqxx` 7.9.0 ne se configure pas sur machine vide, passé en 7.9.2.
- Reste à ajouter plus tard : `libs/network`, `ui-components` et l'app Qt en CI ;
  `auth_smoke.sh` en CI (avec `docker compose`, semaine 4).

Semaine 3 : Docker
- [ ] `Dockerfile` multi-stage du backend (build Conan + CMake, image finale légère)

Semaine 4 : compose et configuration
- [ ] `docker-compose.yml` : backend + PostgreSQL
- [ ] Configuration par variables d'environnement, jamais dans l'image

Livrable : un push déclenche une CI verte, et `docker compose up` lance le
backend avec sa base.

## Mois 3 : images publiées, sécurité, début de Terraform

- [ ] La CI construit l'image et la publie sur GHCR
- [ ] Scan de sécurité de l'image (Trivy) dans le pipeline
- [ ] Client Qt minimal : login, liste, détail, gestion des erreurs réseau
      (distinguer « backend injoignable » de « réponse invalide »)
- [ ] Bases de Terraform : cycle `plan` / `apply` / `destroy` sur un module simple
- [ ] Bases de bash et Python : script de vérification de santé

Livrable : un push publie une image scannée sur GHCR.

## Mois 4 : CI/CD complet et découpage en services

- [ ] Migration de conan_server vers Artifactory CE
- [ ] Semver strict, changelog par lib, publication des libs sur tag git
- [ ] `clang-tidy` et scan de sécurité des images (Trivy) dans le pipeline
- [ ] Extraction d'`auth-service` : exécutable, migration, image et `conanfile.py` propres
- [ ] JWT signés en RS256 : seul `auth-service` détient la clé privée
- [ ] ADR : pourquoi découper (motivation : apprentissage de Kubernetes et profils
      de charge différents, pas une contrainte de charge réelle)

Livrable : un tag git build, teste, scanne et publie les libs et les images des 2 services.

## Mois 5 : Kubernetes local et observabilité

- [ ] Cluster local `kind` : `Deployment` et `Service` par composant,
      `ConfigMap`, `Secret`, probes liveness/readiness, `Ingress`
- [ ] Rolling update, rollback volontaire, panne provoquée d'un service
- [ ] Endpoint `GET /metrics` (format Prometheus) sur chaque service
- [ ] Prometheus et Grafana dans le cluster, dashboard versionné en JSON
- [ ] Option : `notifier` (WebSocket) en 3e service

Livrable : un cluster local avec 2-3 services, monitoré, qui survit à une panne.

## Mois 6 : Terraform et fondations GCP

- [ ] Projet GCP, budget d'alerte dès le premier jour, comptes de service à droits minimaux
- [ ] Terraform : Artifact Registry, Cloud SQL (plus petite instance), Secret Manager, réseau
- [ ] CI vers GCP avec Workload Identity Federation (pas de clé JSON dans GitHub)
- [ ] State Terraform distant (bucket GCS), `terraform plan` en commentaire de PR
- [ ] Bases de bash et Python : script de déploiement, vérification de santé

Livrable : `terraform apply` monte l'infra, `terraform destroy` la détruit proprement.

Point de contrôle : vérifier la facture GCP. Si elle dérive, rester sur Cloud Run
au Mois 7 et garder GKE pour plus tard.

## Mois 7 : GKE et déploiement continu

- [ ] GKE Autopilot : manifests du Mois 5 déployés sur un vrai cluster
- [ ] Cloud SQL branché, secrets via Secret Manager, HTTPS sur l'Ingress
- [ ] CD : un tag déclenche build, scan, push, déploiement
- [ ] Métriques via Managed Prometheus + Grafana (ou Cloud Monitoring), alertes simples
- [ ] Destruction des ressources après chaque session

Livrable : l'app Qt, lancée chez moi, affiche des données venant de GKE.

## Mois 8 : GitOps, fiabilité, portfolio

- [ ] GitOps avec Argo CD (ou Flux) : le dépôt de manifests est la source de vérité
- [ ] Fiabilité : un SLO simple, une alerte, un runbook, un incident simulé diagnostiqué
      avec les métriques
- [ ] Sécurité : SBOM, politiques de base, rotation d'un secret sans coupure
- [ ] Certification Associate Cloud Engineer (optionnel)
- [ ] README avec schéma d'architecture, puis candidatures

Livrable : un projet démontrable de bout en bout, avec une histoire à raconter en entretien.

## Mois 9 (optionnel) : ce qui a été sacrifié

- Cache SQLite côté client (mode hors ligne)
- Architecture à plugins Qt
- Profiling approfondi
- Écrans Qt avancés

## Points de contrôle

- Fin Semaine 1 du Mois 2 : si l'auth dépasse une semaine, la réduire encore
  (un seul utilisateur en base) plutôt que de retarder la CI et Docker.
- Fin Mois 4 : si le pipeline ou `auth-service` prend du retard, abandonner `notifier`.
- Fin Mois 6 : décider GKE ou Cloud Run selon la facture.
- Chaque mois : comparer avec de vraies offres d'emploi (LinkedIn, Welcome to the Jungle).

## Mesurer sa progression

1. Quelqu'un d'autre pourrait-il reprendre ce module avec le README et l'ADR seuls ?
2. Si je devais faire un breaking change ici, saurais-je le versionner et le communiquer ?
3. Qu'ai-je dû arbitrer ce mois-ci, et l'ai-je documenté ?

## Rappels

- Les Mois 5 à 7 sont denses : étaler sur 9 ou 10 mois plutôt que bâcler.
- GKE et Cloud SQL coûtent de l'argent : budget d'alerte et `terraform destroy`.
- Un projet perso n'est pas de l'expérience de production. L'atout reste la
  combinaison C++ et DevOps.
