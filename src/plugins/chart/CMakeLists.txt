set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(Chart)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SOURCES
	chartplugin.cpp
        chartwindow.cpp
        chartmode.cpp
        barchart.cpp
	generalsettingspage.cpp
)

set(LIB_HEADERS
	chartconstants.h
	chartplugin.h
	chartwindow.h
        chartmode.h
        barchart.h
	generalsettingspage.h
)
set(LIB_FORMS
	generalsettingspage.ui
)

set(LIB_RESOURCES
    chart.qrc
)

qt5_wrap_ui(LIB_FORMS_HEADERS ${LIB_FORMS})
qt5_add_resources(LIB_RESOURCES_HEADERS ${LIB_RESOURCES})

list(APPEND LIB_SOURCES
        ${LIB_HEADERS}
        ${LIB_FORMS_HEADERS}
        ${LIB_RESOURCES_HEADERS}
)

include_directories(${extensionsystem_INCLUDE_DIRS} ${utils_INCLUDE_DIRS} ${qwt_INCLUDE_DIRS} ${Core_INCLUDE_DIRS})


set(TARGET_LIBRARIES
    ${Core_LIBRARIES}
    ${extensionsystem_LIBRARIES}
    ${utils_LIBRARIES}
    ${qwt_LIBRARIES}
    ${EXTRA_LIBRARY}
    Qt5::Widgets
    Qt5::Core
)


add_library(${PROJECT_NAME} SHARED ${LIB_SOURCES})
target_link_libraries(${PROJECT_NAME} ${TARGET_LIBRARIES})

set(${PROJECT_NAME}_INCLUDE_DIRS ${PARENT_DIR} CACHE INTERNAL "${PROJECT_NAME}_include" FORCE)
set(${PROJECT_NAME}_LIBRARIES ${PROJECT_NAME} CACHE INTERNAL "${PROJECT_NAME}_library" FORCE)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)
