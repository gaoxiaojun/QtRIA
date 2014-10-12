set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(Help)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SOURCES
    bookmarkmanager.cpp
    filtersettingspage.cpp
    helpplugin.cpp
    openpagesmanager.cpp
    remotehelpfilter.cpp
    centralwidget.cpp
    generalsettingspage.cpp
    helpviewer.cpp
    openpagesmodel.cpp
    searchwidget.cpp
    contentwindow.cpp
    helpfindsupport.cpp
    helpwidget.cpp
    openpagesswitcher.cpp
    textbrowserhelpviewer.cpp
    docsettingspage.cpp
    helpindexfilter.cpp
    indexwindow.cpp
    openpageswidget.cpp
    topicchooser.cpp
    filternamedialog.cpp
    helpmode.cpp
    localhelpmanager.cpp
    qtwebkithelpviewer.cpp
    xbelsupport.cpp
)

set(LIB_HEADERS
    bookmarkmanager.h
    filternamedialog.h
    helpfindsupport.h
    helpviewer.h
    macwebkithelpviewer.h
    openpageswidget.h
    textbrowserhelpviewer.h
    centralwidget.h
    filtersettingspage.h
    helpindexfilter.h
    helpwidget.h
    openpagesmanager.h
    qtwebkithelpviewer.h
    topicchooser.h
    contentwindow.h
    generalsettingspage.h
    helpmode.h
    indexwindow.h
    openpagesmodel.h
    remotehelpfilter.h
    xbelsupport.h
    docsettingspage.h
    helpconstants.h
    helpplugin.h
    localhelpmanager.h
    openpagesswitcher.h
    searchwidget.h
)
set(LIB_FORMS
    bookmarkdialog.ui
    docsettingspage.ui
    filternamedialog.ui
    filtersettingspage.ui
    generalsettingspage.ui
    remotehelpfilter.ui
    topicchooser.ui
)

set(LIB_RESOURCES
    help.qrc
)

qt5_wrap_ui(LIB_FORMS_HEADERS ${LIB_FORMS})
qt5_add_resources(LIB_RESOURCES_HEADERS ${LIB_RESOURCES})

list(APPEND LIB_SOURCES
        ${LIB_HEADERS}
        ${LIB_FORMS_HEADERS}
        ${LIB_RESOURCES_HEADERS}
)

if(APPLE)
    list(APPEND LIB_SOURCES macwebkithelpviewer.mm)
    list(APPEND LIB_HEADERS macwebkithelpviewer.h)
    find_library(APPKIT_FRAMEWORK AppKit)
    find_library(WEBKIT_FRAMEWORK WebKit)
    set(EXTRA_LIBRARY  ${APPKIT_FRAMEWORK} ${WEBKIT_FRAMEWORK})
endif()
get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
include_directories(${PARENT_DIR} ${extensionsystem_INCLUDE_DIRS} ${utils_INCLUDE_DIRS} ${Core_INCLUDE_DIRS})


set(TARGET_LIBRARIES
    ${Core_LIBRARIES}
    ${extensionsystem_LIBRARIES}
    ${utils_LIBRARIES}
    ${EXTRA_LIBRARY}
    Qt5::Widgets
    Qt5::Core
    Qt5::WebKit
    Qt5::WebKitWidgets
)

if(APPLE)
  list(APPEND TARGET_LIBRARIES
       Qt5::MacExtras
  )
endif()

add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${TARGET_LIBRARIES})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PARENT_DIR} CACHE INTERNAL "${PROJECT_NAME}_include" FORCE)
set(${PROJECT_NAME}_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "${PROJECT_NAME}_library" FORCE)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
