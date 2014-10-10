#include "chartwindow.h"

#include <QTextEdit>
#include <QVBoxLayout>

using namespace Chart::Internal;

ChartWindow::ChartWindow(QWidget *parent)
   : QWidget(parent)
{
    QBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(new QTextEdit(tr("Focus me to activate my context!")));
    setWindowTitle(tr("Chart"));
}
