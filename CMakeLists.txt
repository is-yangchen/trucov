##############################################################################
# CMake Build Rules for trucov                                               #
##############################################################################
#  Permission is hereby granted, free of charge, to any person               #
#  obtaining a copy of this software and associated documentation            #
#  files (the "Software"), to deal in the Software without                   #
#  restriction, including without limitation the rights to use,              #
#  copy, modify, merge, publish, distribute, sublicense, and#or sell         #
#  copies of the Software, and to permit persons to whom the                 #
#  Software is furnished to do so, subject to the following                  #
#  conditions:                                                               #
#                                                                            #
#  The above copyright notice and this permission notice shall be            #
#  included in all copies or substantial portions of the Software.           #
#                                                                            #
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,           #
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES           #
#  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND                  #
#  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT               #
#  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,              #
#  WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING              #
#  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR             #
#  OTHER DEALINGS IN THE SOFTWARE.                                           #
##############################################################################
#  COPYRIGHT (c) 2009 Schweitzer Engineering Laboratories, Pullman, WA       #
##############################################################################
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )

##############################################################################
# Set Default Install Path                                                   #
# Specifying this at the command line will override these settings           #
# Note: This line cannot be moved below, otherwise the command line will not #
#       override this setting                                                # 
##############################################################################
#SET( CMAKE_INSTALL_PREFIX /usr )

## Project Name and Executable Name.
SET( TRUCOV_EXECUTABLE_NAME "trucov")
PROJECT( TRUCOV_EXECUTABLE_NAME )

##############################################################################
# Version Information.                                                       #
##############################################################################
SET( TRUCOV_VERSION_YEAR 10 )
SET( TRUCOV_VERSION_REV  1 )
SET( TRUCOV_VERSION_BUG   )
IF ( NOT TRUCOV_VERSION_BUG )
   SET( TRUCOV_VERSION "${TRUCOV_VERSION_YEAR}.${TRUCOV_VERSION_REV}" )
ELSE()
   SET( TRUCOV_VERSION "${TRUCOV_VERSION_YEAR}.${TRUCOV_VERSION_REV}.${TRUCOV_VERSION_BUG}" )
ENDIF()

MESSAGE( "Welcome to Trucov ${TRUCOV_VERSION}" )

## Define Version
ADD_DEFINITIONS( "-DVERSION=\"${TRUCOV_VERSION}\"" )

## Set default build type to Release
IF ( NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE Release CACHE STRING
        "Choose the type of build, options are: None Debug Release."
        FORCE)
ENDIF ( NOT CMAKE_BUILD_TYPE )

MESSAGE ( "Compiling with Build Type = " ${CMAKE_BUILD_TYPE} )
MESSAGE ( "  1) To compile in Release mode: cmake -DCMAKE_BUILD_TYPE=Release" )
MESSAGE ( "  2) To compile in Debug mode:   cmake -DCMAKE_BUILD_TYPE=Debug" )

IF ( NOT DEFINED COVERAGE )
   SET ( COVERAGE "Off" )
ENDIF ()

IF ( DEFINED COVERAGE AND ${COVERAGE} STREQUAL "On" )
   MESSAGE ( "Coverage Profiling = Enabled" )
   ADD_DEFINITIONS( "-fprofile-arcs -ftest-coverage" )
ELSE ()
   MESSAGE ( "Coverage Profiling = Disabled" )
ENDIF ()
MESSAGE ( "  1) To compile with coverage profiling:    cmake -DCOVERAGE=On" )
MESSAGE ( "  2) To compile without coverage profiling: cmake -DCOVERAGE=Off" )

IF ( NOT DEFINED DEBUGFLAG )
   SET ( DEBUGFLAG "On" )
ENDIF ()

IF ( ${DEBUGFLAG} STREQUAL "Off" )
   MESSAGE ( "Compiling with debug option disabled." )
