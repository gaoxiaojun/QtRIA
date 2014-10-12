set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(${APPLICATION_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APPLICATION_ICON qrcp.icns)
configure_file(app_version.h.in app_version.h)

set(APP_SOURCES
    app_main.cpp
    crashhandlersetup.cpp
    qtlockedfile.cpp
    qtlocalpeer.cpp
    qtsingleapplication.cpp
)

set(APP_RESOURCES
    app.qrc
)

qt5_add_resources(APP_RESOURCES_HEADERS ${APP_RESOURCES})
list(APPEND APP_SOURCES
        ${APP_RESOURCES_HEADERS}
)

if(WIN32)
   list(APPEND APP_SOURCES qtlockedfile_win.cpp)
else()
   list(APPEND APP_SOURCES qtlockedfile_unix.cpp)
endif()

include_directories(
    ${utils_INCLUDE_DIRS}
    ${extensionsystem_INCLUDE_DIRS}
)

set(APP_TARGET_LIBRARIES
    ${extensionsystem_LIBRARIES}
    ${utils_LIBRARYIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

target_link_libraries(${PROJECT_NAME} ${APP_TARGET_LIBRARIES})

if(APPLE)
    configure_file(Info.plist.in Info.plist)
    add_custom_command(OUTPUT ${APPLICATION_BUNDLE_DIR}/Info.plist
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Info.plist ${APPLICATION_BUNDLE_DIR}/Info.plist
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/app.icns ${APPLICATION_DATA_DIR}/${APPLICATION_ICON}
        COMMENT "Copy Application Info.plist"
    )
    add_custom_target(${PROJECT_NAME}_copy_info_plist ALL DEPENDS ${APPLICATION_BUNDLE_DIR}/Info.plist)
endif()
