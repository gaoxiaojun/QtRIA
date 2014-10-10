#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include <QWidget>

namespace Chart {
namespace Internal {

class ChartWindow : public QWidget
{
    Q_OBJECT

public:
    ChartWindow(QWidget *parent = 0);
};

} // namespace Internal
} // namespace Chart

#endif // CHARTWINDOW_H
