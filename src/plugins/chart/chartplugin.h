#ifndef CHARTPLUGIN_H
#define CHARTPLUGIN_H

#include "chartmode.h"

#include <extensionsystem/iplugin.h>

namespace Core {
class IMode;
class MiniSplitter;
class SideBar;
class SideBarItem;
}   // Core

namespace Chart {
namespace Internal {

class GeneralSettingsPage;

class ChartPlugin
  : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "Chart.json")

public:
    ChartPlugin();
    ~ChartPlugin();

    bool initialize(const QStringList &arguments, QString *errorMessage);

    void extensionsInitialized();

private slots:
    void sayChart();

private:
    GeneralSettingsPage *m_generalSettingsPage;
    ChartMode *m_mode;
};

} // namespace Internal
} // namespace Chart

#endif // CHARTPLUGIN_H
