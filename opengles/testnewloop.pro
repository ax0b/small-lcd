QT += core

CONFIG += c++11
QMAKE_CXXFLAGS += -fpermissive

TARGET = opengles
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    qtdirectapplication.cpp \
    qtdirectscreen.cpp \
    qtdirectupdateevent.cpp \
    qtdirectinitevent.cpp \
    myscreen.cpp \
    textureloader.cpp

INCLUDEPATH += /opt/vc/include \
	/opt/vc/include/interface/vmcs_host/linux \
	/opt/vc/src/hello_pi/libs/ \
	/opt/vc/src/hello_pi/libs/vgfont
	

HEADERS += \
    qtdirectapplication.h \
    qtdirectscreen.h \
    qtdirectupdateevent.h \
    qtdirectinitevent.h \
    myscreen.h \
    triangle.h \
    textureloader.h

LIBS +=-L/opt/vc/lib/ -lbrcmGLESv2 -lbrcmEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread \
-lrt -lm -L/opt/vc/src/hello_pi/libs/ilclient -L/opt/vc/src/hello_pi/libs/vgfont
