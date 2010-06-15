CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(qtruov)
 
SET(CMAKE_BUILD_TYPE Release)
 
SET(CMAKE_CXX_FLAGS "-Wall")
 
# QT4 Handling
FIND_PACKAGE(Qt4 COMPONENTS QtOpengl QtSvg REQUIRED)
 
INCLUDE(${QT_USE_FILE})
 
FILE( GLOB qtrucov_MOC_files  *.h )
QT4_WRAP_CPP( qtrucov_MOC_Source
   codeeditor.h
   highlighter.h
   mainwindow.h
   proj_dia.h
   secretgui.h
   svgview.h
   svgwindow.h
   ui_mainwindow.h
)

QT4_WRAP_UI( qtrucov_UI_Source
   mainwindow.ui
   proj_dia.ui
)
 
INCLUDE_DIRECTORIES( . )
 
ADD_EXECUTABLE( qtrucov
   codeeditor.cpp
   highlighter.cpp
   main.cpp
   mainwindow.cpp
   proj_dia.cpp
   svgview.cpp
   svgwindow.cpp
	${qtrucov_MOC_Source}
	${qtrucov_UI_Source}
)

TARGET_LINK_LIBRARIES( qtrucov
   ${QT_LIBRARIES}
   )

INSTALL( TARGEST qtrucov )
