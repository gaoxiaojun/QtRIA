#ifndef CHARTCONSTANTS_H
#define CHARTCONSTANTS_H

#include <QtGlobal>
#include <QLatin1String>

namespace Chart {
namespace Constants {

const int  P_MODE_CHART    = 1000;
const char ID_MODE_CHART  [] = "Chart";
const char CHART_CATEGORY[] = "C.Chart";
const char CHART_CATEGORY_ICON[] = ":/chart/images/category_chart.png";
const char CHART_TR_CATEGORY[] = QT_TRANSLATE_NOOP("Chart", "Chart");

const char C_MODE_CHART   [] = "Chart Mode";
const char C_CHART_SIDEBAR[] = "Chart Sidebar";

const char CONTEXT_CHART[] = "Chart.Context";

const char M_CHART[]              = "QtCreator.Group.Edit.Chart";
const char M_CHART_PANES[]        = "QRCP.Menu.Chart.Panes";
const char M_CHART_VIEWS[]        = "QRCP.Menu.Chart.Views";

const char G_CHART[]              = "QRCP.Group.Chart";
const char G_CHART_SIZE[]         = "QRCP.Group.Chart.Size";
const char G_CHART_PANES[]        = "QRCP.Group.Chart.Panes";
const char G_CHART_VIEWS[]        = "QRCP.Group.Chart.Views";
const char G_CHART_SPLIT[]        = "QRCP.Group.Chart.Split";
const char G_CHART_NAVIGATE[]     = "QRCP.Group.Chart.Navigate";
const char G_CHART_OTHER[]        = "QRCP.Group.Chart.Other";

} // Constants
} // Chart

#endif // CHARTCONSTANTS_H
