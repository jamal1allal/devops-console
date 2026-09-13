-- Migration 0001 : schéma initial
-- Appliquer avec : psql -U devops -d devops_console -f 0001_init.sql

CREATE TABLE IF NOT EXISTS services (
    id               VARCHAR(64) PRIMARY KEY,
    name             VARCHAR(255) NOT NULL,
    health           VARCHAR(16) NOT NULL DEFAULT 'unknown'
                         CHECK (health IN ('healthy', 'degraded', 'down', 'unknown')),
    version          VARCHAR(64),
    last_deployed_at TIMESTAMPTZ
);

CREATE TABLE IF NOT EXISTS deployment_events (
    id            SERIAL PRIMARY KEY,
    service_id    VARCHAR(64) NOT NULL REFERENCES services(id) ON DELETE CASCADE,
    version       VARCHAR(64) NOT NULL,
    deployed_at   TIMESTAMPTZ NOT NULL DEFAULT now(),
    deployed_by   VARCHAR(255) NOT NULL,
    success       BOOLEAN NOT NULL DEFAULT true
);

CREATE INDEX IF NOT EXISTS idx_deployment_events_service_id ON deployment_events(service_id);

-- Données de départ pour développer sans attendre un vrai pipeline CI
INSERT INTO services (id, name, health, version, last_deployed_at) VALUES
    ('api-gateway', 'API Gateway', 'healthy', '2.3.1', now() - interval '2 days'),
    ('auth-service', 'Auth Service', 'healthy', '1.8.0', now() - interval '5 days'),
    ('billing-worker', 'Billing Worker', 'degraded', '0.9.4', now() - interval '1 day'),
    ('notifications', 'Notifications', 'down', '1.2.0', now() - interval '3 hours')
ON CONFLICT (id) DO NOTHING;
