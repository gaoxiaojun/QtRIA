set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SOURCES
    mainwindow.cpp
    editmode.cpp
    iwizardfactory.cpp
    tabpositionindicator.cpp
    fancyactionbar.cpp
    fancytabwidget.cpp
    generalsettings.cpp
    id.cpp
    icontext.cpp
    messagemanager.cpp
    messageoutputwindow.cpp
    outputpane.cpp
    outputwindow.cpp
    vcsmanager.cpp
    statusbarmanager.cpp
    versiondialog.cpp
    editormanager/editormanager.cpp
    editormanager/editorview.cpp
    editormanager/documentmodel.cpp
    editormanager/openeditorsview.cpp
    editormanager/openeditorswindow.cpp
    editormanager/ieditorfactory.cpp
    #editormanager/iexternaleditor.cpp
    actionmanager/actionmanager.cpp
    actionmanager/command.cpp
    actionmanager/commandbutton.cpp
    actionmanager/actioncontainer.cpp
    actionmanager/commandsfile.cpp
    dialogs/saveitemsdialog.cpp
    dialogs/newdialog.cpp
    dialogs/settingsdialog.cpp
    actionmanager/commandmappings.cpp
    dialogs/shortcutsettings.cpp
    dialogs/readonlyfilesdialog.cpp
    dialogs/openwithdialog.cpp
    progressmanager/progressmanager.cpp
    progressmanager/progressview.cpp
    progressmanager/progressbar.cpp
    progressmanager/futureprogress.cpp
    statusbarwidget.cpp
    coreplugin.cpp
    variablemanager.cpp
    modemanager.cpp
    basefilewizard.cpp
    basefilewizardfactory.cpp
    generatedfile.cpp
    plugindialog.cpp
    inavigationwidgetfactory.cpp
    navigationwidget.cpp
    manhattanstyle.cpp
    minisplitter.cpp
    styleanimator.cpp
    findplaceholder.cpp
    rightpane.cpp
    sidebar.cpp
    fileiconprovider.cpp
    mimedatabase.cpp
    icore.cpp
    infobar.cpp
    editormanager/ieditor.cpp
    dialogs/ioptionspage.cpp
    settingsdatabase.cpp
    imode.cpp
#    editormanager/systemeditor.cpp
    designmode.cpp
    editortoolbar.cpp
    helpmanager.cpp
    outputpanemanager.cpp
    navigationsubwidget.cpp
    sidebarwidget.cpp
    #externaltool.cpp
    #dialogs/externaltoolconfig.cpp
    #toolsettings.cpp
    variablechooser.cpp
    mimetypemagicdialog.cpp
    mimetypesettings.cpp
    dialogs/promptoverwritedialog.cpp
    fileutils.cpp
    featureprovider.cpp
    idocument.cpp
    idocumentfactory.cpp
    textdocument.cpp
    documentmanager.cpp
    removefiledialog.cpp
    iversioncontrol.cpp
    dialogs/addtovcsdialog.cpp
    icorelistener.cpp
    ioutputpane.cpp
    windowsupport.cpp
    locator/locator.cpp
    locator/commandlocator.cpp
    locator/locatorwidget.cpp
    locator/locatorfiltersfilter.cpp
    locator/opendocumentsfilter.cpp
    locator/filesystemfilter.cpp
    locator/settingspage.cpp
    locator/directoryfilter.cpp
    locator/locatormanager.cpp
    locator/basefilefilter.cpp
    locator/ilocatorfilter.cpp
    locator/executefilter.cpp
    locator/locatorsearchutils.cpp
    find/findtoolwindow.cpp
    find/currentdocumentfind.cpp
    find/basetextfind.cpp
    find/findtoolbar.cpp
    find/findplugin.cpp
    find/searchresulttreeitemdelegate.cpp
    find/searchresulttreeitems.cpp
    find/searchresulttreemodel.cpp
    find/searchresulttreeview.cpp
    find/searchresultwindow.cpp
    find/ifindfilter.cpp
    find/ifindsupport.cpp
    find/searchresultwidget.cpp
    find/treeviewfind.cpp
)

