QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    view_game.cpp

HEADERS += \
    constans.h \
    controller_game.hpp \
    message_dialog.h \
    model_game.hpp \
    style_helper.h \
    ui_playing_field.h \
    view_game.h \
    view_playing_field.h

FORMS += \
    message_dialog.ui \
    view_game.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
    application_icon.png

ICON += application_icon.png
