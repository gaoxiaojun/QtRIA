#ifndef DASHBOARDCONSTANTS_H
#define DASHBOARDCONSTANTS_H

#include <QtGlobal>
#include <QLatin1String>

namespace DashBoard {
namespace Constants {

const int  P_MODE_DASHBOARD    = 1000;
const char ID_MODE_DASHBOARD  [] = "DashBoard";
const char DASHBOARD_CATEGORY[] = "C.DashBoard";
const char DASHBOARD_CATEGORY_ICON[] = ":/dashboard/images/category_dashboard.png";
const char DASHBOARD_TR_CATEGORY[] = QT_TRANSLATE_NOOP("DashBoard", "DashBoard");

const char C_MODE_DASHBOARD   [] = "DashBoard Mode";
const char C_DASHBOARD_SIDEBAR[] = "DashBoard Sidebar";

const char CONTEXT_DASHBOARD[] = "DashBoard.Context";

const char M_DASHBOARD[]              = "QtCreator.Group.Edit.DashBoard";
const char M_DASHBOARD_PANES[]        = "QRCP.Menu.DashBoard.Panes";
const char M_DASHBOARD_VIEWS[]        = "QRCP.Menu.DashBoard.Views";

const char G_DASHBOARD[]              = "QRCP.Group.DashBoard";
const char G_DASHBOARD_SIZE[]         = "QRCP.Group.DashBoard.Size";
const char G_DASHBOARD_PANES[]        = "QRCP.Group.DashBoard.Panes";
const char G_DASHBOARD_VIEWS[]        = "QRCP.Group.DashBoard.Views";
const char G_DASHBOARD_SPLIT[]        = "QRCP.Group.DashBoard.Split";
const char G_DASHBOARD_NAVIGATE[]     = "QRCP.Group.DashBoard.Navigate";
const char G_DASHBOARD_OTHER[]        = "QRCP.Group.DashBoard.Other";

} // Constants
} // DashBoard

#endif // DASHBOARDCONSTANTS_H
