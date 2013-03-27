TEMPLATE = app
CONFIG += qt thread warn_off release 

TARGET = pathwayeditor
DESTDIR = ./
#INCLUDEPATH += ../lpsolve
#LIBS += -L../lpsolve  -llpsolve55

MOC_DIR 	= ./tmp/
UI_DIR   =  ./tmp/
OBJECTS_DIR = ./tmp/

RC_FILE = gui.rc
RESOURCES +=  gui.qrc

HEADERS += 	stable.h edge.h	graphwidget.h  mainwindow.h  node.h  pathwaywidget.h tabledockwidget.h numeric_treewidgetitem.h 
SOURCES +=  edge.cpp  graphwidget.cpp  main.cpp  mainwindow.cpp  node.cpp  pathwaywidget.cpp tabledockwidget.cpp
