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

} // Constants
} // Chart

#endif // CHARTCONSTANTS_H
