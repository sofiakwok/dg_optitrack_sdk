#
# Create the dg plugin library.
#

add_library(entities SHARED optitrack_client_entity.cpp)
# Add the include dependencies.
target_include_directories(
  entities PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                             $<INSTALL_INTERFACE:include>)
# Link the dependencies.
target_link_libraries(entities dynamic-graph::dynamic-graph)
target_link_libraries(entities
                      dynamic-graph-python::dynamic-graph-python)
target_link_libraries(entities mocap4r2_optitrack_driver::mocap4r2_optitrack_driver)
# Install the target and it's python bindings.
install_dynamic_graph_plugin_python_bindings(entities)
# Install the plugin.
get_dynamic_graph_plugin_install_path(plugin_install_path)
install(
  TARGETS entities
  EXPORT export_${PROJECT_NAME}
  LIBRARY DESTINATION ${plugin_install_path}
  ARCHIVE DESTINATION ${plugin_install_path}
  RUNTIME DESTINATION ${plugin_install_path}
  INCLUDES
  DESTINATION include)