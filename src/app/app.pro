include(../../qforex.pri)
include(../shared/qtsingleapplication/qtsingleapplication.pri)

TEMPLATE = app
CONFIG += qtc_runnable
TARGET = $$IDE_APP_TARGET
DESTDIR = $$IDE_APP_PATH

HEADERS += crashhandlersetup.h
SOURCES += app_main.cpp crashhandlersetup.cpp

include(../rpath.pri)

LIBS *= -l$$qtLibraryName(ExtensionSystem) -l$$qtLibraryName(Aggregation) -l$$qtLibraryName(Utils)

QT_BREAKPAD_ROOT_PATH = $$(QT_BREAKPAD_ROOT_PATH)
!isEmpty(QT_BREAKPAD_ROOT_PATH) {
    include($$QT_BREAKPAD_ROOT_PATH/qtbreakpad.pri)
}
win32 {
    RC_FILE = qforex.rc
    target.path = $$QTC_PREFIX/bin
    INSTALLS += target
} else:macx {
    LIBS += -framework CoreFoundation
    ICON = qforex.icns
    FILETYPES.files = profile.icns prifile.icns
    FILETYPES.path = Contents/Resources
    QMAKE_BUNDLE_DATA += FILETYPES
    info.input = Info.plist.in
    info.output = $$IDE_BIN_PATH/../Info.plist
    QMAKE_SUBSTITUTES = info
} else {
    target.path  = $$QTC_PREFIX/bin
    INSTALLS    += target
}

OTHER_FILES += qforex.rc \
    Info.plist.in \
    $$PWD/app_version.h.in

QMAKE_SUBSTITUTES += $$PWD/app_version.h.in

CONFIG += no_batch
