#include "chartplugin.h"
#include "chartmode.h"
#include "chartconstants.h"
#include "generalsettingspage.h"

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
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>

namespace Chart {
namespace Internal {


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

    /*// Register the action with the action manager
    Core::Command *command =
            Core::ActionManager::registerAction(
                    sayAction, "Chart.ChartAction", context);

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
    toolsMenu->addMenu(chartMenu);*/

    setupChartMenu();

    addAutoReleasedObject(m_generalSettingsPage = new GeneralSettingsPage());

    m_mode = new ChartMode;
    addAutoReleasedObject(m_mode);

    return true;
}

void ChartPlugin::setupChartMenu()
{
    Core::ActionContainer *menuBar = Core::ActionManager::actionContainer(Core::Constants::MENU_BAR);
    Core::ActionContainer *chartMenu = Core::ActionManager::createMenu(Constants::M_CHART);

    chartMenu->menu()->setTitle(tr("&Chart"));
    chartMenu->menu()->setEnabled(true);
    chartMenu->appendGroup(Constants::G_CHART_VIEWS);
    Core::Context globalcontext(Core::Constants::C_GLOBAL);

    // Create an action to be triggered by a menu entry
    QAction *sayAction = new QAction(tr("Say \"&Chart World!\""), this);
    connect(sayAction, SIGNAL(triggered()), SLOT(sayChart()));

    // Register the action with the action manager
    Core::Command *command = Core::ActionManager::registerAction(sayAction, "Chart.ChartAction", globalcontext);

    // Add the Chart World action command to the menu
    chartMenu->addAction(command, Constants::G_CHART_VIEWS);

    Core::ActionContainer *mhelp = Core::ActionManager::actionContainer(Core::Constants::M_HELP);
    menuBar->addMenu(mhelp, chartMenu);
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
    //Core::ModeManager::activateMode(m_mode->id());

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
