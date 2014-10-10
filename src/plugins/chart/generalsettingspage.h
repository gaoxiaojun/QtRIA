#ifndef GENERALSETTINGSPAGE_H
#define GENERALSETTINGSPAGE_H

#include "ui_generalsettingspage.h"
#include <coreplugin/dialogs/ioptionspage.h>

#include <QPointer>

namespace Chart {
namespace Internal {

class CentralWidget;

class GeneralSettingsPage : public Core::IOptionsPage
{
    Q_OBJECT

public:
    GeneralSettingsPage();

    QWidget *widget();
    void apply();
    void finish();

private:

    QPointer<QWidget> m_widget;
    Ui::GeneralSettingsPage *m_ui;
};

    }   // Internal
}   // Chart

#endif  // GENERALSETTINGSPAGE_H
