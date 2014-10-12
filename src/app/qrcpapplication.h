#ifndef QRCPAPPLICATION_H
#define QRCPAPPLICATION_H

#include <qglobal.h>
#include "versiondialog.h"

#include "qtsingleapplication.h"

QT_BEGIN_NAMESPACE
class QWidget;
QT_END_NAMESPACE


class QRcpApplication : public SharedTools::QtSingleApplication
{
    Q_OBJECT
public:
    QRcpApplication(const QString &id, int &argc, char **argv);

signals:

public slots:
    void showAboutApplication(QWidget *parent);
    void destroyVersionDialog();

private:
    VersionDialog *m_versionDialog;
};

#endif // QRCPAPPLICATION_H