ELSE ()
   MESSAGE ( "Compiling with debug option enabled." )
   ADD_DEFINITIONS( "-DDEBUGFLAG" )
ENDIF ()
MESSAGE ( "  1) To compile with debug option enabled:  cmake -DDEBUGFLAG=On" )
MESSAGE ( "  2) To compile with debug option disabled: cmake -DDEBUGFLAG=Off" )

## Add Debugging flags
IF ( CMAKE_BUILD_TYPE STREQUAL "Debug" )
   ADD_DEFINITIONS( "-DDEBUG" )
ENDIF ()
IF ( CMAKE_BUILD_TYPE STREQUAL "Release" )
   add_definitions( ${RELEASE_BUILD_FUNCTION} )  
ENDIF ()

## Added for boost version
ADD_DEFINITIONS("-DNEW_SPIRIT_VERSION=103600")

IF ( NOT DEFINED BOOST_VERSION )
   SET ( BOOST_VERSION "New" )
ENDIF ()

IF ( ${BOOST_VERSION} STREQUAL "Old" )
   MESSAGE ( "Assuming older version of boost." )
ELSE ()
   MESSAGE ( "Assuming newer version of boost." )
ENDIF ()
MESSAGE ( "  1) To compile with newer versions of boost: cmake -DBOOST_VERSION=New" )
MESSAGE ( "  2) To compile with older versions of boost: cmake -DBOOST_VERSION=Old" )
MESSAGE ( "  If boost fails to link, attempt to compile with the older version of boost." )

##############################################################################
# Trucov Compilation.                                                        #
##############################################################################

## Paths to code files.
SET( TRUCOV_INCLUDE_DIRECTORIES "include" ) 
SET( TRUCOV_SRC src/ )

## The name of the Trucov library.
SET( TRUCOV_LIBS trucov_lib )

## All static libraries needed to link boost.
IF ( BOOST_VERSION STREQUAL "Old" ) 
   SET( BOOST_LIBS 
       boost_program_options 
       boost_filesystem
       boost_thread
       boost_regex )
ELSE ()
   SET( BOOST_LIBS 
       boost_program_options-mt 
       boost_filesystem-mt
       boost_thread-mt
       boost_regex-mt ) 
ENDIF ()

## Linking used for gcov.
SET ( GCOV_LIBS gcov )

## Build trucov code as a library.
ADD_LIBRARY( ${TRUCOV_LIBS} 
    ${TRUCOV_SRC}/config.cpp 
    ${TRUCOV_SRC}/coverage_creator.cpp 
    ${TRUCOV_SRC}/parser.cpp 
    ${TRUCOV_SRC}/record.cpp 
    ${TRUCOV_SRC}/tru_utility.cpp 
    ${TRUCOV_SRC}/command.cpp
    ${TRUCOV_SRC}/selector.cpp
    ${TRUCOV_SRC}/dot_creator.cpp
    ${TRUCOV_SRC}/parser_builder.cpp
    ${TRUCOV_SRC}/revision_script_manager.cpp
)

## Create Trucov Executable.
ADD_EXECUTABLE( ${TRUCOV_EXECUTABLE_NAME} ${TRUCOV_SRC}/main.cpp )
TARGET_LINK_LIBRARIES( ${TRUCOV_EXECUTABLE_NAME} ${TRUCOV_LIBS} ${BOOST_LIBS} ${GCOV_LIBS} )
INCLUDE_DIRECTORIES( ${TRUCOV_INCLUDE_DIRECTORIES} )

##############################################################################
# Trucov Testing.                                                            #
##############################################################################

## Test code paths.
SET( TEST_TARGET test )
SET( GTEST_SRC test/gtest/ )
SET( UT_SRC test/unit_test )
SET( UT_LIST 
   selector_unittest 
   revision_script_manager_unittest
   gcno_grammar_unittest
   gcda_grammar_unittest
   record_unittest
   prims_unittest
   gcno_actors_unittest  
   gcda_actors_unittest
   utility_unittest
   config_unittest
   command_unittest 
   parser_builder_unittest
   parser_unittest
   coverage_creator_unittest
   dot_creator_unittest ) 

