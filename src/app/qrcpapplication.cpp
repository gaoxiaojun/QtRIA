#include "qrcpapplication.h"

#include <QtDebug>

QRcpApplication::QRcpApplication(const QString &id, int &argc, char **argv)
    : SharedTools::QtSingleApplication(id, argc, argv),
      m_versionDialog(0)
{
}

void QRcpApplication::showAboutApplication(QWidget *parent)
{
    if (!m_versionDialog) {
        m_versionDialog = new VersionDialog(parent);
        connect(m_versionDialog, SIGNAL(finished(int)),
                this, SLOT(destroyVersionDialog()));
    }
    m_versionDialog->show();
}

void QRcpApplication::destroyVersionDialog()
{
    if (m_versionDialog) {
        m_versionDialog->deleteLater();
        m_versionDialog = 0;
    }
}
