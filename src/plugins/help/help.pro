QT += network
greaterThan(QT_MAJOR_VERSION, 4) {
    QT += printsupport help
    !isEmpty(QT.webkitwidgets.name): QT += webkitwidgets webkit
    else: DEFINES += QT_NO_WEBKIT
} else {
    CONFIG += help
    contains(QT_CONFIG, webkit): QT += webkit
}

INCLUDEPATH += $$PWD

include(../../qtcreatorplugin.pri)

DEFINES += \
    QT_CLUCENE_SUPPORT \
    HELP_LIBRARY

HEADERS += \
    centralwidget.h \
    docsettingspage.h \
    filtersettingspage.h \
    generalsettingspage.h \
    helpconstants.h \
    helpfindsupport.h \
    helpindexfilter.h \
    localhelpmanager.h \
    helpmode.h \
    helpplugin.h \
    helpviewer.h \
    openpagesmanager.h \
    openpagesmodel.h \
    openpagesswitcher.h \
    openpageswidget.h \
    remotehelpfilter.h \
    searchwidget.h \
    xbelsupport.h \
    qtwebkithelpviewer.h \
    textbrowserhelpviewer.h \
    helpwidget.h

SOURCES += \
    centralwidget.cpp \
    docsettingspage.cpp \
    filtersettingspage.cpp \
    generalsettingspage.cpp \
    helpfindsupport.cpp \
    helpindexfilter.cpp \
    localhelpmanager.cpp \
    helpmode.cpp \
    helpplugin.cpp \
    helpviewer.cpp \
    openpagesmanager.cpp \
    openpagesmodel.cpp \
    openpagesswitcher.cpp \
    openpageswidget.cpp \
    remotehelpfilter.cpp \
    searchwidget.cpp \
    xbelsupport.cpp \
    qtwebkithelpviewer.cpp \
    textbrowserhelpviewer.cpp \
    helpwidget.cpp

FORMS += docsettingspage.ui \
    filtersettingspage.ui \
    generalsettingspage.ui \
    remotehelpfilter.ui

macx:minQtVersion(5, 2, 0) {
    DEFINES += QTC_MAC_NATIVE_HELPVIEWER
    QT += macextras
    HEADERS += macwebkithelpviewer.h
    OBJECTIVE_SOURCES += macwebkithelpviewer.mm

    LIBS += -framework WebKit -framework AppKit
}

RESOURCES += help.qrc
include(../../shared/help/help.pri)
