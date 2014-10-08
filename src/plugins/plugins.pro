include(../../qforex.pri)

TEMPLATE  = subdirs

SUBDIRS   = \
    coreplugin \
    texteditor \
    welcome \
    help

isEmpty(IDE_PACKAGE_MODE) {
    SUBDIRS += \
        helloworld #\
        #updateinfo
#} else:!isEmpty(UPDATEINFO_ENABLE) {
#    SUBDIRS += \
#        updateinfo
}


for(p, SUBDIRS) {
    QTC_PLUGIN_DEPENDS =
    include($$p/$${p}_dependencies.pri)
    pv = $${p}.depends
    $$pv = $$QTC_PLUGIN_DEPENDS
}
