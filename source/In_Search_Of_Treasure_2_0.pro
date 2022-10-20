QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    view/menu_game.cpp


HEADERS += \
    controller/controller_game.hpp \
    model/abstract_cell.hpp \
    model/block_cell.hpp \
    model/chip_cell.hpp \
    model/constans.hpp \
    model/free_cell.hpp \
    model/game_mechanics.hpp \
    model/playing_field.hpp \
    view/menu_game.h \
    view/message_dialog.h \
    view/playing_field_game.h \
    view/style_helper.h \
    view/ui/ui_playing_field_game.h

FORMS += \
    view/ui/menu_game_ui \
    view/ui/message_dialog.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

DISTFILES += \
    logo_game.png.png

ICON += logo_game.png
