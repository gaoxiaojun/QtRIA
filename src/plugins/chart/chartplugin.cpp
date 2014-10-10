#include "chartplugin.h"

#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/icore.h>
#include <coreplugin/imode.h>
#include <coreplugin/modemanager.h>
#include <coreplugin/id.h>

#include <QDebug>
#include <QtPlugin>
#include <QAction>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>

namespace Chart {
namespace Internal {

/*!  A mode with a push button based on BaseMode.  */

class ChartMode : public Core::IMode
{
public:
    ChartMode()
    {
        setWidget(new QPushButton(tr("Chart World PushButton!")));
        setContext(Core::Context("Chart.MainView"));
        setDisplayName(tr("Chart world!"));
        setIcon(QIcon());
        setPriority(0);
        setId("Chart.ChartMode");
        setContextHelpId(QString());
    }
};


/*! Constructs the Chart World plugin. Normally plugins don't do anything in
    their constructor except for initializing their member variables. The
    actual work is done later, in the initialize() and extensionsInitialized()
    functions.
*/
ChartPlugin::ChartPlugin()
{
}

/*! Plugins are responsible for deleting objects they created on the heap, and
    to unregister objects from the plugin manager that they registered there.
*/
ChartPlugin::~ChartPlugin()
{
}

/*! Initializes the plugin. Returns true on success.
    Plugins want to register objects with the plugin manager here.

    \a errorMessage can be used to pass an error message to the plugin system,
       if there was any.
*/
bool ChartPlugin::initialize(const QStringList &arguments, QString *errorMessage)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorMessage)

    // Create a unique context for our own view, that will be used for the
    // menu entry later.
    Core::Context context("Chart.MainView");

    // Create an action to be triggered by a menu entry
    QAction *chartAction = new QAction(tr("Say \"&Chart World!\""), this);
    connect(chartAction, SIGNAL(triggered()), SLOT(sayChart()));

    // Register the action with the action manager
    Core::Command *command =
            Core::ActionManager::registerAction(
                    chartAction, "Chart.ChartAction", context);

    // Create our own menu to place in the Tools menu
    Core::ActionContainer *chartMenu =
            Core::ActionManager::createMenu("Chart.ChartMenu");
    QMenu *menu = chartMenu->menu();
    menu->setTitle(tr("&Chart World"));
    menu->setEnabled(true);

    // Add the Chart World action command to the menu
    chartMenu->addAction(command);

    // Request the Tools menu and add the Chart World menu to it
    Core::ActionContainer *toolsMenu =
            Core::ActionManager::actionContainer(Core::Constants::M_TOOLS);
    toolsMenu->addMenu(chartMenu);

    // Add a mode with a push button based on BaseMode. Like the BaseView,
    // it will unregister itself from the plugin manager when it is deleted.
    Core::IMode *chartMode = new ChartMode;
    addAutoReleasedObject(chartMode);

    return true;
}

/*! Notification that all extensions that this plugin depends on have been
    initialized. The dependencies are defined in the plugins .pluginspec file.

    Normally this function is used for things that rely on other plugins to have
    added objects to the plugin manager, that implement interfaces that we're
    interested in. These objects can now be requested through the
    PluginManagerInterface.

    The ChartPlugin doesn't need things from other plugins, so it does
    nothing here.
*/
void ChartPlugin::extensionsInitialized()
{
}

void ChartPlugin::sayChart()
{
    // When passing 0 for the parent, the message box becomes an
    // application-global modal dialog box
    QMessageBox::information(
            0, tr("Chart World!"), tr("Chart World! Beautiful day today, isn't it?"));
}

} // namespace Internal
} // namespace Chart

Q_EXPORT_PLUGIN(Chart::Internal::ChartPlugin)
