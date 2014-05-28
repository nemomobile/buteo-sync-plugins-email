/* Copyright (C) 2013 - 2014 Jolla Ltd.
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

#include "syncemailclient.h"
// QMF
#include <qmailnamespace.h>
// buteo-syncfw
#include <ProfileEngineDefs.h>
#include <QDebug>

extern "C" SyncEmailClient* createPlugin(const QString& pluginName,
                                         const Buteo::SyncProfile& profile,
                                         Buteo::PluginCbInterface *cbInterface)
{
    return new SyncEmailClient(pluginName, profile, cbInterface);
}

extern "C" void destroyPlugin(SyncEmailClient *client)
{
    delete client;
}

SyncEmailClient::SyncEmailClient(const QString& pluginName,
                                 const Buteo::SyncProfile& profile,
                                 Buteo::PluginCbInterface *cbInterface) :
    ClientPlugin(pluginName, profile, cbInterface)
{
}

SyncEmailClient::~SyncEmailClient()
{
}

bool SyncEmailClient::init()
{
    m_accountId = QMailAccountId(profile().key(Buteo::KEY_ACCOUNT_ID).toInt());
    if (!m_accountId.isValid()) {
        qWarning() << Q_FUNC_INFO << "Invalid email account, ID: " << m_accountId.toULongLong();
        return false;
    }

    int id = QMail::fileLock("messageserver-instance.lock");
    if (id == -1) {
        // Server is currently running
        m_emailAgent = new EmailAgent(this);
        m_emailAgent->setBackgroundProcess(true);
        connect(m_emailAgent, SIGNAL(synchronizingChanged(EmailAgent::Status)), this, SLOT(syncStatusChanged(EmailAgent::Status)));
        return true;
    } else {
        QMail::fileUnlock(id);
        qWarning() << Q_FUNC_INFO << "Abording scheduled sync, IPC failure";
        return false;
    }
}

bool SyncEmailClient::uninit()
{
    disconnect(m_emailAgent, SIGNAL(synchronizingChanged(EmailAgent::Status)), this, SLOT(syncStatusChanged(EmailAgent::Status)));
    delete m_emailAgent;
    m_emailAgent = 0;
    return true;
}

bool SyncEmailClient::startSync()
{
    m_emailAgent->syncAccounts(QMailAccountIdList() << m_accountId);
    qDebug() << Q_FUNC_INFO << "Starting scheduled sync for email account: " << m_accountId.toULongLong();
    return true;
}

void SyncEmailClient::abortSync(Sync::SyncStatus status)
{
    //TODO: check if there's a use case for this (some place in the UI ?)
    Q_UNUSED(status)
}

Buteo::SyncResults SyncEmailClient::getSyncResults() const
{
    return m_syncResults;
}

bool SyncEmailClient::cleanUp()
{
    return true;
}

void SyncEmailClient::connectivityStateChanged(Sync::ConnectivityType, bool)
{
    // TODO
}

void SyncEmailClient::syncStatusChanged(EmailAgent::Status status)
{
    // TODO: Do we need to care about various status here ?
    // if so status info needs to be added to EmailAgent
    if (!m_emailAgent->synchronizing()) {
        if (status == EmailAgent::Completed) {
            updateResults(Buteo::SyncResults(QDateTime::currentDateTime(), Buteo::SyncResults::SYNC_RESULT_SUCCESS, Buteo::SyncResults::NO_ERROR));
            emit success(getProfileName(), "Sync completed");
        } else if (status == EmailAgent::Error) {
            updateResults(Buteo::SyncResults(QDateTime::currentDateTime(), Buteo::SyncResults::SYNC_RESULT_FAILED, Buteo::SyncResults::ABORTED));
            emit error(getProfileName(), "Sync failed", Buteo::SyncResults::SYNC_RESULT_FAILED);
        }
    }
}

void SyncEmailClient::updateResults(const Buteo::SyncResults &results)
{
    m_syncResults = results;
    m_syncResults.setScheduled(true);
}
