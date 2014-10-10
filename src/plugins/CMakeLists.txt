set(JSON_XSL ${CMAKE_CURRENT_SOURCE_DIR}/qtcreatorplugin2json.xsl CACHE INTERNAL "json_xsl" FORCE)

macro(add_plugin plugin_dir plugin_name)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${plugin_dir}/${plugin_name}.pluginspec.in ${CMAKE_CURRENT_BINARY_DIR}/${plugin_dir}/${plugin_name}.pluginspec)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${plugin_dir}/${plugin_name}.json
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${plugin_dir}/${plugin_name}.pluginspec
        COMMAND ${QT_XMLPATTERNS_EXECUTABLE} -no-format -output ${CMAKE_CURRENT_BINARY_DIR}/${plugin_dir}/${plugin_name}.json ${JSON_XSL} ${CMAKE_CURRENT_BINARY_DIR}/${plugin_dir}/${plugin_name}.pluginspec
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/${plugin_dir}/${plugin_name}.pluginspec ${APPLICATION_PLUGINS_DIR}/${plugin_name}.pluginspec
        COMMENT "Generator ${plugin_name}.json"
    )
    add_custom_target(${plugin_name}_generator_json ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${plugin_dir}/${plugin_name}.json)
    add_subdirectory(${plugin_dir})
endmacro()

add_plugin(coreplugin Core)
add_plugin(texteditor TextEditor)
#add_plugin(help Help)
add_plugin(chart Chart)
