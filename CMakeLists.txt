cmake_minimum_required( VERSION 2.8.12 )

project( tdeeasyprojector )
set( VERSION 0.1 )

# include needed modules
include( FindPkgConfig          )
include( CheckFunctionExists    )
include( CheckSymbolExists      )
include( CheckIncludeFile       )
include( CheckLibraryExists     )
include( CheckCSourceCompiles   )
include( CheckCXXSourceCompiles )

set( CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules" )
include( TDEMacros )
include( TDESetupPaths )
tde_setup_paths( )

# configure checks
include( ConfigureChecks.cmake )

# compiler flags
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TQT_CXX_FLAGS}" )
set( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined" )
set( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -Wl,--no-undefined" )

option( BUILD_ALL          "Build all"          ON )
option( BUILD_DOC          "Build doc"          ${BUILD_ALL} )
option( BUILD_TRANSLATIONS "Build translations" ${BUILD_ALL} )

tde_conditional_add_project_docs( BUILD_DOC )
tde_conditional_add_project_translations( BUILD_TRANSLATIONS )

add_subdirectory( src )
add_subdirectory( data )
