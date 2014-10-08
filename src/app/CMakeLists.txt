set(CMAKE_INCLUDE_CURRENT_DIR ON)

project(${APPLICATION_NAME})

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(app_version.h.in app_version.h)

set(APP_SOURCES
    app_main.cpp
    crashhandlersetup.cpp
    qtlockedfile.cpp
    qtlocalpeer.cpp
    qtsingleapplication.cpp
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
)

add_executable(${PROJECT_NAME} ${APP_SOURCES})

target_link_libraries(${PROJECT_NAME} ${APP_TARGET_LIBRARIES})

set(CRASH_SOURCES
    crash_main.cpp
    backtracecollector.cpp
    crashhandlerdialog.cpp
    crashhandler.cpp
    utils.cpp
    ../libs/utils/qtcassert.cpp
    ../libs/utils/checkablemessagebox.cpp
    ../libs/utils/environment.cpp
)
set(CRASH_HEADERS
    backtracecollector.h
    crashhandlerdialog.h
    crashhandler.h
    utils.h
    ../libs/utils/qtcassert.h
    ../libs/utils/checkablemessagebox.h
    ../libs/utils/environment.h
)
set(CRASH_FORMS crashhandlerdialog.ui)

qt5_wrap_ui(CREASH_FORMS_HEADERS ${CRASH_FORMS})

list(APPEND CRASH_SOURCES
        ${CRASH_HEADERS}
        ${CREASH_FORMS_HEADERS}
)

set(CRASH_TARGET_LIBRARIES
    Qt5::Core
    Qt5::Widgets
)

add_executable(crash_handler ${CRASH_SOURCES})

target_link_libraries(crash_handler ${CRASH_TARGET_LIBRARIES})

get_filename_component(PARENT_DIR ${CMAKE_CURRENT_BINARY_DIR} DIRECTORY)
set(${PROJECT_NAME}_INCLUDE_DIRS ${PARENT_DIR} CACHE INTERNAL "${PROJECT_NAME}_include" FORCE)

if(APPLE)
    configure_file(Info.plist.in Info.plist)
    add_custom_command(OUTPUT ${APPLICATION_BUNDLE_DIR}/Info.plist
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Info.plist
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/Info.plist ${APPLICATION_BUNDLE_DIR}/Info.plist
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/app.icns ${APPLICATION_DATA_DIR}/${APPLICATION_NAME}.icns
        COMMENT "Copy Application Info.plist"
    )
    add_custom_target(${PROJECT_NAME}_copy_info_plist ALL DEPENDS ${APPLICATION_BUNDLE_DIR}/Info.plist)
endif()