set(LIB_HEADERS
    mainwindow.h
    editmode.h
    iwizardfactory.h
    tabpositionindicator.h
    fancyactionbar.h
    fancytabwidget.h
    generalsettings.h
    id.h
    messagemanager.h
    messageoutputwindow.h
    outputpane.h
    outputwindow.h
    vcsmanager.h
    statusbarmanager.h
    editormanager/editormanager.h
    editormanager/editorview.h
    editormanager/documentmodel.h
    editormanager/openeditorsview.h
    editormanager/openeditorswindow.h
    editormanager/ieditor.h
    #editormanager/iexternaleditor.h
    editormanager/ieditorfactory.h
    actionmanager/actioncontainer.h
    actionmanager/actionmanager.h
    actionmanager/command.h
    actionmanager/commandbutton.h
    actionmanager/actionmanager_p.h
    actionmanager/command_p.h
    actionmanager/actioncontainer_p.h
    actionmanager/commandsfile.h
    dialogs/saveitemsdialog.h
    dialogs/newdialog.h
    dialogs/settingsdialog.h
    actionmanager/commandmappings.h
    dialogs/readonlyfilesdialog.h
    dialogs/shortcutsettings.h
    dialogs/openwithdialog.h
    dialogs/ioptionspage.h
    progressmanager/progressmanager_p.h
    progressmanager/progressview.h
    progressmanager/progressbar.h
    progressmanager/futureprogress.h
    progressmanager/progressmanager.h
    icontext.h
    icore.h
    infobar.h
    imode.h
    ioutputpane.h
    coreconstants.h
    iversioncontrol.h
    ifilewizardextension.h
    icorelistener.h
    versiondialog.h
    core_global.h
    statusbarwidget.h
    coreplugin.h
    variablemanager.h
    modemanager.h
    basefilewizard.h
    basefilewizardfactory.h
    generatedfile.h
    plugindialog.h
    inavigationwidgetfactory.h
    navigationwidget.h
    manhattanstyle.h
    minisplitter.h
    styleanimator.h
    findplaceholder.h
    rightpane.h
    sidebar.h
    fileiconprovider.h
    mimedatabase.h
    settingsdatabase.h
#    editormanager/systemeditor.h
    designmode.h
    editortoolbar.h
    helpmanager.h
    outputpanemanager.h
    navigationsubwidget.h
    sidebarwidget.h
    #externaltool.h
    #dialogs/externaltoolconfig.h
    #toolsettings.h
    variablechooser.h
    mimetypemagicdialog.h
    mimetypesettings.h
    dialogs/promptoverwritedialog.h
    fileutils.h
    #externaltoolmanager.h
    generatedfile.h
    featureprovider.h
    idocument.h
    idocumentfactory.h
    textdocument.h
    documentmanager.h
    removefiledialog.h
    dialogs/addtovcsdialog.h
    windowsupport.h
    vcsbaseconstants.h
    locator/locator.h
    locator/commandlocator.h
    locator/locatorwidget.h
    locator/locatorfiltersfilter.h
    locator/settingspage.h
    locator/ilocatorfilter.h
    locator/opendocumentsfilter.h
    locator/filesystemfilter.h
    locator/locatorconstants.h
    locator/directoryfilter.h
    locator/locatormanager.h
    locator/basefilefilter.h
    locator/executefilter.h
    locator/locatorsearchutils.h
    find/findtoolwindow.h
    find/textfindconstants.h
    find/ifindsupport.h
    find/ifindfilter.h
    find/currentdocumentfind.h
    find/basetextfind.h
    find/findtoolbar.h
    find/findplugin.h
    find/searchresultcolor.h
    find/searchresulttreeitemdelegate.h
    find/searchresulttreeitemroles.h
    find/searchresulttreeitems.h
    find/searchresulttreemodel.h
    find/searchresulttreeview.h
    find/searchresultwindow.h
    find/searchresultwidget.h
    find/treeviewfind.h
)
set(LIB_FORMS
    dialogs/newdialog.ui
    dialogs/saveitemsdialog.ui
    dialogs/readonlyfilesdialog.ui
    dialogs/openwithdialog.ui
    generalsettings.ui
    #dialogs/externaltoolconfig.ui
    mimetypesettingspage.ui
    mimetypemagicdialog.ui
    removefiledialog.ui
    dialogs/addtovcsdialog.ui
    locator/settingspage.ui
    locator/filesystemfilter.ui
    locator/directoryfilter.ui
    find/findwidget.ui
    find/finddialog.ui
)

set(LIB_RESOURCES
    core.qrc
    fancyactionbar.qrc
    locator/locator.qrc
    find/find.qrc
)


if(WIN32)
    include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})
    list(APPEND LIB_SOURCES progressmanager/progressmanager_win.cpp)
    find_library(OLE32_LIBRARY ole32)
    find_library(USER32_LIBRARY user32)
    set(EXTRA_LIBRARY  ${USER32_LIBRARY} ${OLE32_LIBRARY})
elseif(APPLE)
    list(APPEND LIB_SOURCES progressmanager/progressmanager_mac.mm)
    find_library(APPKIT_FRAMEWORK AppKit)
    set(EXTRA_LIBRARY  ${APPKIT_FRAMEWORK})
else()
    list(APPEND LIB_SOURCES progressmanager/progressmanager_x11.cpp)
endif()

add_definitions(-DIDE_TEST_DIR="${PROJECT_SOURCE_DIR}")

qt5_wrap_ui(LIB_FORMS_HEADERS ${LIB_FORMS})
qt5_add_resources(LIB_RESOURCES_HEADERS ${LIB_RESOURCES})

list(APPEND LIB_SOURCES
        ${LIB_HEADERS}
        ${LIB_FORMS_HEADERS}
        ${LIB_RESOURCES_HEADERS}
)

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
include_directories(${PARENT_DIR} ${extensionsystem_INCLUDE_DIRS} ${utils_INCLUDE_DIRS})


set(TARGET_LIBRARIES
    ${extensionsystem_LIBRARIES}
    ${utils_LIBRARIES}
    ${EXTRA_LIBRARY}
    Qt5::Widgets
    Qt5::Core
    Qt5::PrintSupport
    Qt5::Sql
    Qt5::Help
)

add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${TARGET_LIBRARIES})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PARENT_DIR} CACHE INTERNAL "${PROJECT_NAME}_include" FORCE)
set(${PROJECT_NAME}_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "${PROJECT_NAME}_library" FORCE)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
