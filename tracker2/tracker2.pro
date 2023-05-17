QT       += core gui

QT +=sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    playermainwindow.cpp \
    playerslider.cpp \
    recordthread.cpp \
    videothread.cpp \
    widget.cpp \
    yolo.cpp

HEADERS += \
    mainwindow.h \
    playermainwindow.h \
    playerslider.h \
    recordthread.h \
    videothread.h \
    widget.h \
    yolo.h

FORMS += \
    mainwindow.ui \
    playermainwindow.ui \
    widget.ui


#INCLUDEPATH += C:\OpenCV\opencv\op_build\install\include

#LIBS += C:\OpenCV\opencv\op_build\lib\libopencv_*.a
INCLUDEPATH += C:\OpenCV\build2\install\include
DEPENDPATH += C:\OpenCV\build2\install\include
LIBS +=
Debug:{
    LIBS += C:\OpenCV\build\lib\Debug\opencv_world454d.lib
}
Release:{
    LIBS +=C:\OpenCV\build\install\x64\vc16\lib\opencv_world454.lib
}


LIBS +=$$PWD/libs/libcomponents.a
INCLUDEPATH += $$PWD/libs/Include

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
QT += multimedia multimediawidgets

RESOURCES += \
    Resources.qrc

DISTFILES +=


