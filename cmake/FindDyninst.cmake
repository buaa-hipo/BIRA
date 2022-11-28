# - Find the Dyninst library.
# This finds dyninst on the system, and the various dependencies
# that it needs.  Right now the pieces of Dyninst that we find are:
#
# StackwalkerAPI
# SymtabAPI
#
# === Variables ===
# If the various components are found, the following variables will
# be set:
#
# Dyninst_FOUND           TRUE if anything was found
# STACKWALKER_FOUND       TRUE if stackwalker was found
# SYMTAB_FOUND            TRUE if stackwalker was found
#
# Dyninst_INCLUDE_DIR     include directory for dyninst headers
# STACKWALKER_INCLUDE_DIR include directory for stackwalker headers
# SYMTAB_INCLUDE_DIR      include directory for symtab headers
#
# Dyninst_LIBRARIES       libraries for all components that were found.
# STACKWALKER_LIBRARIES   libraries necessary to use stackwalker
# SYMTAB_LIBRARIES        libraries necessary to use symtabAPI
#
# In practice, all the include directories will be the same, as Dyninst
# installs everything in the same prefix.  The LIBRARIES variables are
# the subsets of libraries used for each individual API.
#
# === Usage ===
# If stackwalker and the above libraries are installed in standard
# locations, you don't need to confgure anything.  The user can also
# set the following variables to tell this module where to find things:
#
# ELF_DIR
# DWARF_DIR
# Dyninst_DIR
#
# e.g.:
#
#   cmake -DDyninst_DIR=/path/to/dyninst -DDWARF_DIR=/path/to/dwarf .
#
# And obviously you can edit the *_INCLUDE_DIR and *_LIBRARIES variables
# above in CMake's cache.
#
include(FindPackageHandleStandardArgs)
include(GetPrerequisites)

#
# Utility functions
#
function(find_path_with_hints package header)
  find_path(${package}_INCLUDE_DIR ${header}
    PATH_SUFFIXES include
    HINTS $ENV{${package}_DIR} ${${package}_DIR})
endfunction()

function(find_library_with_hints package lib)
  find_library(${package}_LIBRARY ${lib}
    PATH_SUFFIXES lib lib64
    HINTS $ENV{${package}_DIR} ${${package}_DIR})
endfunction()

#
# Find dyninst and all of its dependencies.
#
find_library(IBERTY_LIBRARY iberty)
if (IBERTY_LIBRARY)
  set(MAYBE_IBERTY ${IBERTY_LIBRARY})
else()
  set(MAYBE_IBERTY "")
endif()

find_path_with_hints(ELF libelf.h)
find_library_with_hints(ELF elf)

find_path_with_hints(DWARF libdwarf.h)
find_library_with_hints(DWARF dwarf)

find_path_with_hints(Dyninst walker.h)
find_library(STACKWALKER_LIBRARY stackwalk
  PATH_SUFFIXES lib lib64
  HINTS $ENV{Dyninst_DIR} ${Dyninst_DIR})
find_library(SYMTAB_LIBRARY symtabAPI
  PATH_SUFFIXES lib lib64
  HINTS $ENV{Dyninst_DIR} ${Dyninst_DIR})
find_library(COMMON_LIBRARY common
  PATH_SUFFIXES lib lib64
  HINTS $ENV{Dyninst_DIR} ${Dyninst_DIR})

#
# Variables to describe what was found by this find module.
#
if (DEFINED STACKWALKER_LIBRARY)
  set(STACKWALKER_FOUND TRUE)
endif()

if (DEFINED SYMTAB_LIBRARY)
  set(SYMTAB_FOUND TRUE)
endif()

if (SYMTAB_FOUND OR STACKWALKER_FOUND)
  set(Dyninst_FOUND TRUE)
endif()

#
# Variables that allow you to link with various components of dyninst
#
set(SYMTAB_LIBRARIES
  ${SYMTAB_LIBRARY} ${DYNINST_COMMON_LIBRARY}
  ${MAYBE_IBERTY} ${DWARF_LIBRARY} ${ELF_LIBRARY})

set(STACKWALKER_LIBRARIES
  ${STACKWALKER_LIBRARY} ${SYMTAB_LIBRARIES})

set(Dyninst_LIBRARIES ${STACKWALKER_LIBRARIES})