## Set testing library and testing dependencies.
SET( TEST_LIBS gtest_main ${TRUCOV_LIBS} ${BOOST_LIBS} ${GCOV_LIBS} )
SET( TEST_DEPENDENCIES ${TRUCOV_LIBS} gtest_main )

## Create gtest libraries. 
ADD_LIBRARY( gtest_all EXCLUDE_FROM_ALL
    ${GTEST_SRC}/gtest-all.cc )
ADD_LIBRARY( gtest_main EXCLUDE_FROM_ALL
    ${GTEST_SRC}/gtest_main.cc ${GTEST_SRC}/gtest-all.cc )

## Create all unit test executables. 
FOREACH (PROGRAM ${UT_LIST})
   ADD_EXECUTABLE( ${PROGRAM} EXCLUDE_FROM_ALL
      ${UT_SRC}/${PROGRAM}.cpp ) 
   TARGET_LINK_LIBRARIES( ${PROGRAM} ${TEST_LIBS} )
   ADD_DEPENDENCIES( ${PROGRAM} ${TEST_DEPENDENCIES} )
ENDFOREACH (PROGRAM)

## Test all tests.
SET( ALL_UT all_unittest )
ADD_EXECUTABLE( ${ALL_UT} EXCLUDE_FROM_ALL 
    ${UT_SRC}/all_unittest.cpp )
TARGET_LINK_LIBRARIES( ${ALL_UT} ${TEST_LIBS} )
ADD_DEPENDENCIES( ${ALL_UT} ${TEST_DEPENDENCIES} )

## Create functional tests.
SET( FUNCTIONAL_SRC test/functional_test )
SET( FUNC_LIST 
   helper_functionaltest 
   list_status_functionaltest
   report_functionaltest 
   dot_functionaltest
   dot_report_functionaltest
   graph_functionaltest
   graph_report_functionaltest
   all_report_functionaltest
   cmd_line_options_functionaltest
   config_options_functionaltest
   cmd_config_override_functionaltest
   project_setup_functionaltest
   parse_behavior_functionaltest 
   all_functionaltest )

FOREACH (PROGRAM ${FUNC_LIST})
   ADD_EXECUTABLE( ${PROGRAM} EXCLUDE_FROM_ALL
      ${FUNCTIONAL_SRC}/${PROGRAM}.cpp ) 
   TARGET_LINK_LIBRARIES( ${PROGRAM} ${TEST_LIBS} )
   ADD_DEPENDENCIES( ${PROGRAM} ${TEST_DEPENDENCIES} ${TRUCOV_EXECUTABLE_NAME} )
ENDFOREACH (PROGRAM)

## Add test target.
ADD_CUSTOM_TARGET( unittest )
ADD_DEPENDENCIES( unittest ${UT_LIST} ${ALL_UT} )

ADD_CUSTOM_TARGET( functionaltest )
ADD_DEPENDENCIES( functionaltest ${FUNC_LIST} )

ADD_CUSTOM_TARGET( ${TEST_TARGET} )
ADD_DEPENDENCIES( ${TEST_TARGET} unittest functionaltest ) 

##############################################################################
# Installation.                                                              #
##############################################################################
SET ( TRUCOV_RC_FILE "~/.trucovrc" )
SET ( SEL_TRUCOV_RC config/sel_trucovrc)
SET ( DEFAULT_TRUCOV_RC config/default_trucovrc )

## Install executable.
INSTALL( TARGETS ${TRUCOV_EXECUTABLE_NAME}
         DESTINATION bin
         PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE 
                     WORLD_READ WORLD_EXECUTE )

## Install config file.
INSTALL( FILES config/default_trucovrc config/sel_trucovrc
         DESTINATION share/trucov
         PERMISSIONS OWNER_READ GROUP_READ WORLD_READ )

