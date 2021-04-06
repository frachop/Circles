TEMPLATE = app
QT += core gui widgets
TARGET = Circles

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

# Input
INCLUDEPATH += src/
HEADERS += src/*.hpp
FORMS += forms/*.ui
SOURCES += src/*.cpp

# Output
OBJECTS_DIR = build/qtgenerated
UI_DIR = build/qtgenerated
MOC_DIR= build/qtgenerated
RCC_DIR= build/qtgenerated

