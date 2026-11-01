#!/usr/bin/env bash
# Tests de bout en bout de l'authentification, contre un backend en cours d'exécution.
# Usage : BASE_URL=http://localhost:8090 TEST_USER=admin TEST_PASSWORD=... ./auth_smoke.sh
set -u

BASE_URL="${BASE_URL:-http://localhost:8090}"
: "${TEST_USER:?TEST_USER is required}"
: "${TEST_PASSWORD:?TEST_PASSWORD is required}"

failures=0

check() {  # nom, attendu, obtenu
    if [ "$2" = "$3" ]; then
        echo "PASS  $1"
    else
        echo "FAIL  $1 (attendu $2, obtenu $3)"
        failures=$((failures + 1))
    fi
}

status() { curl -s -o /dev/null -w '%{http_code}' "$@"; }

# Renvoie le corps JSON, puis le code HTTP sur la dernière ligne.
login() {
    jq -n --arg u "$1" --arg p "$2" '{username:$u,password:$p}' \
        | curl -s -w '\n%{http_code}' -X POST "$BASE_URL/api/v1/auth/login" \
            -H 'Content-Type: application/json' -d @-
}

# 1. Login réussi
resp=$(login "$TEST_USER" "$TEST_PASSWORD")
check "login valide -> 200" 200 "${resp##*$'\n'}"
token=$(printf '%s' "${resp%$'\n'*}" | jq -r '.token // empty')
check "login valide renvoie un token" oui "$([ -n "$token" ] && echo oui || echo non)"

# 2. Mauvais mot de passe
resp=$(login "$TEST_USER" "mot-de-passe-incorrect")
check "mauvais mot de passe -> 401" 401 "${resp##*$'\n'}"

# 3. Route protégée sans token, puis avec token
check "route protégée sans token -> 401" 401 "$(status "$BASE_URL/api/v1/services")"
check "route protégée avec token -> 200" 200 \
    "$(status -H "Authorization: Bearer $token" "$BASE_URL/api/v1/services")"

if [ "$failures" -ne 0 ]; then
    echo "$failures test(s) en échec"
    exit 1
fi
echo "Tous les tests passent"