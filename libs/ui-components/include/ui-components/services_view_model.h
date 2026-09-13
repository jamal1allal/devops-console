#pragma once

#include <QObject>
#include <QVariantList>
#include <memory>
#include <qqmlintegration.h>

#include "network/services_api.h"

namespace devops::ui {

/// ViewModel MVVM exposé à QML sous le nom `ServicesViewModel`.
/// Toute la logique d'appel réseau vit ici, pas dans le QML : le QML
/// se contente de binder sur `services` et d'appeler `refresh()`.
class ServicesViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVariantList services READ services NOTIFY servicesChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
    explicit ServicesViewModel(QObject* parent = nullptr);

    QVariantList services() const { return m_services; }
    bool loading() const { return m_loading; }
    QString errorMessage() const { return m_errorMessage; }

public slots:
    /// Déclenche un appel réseau asynchrone (thread pool Qt) pour ne
    /// jamais bloquer l'UI, puis met à jour les propriétés sur le thread
    /// principal via QMetaObject::invokeMethod.
    void refresh();

signals:
    void servicesChanged();
    void loadingChanged();
    void errorMessageChanged();

private:
    void setLoading(bool loading);
    void setErrorMessage(const QString& message);

    std::shared_ptr<network::ServicesApi> m_api;
    QVariantList m_services;
    bool m_loading = false;
    QString m_errorMessage;
};

}  // namespace devops::ui
