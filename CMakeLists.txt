# Minimum of 3.5 for adding submodule of LIEF because LIEF min is 3.5,
# otherwise 3.11+ for FetchContent command
cmake_minimum_required(VERSION 3.5)

project(codec LANGUAGES CXX)

# User must provide location of Dyninst cmake files either as a cache or
# environment variable
if(NOT Dyninst_DIR)
  if("$ENV{Dyninst_DIR}" STREQUAL "")
    message(
      FATAL_ERROR
        "Dyninst_DIR not found: define as a cache or environment variable")
  else()
    set(_dyninst_dir $ENV{Dyninst_DIR})
  endif()
else()
  set(_dyninst_dir ${Dyninst_DIR})
  set($ENV{Dyninst_DIR} ${_dyninst_dir})
endif()

# Make sure it's an absolute path
if(NOT IS_ABSOLUTE ${_dyninst_dir})
  get_filename_component(_tmp ${_dyninst_dir} ABSOLUTE)
  set(_dyninst_dir ${_tmp})
  unset(_tmp)
endif()

# Save the munged path in the global name
set(Dyninst_DIR ${_dyninst_dir})
unset(_dyninst_dir)

# Use the Dyninst-provided CMake modules
set(CMAKE_MODULE_PATH
    "${Dyninst_DIR}"
    "${Dyninst_DIR}/Modules"
    ${CMAKE_MODULE_PATH})

# Import the Dyninst components
find_package(Dyninst REQUIRED
             COMPONENTS common
                        dyninstAPI
                        instructionAPI
                        parseAPI
			patchAPI
                        symtabAPI
                        common)

# Read the cache generated from building Dyninst
load_cache(${Dyninst_DIR}
           READ_WITH_PREFIX DYNINST_
           Boost_LIBRARIES
           Boost_INCLUDE_DIRS
           Boost_LIBRARY_DIRS
           Boost_DEFINES
           TBB_INCLUDE_DIRS)

# Set default configuration type
if(NOT CMAKE_BUILD_TYPE)
  set(
    CMAKE_BUILD_TYPE RelWithDebInfo
    CACHE
      STRING
      "Choose the build type (None, Debug, Release, RelWithDebInfo, MinSizeRel)"
    FORCE)
endif()

# Set the C and C++ language API and ABI standards
include(LanguageStandards)

# Use LIEF as an embedded/vendored project
# ========================================

# LIEF build config. Set the default options for LIEF's project setup
option(LIEF_DOC "Build LIEF docs" OFF)
option(LIEF_PYTHON_API "Build LIEF Python API" OFF)
option(LIEF_EXAMPLES "Build LIEF examples" OFF)
option(LIEF_TESTS "Build LIEF tests" OFF)

if(MSVC)
  set(LIEF_USE_CRT_RELEASE "MT" CACHE STRING "LIEF CRT option")
endif()


# If you have LIEF as a submodule in a directory, then you can add it to this
# project with ``add_subdirectory``
# NOTE: This submodule does not exist for this example, but it does the same
# thing as FetchContent without the download part
set(vendorLIEF_submodule_dir "${CMAKE_CURRENT_LIST_DIR}/LIEF")
if(EXISTS "${vendorLIEF_submodule_dir}")
  add_subdirectory("${vendorLIEF_submodule_dir}")

# Else, we'll specify how to obtain LIEF another way (downloading)
else()
  # CMake 3.11 has FetchContent which provides a simple experience for what
  # we're trying to do
  cmake_minimum_required(VERSION 3.11)

  # URL of the LIEF repo (Can be your fork)
  set(LIEF_GIT_URL "https://github.com/lief-project/LIEF.git")

  # LIEF's version to be used (can be 'master')
  set(LIEF_VERSION 0.12.0)

  include(FetchContent)
  FetchContent_Declare(LIEF
    GIT_REPOSITORY  "${LIEF_GIT_URL}"
    GIT_TAG         ${LIEF_VERSION}
    # You may specify an existing LIEF source directory if you don't want to
    # download. Just comment out the above ``GIT_*`` commands and uncoment the
    # following ``SOURCE_DIR`` line
    #SOURCE_DIR      "${CMAKE_CURRENT_LIST_DIR}/../../.."
    )

  if(${CMAKE_VERSION} VERSION_LESS "3.14.0")
    # CMake 3.11 to 3.13 needs more verbose method to make LIEF available
    FetchContent_GetProperties(LIEF)
    if(NOT LIEF_POPULATED)
      FetchContent_Populate(LIEF)
      add_subdirectory(${LIEF_SOURCE_DIR} ${LIEF_BINARY_DIR})
    endif()
  else()
    # CMake 3.14+ has single function to make LIEF available (recommended)
    FetchContent_MakeAvailable(LIEF)
  endif()
endif()


# Add our executable
# ==================
add_compile_options(-fpermissive)
add_executable(codec my_codec.cpp)

if(MSVC)
  set_property(TARGET codec PROPERTY LINK_FLAGS /NODEFAULTLIB:MSVCRT)
endif()

# Enable C++11
set_property(TARGET codec
             PROPERTY CXX_STANDARD           11
             PROPERTY CXX_STANDARD_REQUIRED  ON)

target_include_directories(codec PRIVATE ${DYNINST_INCLUDE_DIR} ${DYNINST_Boost_INCLUDE_DIRS} ${DYNINST_TBB_INCLUDE_DIRS})
# Link the executable with LIEF
target_link_libraries(codec PUBLIC LIEF::LIEF dyninstAPI symtabAPI common ${DYNINST_Boost_LIBRARIES})

install(TARGETS codec DESTINATION bin)