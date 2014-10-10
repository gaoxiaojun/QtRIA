#ifndef CHARTMODE_H
#define CHARTMODE_H

#include <coreplugin/imode.h>
#include <coreplugin/minisplitter.h>

#include <QString>
#include <QIcon>
#include <QVBoxLayout>
#include <QMdiSubWindow>

namespace Chart {
namespace Internal {

class ChartMode : public Core::IMode
{
public:
    explicit ChartMode(QObject *parent = 0);
    ~ChartMode();

private:
    QSplitter *m_splitter;
};

} // namespace Internal
} // namespace Chart

#endif // CHARTMODE_H
