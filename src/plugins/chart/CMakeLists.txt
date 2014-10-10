set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(Chart)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(LIB_SOURCES
	chartplugin.cpp
        chartwindow.cpp
)

set(LIB_HEADERS
	chartplugin.h
	chartwindow.h
)
set(LIB_FORMS
)

set(LIB_RESOURCES
)

qt5_wrap_ui(LIB_FORMS_HEADERS ${LIB_FORMS})
qt5_add_resources(LIB_RESOURCES_HEADERS ${LIB_RESOURCES})

list(APPEND LIB_SOURCES
        ${LIB_HEADERS}
        ${LIB_FORMS_HEADERS}
        ${LIB_RESOURCES_HEADERS}
)

get_filename_component(PARENT_DIR ${PROJECT_SOURCE_DIR} DIRECTORY)
include_directories(${PARENT_DIR} ${${APPLICATION_NAME}_INCLUDE_DIRS} ${extensionsystem_INCLUDE_DIRS} ${utils_INCLUDE_DIRS} ${Core_INCLUDE_DIRS})


set(TARGET_LIBRARIES
    ${Core_LIBRARIES}
    ${extensionsystem_LIBRARIES}
    ${utils_LIBRARIES}
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