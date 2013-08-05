/* Copyright (C) 2013 Jolla Ltd.
 *
 * Contributors: Valerio Valerio <valerio.valerio@jollamobile.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * version 2.1 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef SYNCEMAILCLIENT_H
#define SYNCEMAILCLIENT_H

#include <QNetworkReply>

#include <ClientPlugin.h>
#include <SyncResults.h>
#include <emailagent.h>

class SyncEmailClient : Buteo::ClientPlugin
{
    Q_OBJECT

public:
    SyncEmailClient(const QString& pluginName,
                  const Buteo::SyncProfile& profile,
                  Buteo::PluginCbInterface *cbInterface);

    virtual ~SyncEmailClient();
    virtual bool init();
    virtual bool uninit();
    virtual bool startSync();
    virtual void abortSync(Sync::SyncStatus status = Sync::SYNC_ABORTED);
    virtual Buteo::SyncResults getSyncResults() const;
    virtual bool cleanUp();

public slots:
    virtual void connectivityStateChanged(Sync::ConnectivityType type, bool state);

private slots:
    void syncStatusChanged(EmailAgent::Status status);

private:
    Buteo::SyncResults m_syncResults;
    EmailAgent *m_emailAgent;

    void updateResults(const Buteo::SyncResults &results);
};

extern "C" SyncEmailClient* createPlugin(const QString& pluginName,
                                       const Buteo::SyncProfile& profile,
                                       Buteo::PluginCbInterface *cbInterface);

extern "C" void destroyPlugin(SyncEmailClient *client);

#endif // SYNCEMAILCLIENT_H
