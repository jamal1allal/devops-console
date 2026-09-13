# ADR 0002 — Dépôt Conan privé (Conan Server / Artifactory)

## Statut
Accepté

## Contexte
Les 4 libs internes (`devops-core`, `devops-protocol`, `devops-network`,
`devops-ui-components`) doivent être publiées quelque part pour que
`backend/` et `app/` puissent les consommer par version, comme le
feraient deux équipes différentes dans une vraie organisation.

## Décision
On utilise un dépôt Conan privé auto-hébergé en local via Docker, avec
deux options possibles :

1. **`conanio/conan-server`** — léger, suffisant pour développer seul et
   comprendre le mécanisme (remote add, upload, download).
2. **JFrog Artifactory CE (Community Edition)** — plus proche d'un vrai
   contexte d'entreprise (UI web, gestion de utilisateurs/permissions,
   support multi-format au-delà de Conan).

Recommandation : démarrer avec `conan-server` pour les Mois 1-2 (mise en
place rapide), puis migrer vers Artifactory CE au Mois 4 quand le
pipeline CI doit publier automatiquement — c'est l'occasion de pratiquer
une vraie migration d'infra, ce qu'un tech lead fait régulièrement.

```bash
# Option 1 : Conan Server (rapide à démarrer)
docker run -d --name conan-server -p 9300:9300 conanio/conan-server
conan remote add devops-local http://localhost:9300

# Option 2 : Artifactory CE (plus proche de la prod)
docker run -d --name artifactory -p 8081:8081 -p 8082:8082 \
  releases-docker.jfrog.io/jfrog/artifactory-cnas:latest
# Puis créer un repo Conan local depuis l'UI (http://localhost:8082)
conan remote add devops-artifactory http://localhost:8081/artifactory/api/conan/devops-conan-local
```

## Conséquences
- Chaque `conan create` doit être suivi d'un `conan upload` explicite :
  ça matérialise la frontière entre "j'ai buildé en local" et "j'ai
  publié une version que d'autres composants peuvent consommer".
- Nécessite de gérer des credentials (`conan remote login`) même en
  local — bonne pratique à prendre tôt plutôt qu'à découvrir en prod.
- Le passage à Artifactory CE en Mois 4 est un exercice volontaire de
  migration d'infra, pas juste "on démarre avec le bon outil direct".

## Alternative rejetée
Publier les libs internes directement sur Conan Center ou un artefact
public. Rejeté : dans une vraie boîte, le code interne n'est jamais
public, et l'objectif est justement de pratiquer la gestion d'un dépôt
privé.
