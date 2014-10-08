set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(utils)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SOURCES
    environment.cpp
    environmentmodel.cpp
    qtcprocess.cpp
    reloadpromptutils.cpp
    settingsselector.cpp
    stringutils.cpp
    filesearch.cpp
    pathchooser.cpp
    pathlisteditor.cpp
    wizard.cpp
    filewizardpage.cpp
    filesystemwatcher.cpp
    projectintropage.cpp
    filenamevalidatinglineedit.cpp
    projectnamevalidatinglineedit.cpp
    codegeneration.cpp
    newclasswidget.cpp
    classnamevalidatinglineedit.cpp
    linecolumnlabel.cpp
    fancylineedit.cpp
    qtcolorbutton.cpp
    savedaction.cpp
    synchronousprocess.cpp
    savefile.cpp
    fileutils.cpp
    textfileformat.cpp
    consoleprocess.cpp
    uncommentselection.cpp
    parameteraction.cpp
    headerviewstretcher.cpp
    checkablemessagebox.cpp
    styledbar.cpp
    stylehelper.cpp
    iwelcomepage.cpp
    fancymainwindow.cpp
    detailsbutton.cpp
    detailswidget.cpp
    changeset.cpp
    faketooltip.cpp
    htmldocextractor.cpp
    navigationtreeview.cpp
    crumblepath.cpp
    historycompleter.cpp
    buildablehelperlibrary.cpp
    annotateditemdelegate.cpp
    fileinprojectfinder.cpp
    ipaddresslineedit.cpp
    statuslabel.cpp
    outputformatter.cpp
    flowlayout.cpp
    networkaccessmanager.cpp
    persistentsettings.cpp
    completingtextedit.cpp
    json.cpp
    portlist.cpp
    tcpportsgatherer.cpp
    appmainwindow.cpp
    sleep.cpp
    basetreeview.cpp
    qtcassert.cpp
    elfreader.cpp
    bracematcher.cpp
    proxyaction.cpp
    elidinglabel.cpp
    hostosinfo.cpp
    tooltip/tooltip.cpp
    tooltip/tips.cpp
    tooltip/tipcontents.cpp
    unixutils.cpp
    ansiescapecodehandler.cpp
    execmenu.cpp
    completinglineedit.cpp
    winutils.cpp
    itemviews.cpp
    treeviewcombobox.cpp
    proxycredentialsdialog.cpp
)

if(WIN32)
   list(APPEND LIB_SOURCES consoleprocess_win.cpp)
else()
   list(APPEND LIB_SOURCES consoleprocess_unix.cpp)
endif()

file(GLOB LIB_HEADERS *.h)
file(GLOB LIB_FORMS *.ui)
file(GLOB LIB_RESOURCES *.qrc)

qt5_wrap_ui(LIB_FORMS_HEADERS ${LIB_FORMS})
qt5_add_resources(LIB_RESOURCES_HEADERS ${LIB_RESOURCES})

list(APPEND LIB_SOURCES
        ${LIB_HEADERS}
        ${LIB_FORMS_HEADERS}
        ${LIB_RESOURCES_HEADERS}
)

include_directories(${aggregation_INCLUDE_DIRS})

set(TARGET_LIBRARIES
    ${aggregation_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::Script
    Qt5::Concurrent
)


add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})
add_library(${PROJECT_NAME}_static STATIC ${LIB_SOURCES})

target_link_libraries(${PROJECT_NAME} ${TARGET_LIBRARIES})
target_link_libraries(${PROJECT_NAME}_static ${TARGET_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${PROJECT_NAME}_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(${PROJECT_NAME}_static PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
set(${PROJECT_NAME}_INCLUDE_DIRS ${PARENT_DIR} CACHE INTERNAL "${PROJECT_NAME}_include" FORCE)
set(${PROJECT_NAME}_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "${PROJECT_NAME}_library" FORCE)
set(${PROJECT_NAME}_STATIC_LIBRARIES ${PROJECT_NAME}_static CACHE INTERNAL "${PROJECT_NAME}_static_library" FORCE)

install(TARGETS ${PROJECT_NAME} ${PROJECT_NAME}_static
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
    ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
