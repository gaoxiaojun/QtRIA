#include "chartmode.h"
#include "chartconstants.h"
#include "barchart.h"

#include <QCoreApplication>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QSignalMapper>
#include <QTextEdit>
#include <qwt_plot_barchart.h>
#include <qwt_plot.h>

#include <coreplugin/outputpane.h>
#include <coreplugin/modemanager.h>

using namespace Chart;
using namespace Chart::Internal;

ChartMode::ChartMode(QObject *parent)
  : Core::IMode(parent) ,
    m_splitter(new Core::MiniSplitter)
{
    setObjectName(QLatin1String("ChartMode"));
    setContext(Core::Context(Constants::C_MODE_CHART));
    setIcon(QIcon(QLatin1String(":/chart/images/mode_chart.png")));
    setDisplayName(QCoreApplication::translate("Chart::Internal::ChartMode", "Chart"));
    setPriority(Constants::P_MODE_CHART);
    setId(Constants::ID_MODE_CHART);

    Core::MiniSplitter *rightPaneSplitter = new Core::MiniSplitter;
    rightPaneSplitter->setOrientation(Qt::Vertical);
    rightPaneSplitter->insertWidget(0, new QTextEdit(QString("Editor1")));
    rightPaneSplitter->insertWidget(1, new QTextEdit(QString("Editor2")));
    rightPaneSplitter->setStretchFactor(0, 1);
    rightPaneSplitter->setStretchFactor(1, 0);

    QMdiArea *chartMDI = new QMdiArea(m_splitter);
    //chartMDI->setViewMode(QMdiArea::TabbedView);
    chartMDI->setObjectName(QLatin1String("ChartMDI"));
    chartMDI->setWindowTitle(tr("ChartMDI"));
    chartMDI->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    chartMDI->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    chartMDI->addSubWindow(new BarChart());
    chartMDI->tileSubWindows();

    m_splitter->insertWidget(0, rightPaneSplitter);
    m_splitter->insertWidget(1, chartMDI);
    m_splitter->setStretchFactor(0, 0);
    m_splitter->setStretchFactor(1, 1);

    //m_splitter->setFocusProxy(Core::EditorManager::instance());

    setWidget(m_splitter);
}

ChartMode::~ChartMode()
{
    delete m_splitter;
}
