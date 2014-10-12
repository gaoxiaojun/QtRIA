#ifndef DASHBOARDPLUGIN_H
#define DASHBOARDPLUGIN_H

#include <extensionsystem/iplugin.h>

QT_BEGIN_NAMESPACE
class QQmlEngine;
QT_END_NAMESPACE

namespace DashBoard {
namespace Internal {

class DashBoardMode;

class DashBoardPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "DashBoard.json")

public:
    DashBoardPlugin();

    virtual bool initialize(const QStringList &arguments, QString *errorMessage);
    virtual void extensionsInitialized();

private:
    DashBoardMode *m_dashboardMode;
};

} // namespace DashBoard
} // namespace Internal

#endif // DASHBOARDPLUGIN_H
