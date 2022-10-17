#ifndef UI_PLAYING_FIELD_H
#define UI_PLAYING_FIELD_H

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <QDebug>

#include "style_helper.h"

QT_BEGIN_NAMESPACE

namespace InSearchOfTreasure {

constexpr  uint8_t kLevels = 9; // количество уровней в игре

constexpr QSize kIconSizeLevel[kLevels] {
    QSize(70, 70),QSize(67, 67),QSize(64, 64),
    QSize(61, 61),QSize(58, 58),QSize(55, 55),
    QSize(52, 52),QSize(49, 49),QSize(46, 46),
};

class Ui_PlayingField {
public:
    Ui_PlayingField() = default;
    Ui_PlayingField(Ui_PlayingField const &other) = delete;
    Ui_PlayingField(Ui_PlayingField &&other) = delete;
    auto operator=(Ui_PlayingField const &other) = delete;
    auto operator=(Ui_PlayingField &&other) = delete;

    auto setupUi(QWidget *Widget, uint8_t level_game) -> void {

//        Widget->setStyleSheet(StyleHelper::getBackgroundWidget());

        uint8_t rows_field{};
        uint8_t columns_field{};
        if (level_game <= kLevels) {
            rows_field = 2 * level_game + 1;
            columns_field = 2 * level_game + 1;
        } else {
            // выставляем уровень по умолчанию
            rows_field = 2 * 2 + 1;
            columns_field = 2 * 2 + 1;
            level_game = 2;
        }

        horisontal_layout = new QHBoxLayout();
        horisontal_layout->setSpacing(9);
        horisontal_layout->setContentsMargins(9, 9, 9, 9);
        //
        button_under_field = new QButtonGroup(horisontal_layout);
        for (uint8_t index = 0; index < rows_field; ++index) {
            if (index % 2 == 0) {
                QPushButton *button = new QPushButton;
                //
                QSizePolicy size_policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                size_policy.setHorizontalStretch(0);
                size_policy.setVerticalStretch(0);
                size_policy.setHeightForWidth(button->sizePolicy().hasHeightForWidth());
                button->setSizePolicy(size_policy);
                button->setMinimumSize(kIconSizeLevel[level_game - 1]);
                button->setMaximumSize(kIconSizeLevel[level_game - 1]);
//                button->setIconSize(kIconSizeLevel[level_game - 1]);
//                button->setStyleSheet(StyleHelper::getButtonStyleUnderGameField());
                //
                button_under_field->addButton(button);
                horisontal_layout->addWidget(button);
            } else {
                QSpacerItem *h_spacer = new QSpacerItem(70, 20, QSizePolicy::Expanding, QSizePolicy::Expanding);
                horisontal_layout->addSpacerItem(h_spacer);
            }
        }
        //
        grid_layout = new QGridLayout();
        grid_layout->setSpacing(9);
        grid_layout->setContentsMargins(9, 9, 9, 9);
        //
        button_playing_field = new QButtonGroup(grid_layout);
        for (uint8_t row = 0, id_button = 0; row < rows_field; ++row) {
            for (uint8_t column = 0; column < columns_field; ++column, ++id_button) {
                QPushButton *button = new QPushButton;
                //
                QSizePolicy size_policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                size_policy.setHorizontalStretch(0);
                size_policy.setVerticalStretch(0);
                size_policy.setHeightForWidth(button->sizePolicy().hasHeightForWidth());
                //
                button->setSizePolicy(size_policy);
                button->setMinimumSize(kIconSizeLevel[level_game - 1]);
                button->setMaximumSize(kIconSizeLevel[level_game - 1]);
//                button->setIconSize(kIconSizeLevel[level_game - 1]);
//                button->setStyleSheet(StyleHelper::getButtonStyleUnderGameField());
                button->setCursor(QCursor(Qt::OpenHandCursor));
                //
                button_playing_field->addButton(button, id_button);
                //
                grid_layout->addWidget(button, row, column, 1, 1);
            }
        }
        //
        vertical_layout = new QVBoxLayout(Widget);
        vertical_layout->setSpacing(9);
        vertical_layout->setContentsMargins(9, 9, 9, 9);
        //
        vertical_layout->addLayout(horisontal_layout);
        vertical_layout->addLayout(grid_layout);
        //
        Widget->setLayout(vertical_layout);
        Widget->setMinimumSize(vertical_layout->totalMinimumSize());
        Widget->setMaximumSize(vertical_layout->totalMinimumSize());
    } // setupUi

public:
    QHBoxLayout *horisontal_layout;
    QButtonGroup *button_under_field;
    QGridLayout *grid_layout;
    QButtonGroup *button_playing_field;
    QVBoxLayout *vertical_layout;
};
}
QT_END_NAMESPACE

#endif // UI_PLAYING_FIELD_H
