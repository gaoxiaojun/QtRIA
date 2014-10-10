#ifndef CHARTPLUGIN_H
#define CHARTPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace Chart {
namespace Internal {

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
};

} // namespace Internal
} // namespace Chart

#endif // CHARTPLUGIN_H
