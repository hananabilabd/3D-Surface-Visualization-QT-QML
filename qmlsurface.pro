!include( ../examples.pri ) {
    error( "Couldn't find the examples.pri file!" )
}
QT +=widgets
QT += quickcontrols2
# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp  \
    model.cpp 

RESOURCES += qmlsurface.qrc

OTHER_FILES += qml/qmlsurface/*

HEADERS += \
    model.h 
