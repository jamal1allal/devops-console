# ADR 0003 — Drogon + PostgreSQL pour le backend REST

## Statut
Accepté

## Contexte
Le backend doit exposer une API REST consommée par `libs/network`, et
persister l'état des services + historique de déploiement.

## Décision
- **Drogon** comme framework HTTP C++ : asynchrone, orienté controllers
  façon Spring/Express, packagé Conan directement (`drogon/1.9.4`),
  intègre déjà un client PostgreSQL asynchrone.
- **PostgreSQL** comme SGBD : robuste, gratuit, bon support des types
  temporels (`TIMESTAMPTZ`) utiles pour l'historique de déploiement.
- **jwt-cpp** pour l'authentification par JWT, appliqué via un
  `HttpFilter` Drogon plutôt que vérifié manuellement dans chaque
  controller — un seul point à auditer en cas de faille.
- Le SQL brut est isolé dans `ServiceRepository` (pas d'ORM) : à ce
  stade du projet, un ORM ajouterait de la magie sans réel bénéfice sur
  un schéma aussi simple (2 tables). À réévaluer si le schéma grossit.

## Conséquences
- Migrations SQL versionnées manuellement (`db/migrations/000X_*.sql`)
  et appliquées via `psql`. Pas d'outil de migration automatique pour
  l'instant (candidat naturel d'amélioration au Mois 5-6, ex. `dbmate`
  ou un système de migration Drogon).
- Le repository expose des méthodes synchrones (`execSqlSync`) : plus
  simple à écrire et à tester au démarrage du projet, au prix de
  bloquer un thread du pool Drogon par requête. Acceptable au volume
  actuel ; à documenter comme dette technique si le trafic grossit.

## Alternative rejetée
- **cpp-httplib côté serveur** (comme côté client) : plus simple mais
  sans gestion native de la BDD ni de controllers structurés — aurait
  fait grossir le code applicatif pour réinventer ce que Drogon fournit.
- **gRPC** : pertinent pour de la communication interservices, mais le
  brief demandait explicitement des appels REST consommés par un client
  Qt/QML, donc HTTP+JSON reste le choix le plus direct.
