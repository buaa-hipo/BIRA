cmake_minimum_required(VERSION 3.7)

include(../../make/policies.cmake NO_POLICY_SCOPE)

find_package(PAPI REQUIRED)

option(SHOW_RESULTS
  "Display client results in pop-up (Windows) or console message (Linux)"
  ${SHOW_RESULTS_DEFAULT})
if (SHOW_RESULTS)
  add_definitions(-DSHOW_RESULTS)
endif (SHOW_RESULTS)

if (UNIX)
  add_compile_options(-Wno-deprecated)
  add_compile_options(-Wno-unused-result)
  add_compile_options(-Wno-unused-variable)
  add_compile_options(-Wno-unused-local-typedefs)
  add_compile_options(-Wno-unused-function)
  add_compile_options(-Werror=sign-compare)
  add_compile_options(-Werror=narrowing)
  add_compile_options(-std=c++11)

  add_compile_options(-march=native)
  add_compile_options(-O3)

  if (DEBUG)
    add_compile_options(-g3)
  endif (DEBUG)
endif (UNIX)

# add third-party libraries

add_library(dr_func_wrap SHARED
  dr_func_wrap.cpp
  pmu_collector.cpp
  )

configure_DynamoRIO_client(dr_func_wrap)
use_DynamoRIO_extension(dr_func_wrap droption) 
use_DynamoRIO_extension(dr_func_wrap drsyms) 
use_DynamoRIO_extension(dr_func_wrap drmgr) 
use_DynamoRIO_extension(dr_func_wrap drwrap) 
# use_DynamoRIO_extension(dr_func_wrap drx) 
place_shared_lib_in_lib_dir(dr_func_wrap)

set(HEADER ./include/pmu_collector.h ./include/record_type.h ./include/bira_log.h ./include/tsc_timer.h)
target_include_directories(dr_func_wrap PRIVATE ./include ${PAPI_INCLUDE_DIRS})
target_link_libraries(dr_func_wrap ${PAPI_LIBRARIES})
add_dependencies(dr_func_wrap api_headers)

# Provide a hint for how to use the client
if (NOT DynamoRIO_INTERNAL OR NOT "${CMAKE_GENERATOR}" MATCHES "Ninja")
  add_custom_command(TARGET dr_func_wrap
    POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    ARGS -E echo "Usage: pass to drconfig or drrun: -t dr_func_wrap"
    VERBATIM)
endif ()

install_target(dr_func_wrap ${INSTALL_CLIENTS_LIB})

set(INSTALL_CCTTEST_CONFIG ${INSTALL_CLIENTS_BASE})

function (write_config_file dst bindir libdir)
  file(WRITE  ${dst} "# dr_func_wrap tool config file\n")
  file(APPEND ${dst} "CLIENT_REL=${libdir}/${LIB_PFX}dr_func_wrap${LIB_EXT}\n")
endfunction ()

if (X64)
  set(CONFIG_INSTALL ${PROJECT_BINARY_DIR}/dr_func_wrap.drrun64)
  set(CONFIG_BUILD ${PROJECT_BINARY_DIR}/tools/dr_func_wrap.drrun64)
else (X64)
  set(CONFIG_INSTALL ${PROJECT_BINARY_DIR}/dr_func_wrap.drrun32)
  set(CONFIG_BUILD ${PROJECT_BINARY_DIR}/tools/dr_func_wrap.drrun32)
endif (X64)

set(BUILD_CLIENTS_BIN clients/${INSTALL_BIN})
set(BUILD_CLIENTS_LIB clients/${INSTALL_LIB})

write_config_file(${CONFIG_INSTALL} ${INSTALL_CLIENTS_BIN} ${INSTALL_CLIENTS_LIB})
write_config_file(${CONFIG_BUILD} ${BUILD_CLIENTS_BIN} ${BUILD_CLIENTS_LIB})

DR_install(FILES "${CONFIG_INSTALL}" DESTINATION ${INSTALL_CCTTEST_CONFIG})
register_tool_file("dr_func_wrap")

