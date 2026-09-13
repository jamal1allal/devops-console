#include "ui-components/services_view_model.h"

#include <QMetaObject>
#include <QVariantMap>
#include <QtConcurrent/QtConcurrent>

#include "core/config.h"
#include "core/logger.h"
#include "network/rest_client.h"

namespace devops::ui {

using devops::core::Config;
using devops::core::Logger;

namespace {
QVariantMap statusToVariant(const protocol::ServiceStatus& status) {
    QVariantMap map;
    map["id"] = QString::fromStdString(status.id);
    map["name"] = QString::fromStdString(status.name);
    map["health"] = QString::fromStdString(protocol::toString(status.health));
    map["version"] = status.version ? QString::fromStdString(*status.version) : QString();
    map["lastDeployedAt"] = status.lastDeployedAt ? QString::fromStdString(*status.lastDeployedAt) : QString();
    return map;
}
}  // namespace

ServicesViewModel::ServicesViewModel(QObject* parent) : QObject(parent) {
    const auto baseUrl = Config::instance().get("BACKEND_URL", "http://localhost:8080");
    auto client = std::make_shared<network::RestClient>(baseUrl);
    m_api = std::make_shared<network::ServicesApi>(client);
}

void ServicesViewModel::refresh() {
    setLoading(true);
    setErrorMessage(QString());

    // L'appel réseau (bloquant côté RestClient) tourne sur le thread pool
    // Qt : l'UI thread reste totalement libre pendant ce temps.
    auto future = QtConcurrent::run([api = m_api]() { return api->listServices(); });

    auto* watcher = new QFutureWatcher<std::vector<protocol::ServiceStatus>>(this);
    connect(watcher, &QFutureWatcher<std::vector<protocol::ServiceStatus>>::finished, this, [this, watcher]() {
        try {
            const auto services = watcher->result();
            QVariantList list;
            for (const auto& service : services) {
                list.append(statusToVariant(service));
            }
            m_services = std::move(list);
            emit servicesChanged();
        } catch (const std::exception& e) {
            Logger::instance().error("ServicesViewModel", std::string("refresh failed: ") + e.what());
            setErrorMessage(QString::fromStdString(e.what()));
        }
        setLoading(false);
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}

void ServicesViewModel::setLoading(bool loading) {
    if (m_loading == loading) return;
    m_loading = loading;
    emit loadingChanged();
}

void ServicesViewModel::setErrorMessage(const QString& message) {
    if (m_errorMessage == message) return;
    m_errorMessage = message;
    emit errorMessageChanged();
}

}  // namespace devops::ui
