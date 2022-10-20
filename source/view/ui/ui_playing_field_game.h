#ifndef UI_PLAYING_FIELD_H
#define UI_PLAYING_FIELD_H

#include <QDebug>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include "../../model/constans.hpp"
#include "../style_helper.h"

QT_BEGIN_NAMESPACE

namespace InSearchOfTreasure_2_0 {

// размер иконок игрового поля с 1 до 9 уровней
constexpr QSize kIconSizeLevel[kLevelsGame]{
    QSize(66, 66), QSize(62, 62), QSize(58, 58), QSize(54, 54), QSize(50, 50),
    QSize(46, 46), QSize(42, 42), QSize(38, 38), QSize(34, 34),
};

class Ui_PlayingField {
 public:
  Ui_PlayingField() = default;
  Ui_PlayingField(Ui_PlayingField const &other) = delete;
  Ui_PlayingField(Ui_PlayingField &&other) = delete;
  auto operator=(Ui_PlayingField const &other) = delete;
  auto operator=(Ui_PlayingField &&other) = delete;

  auto setupUi(QWidget *Widget, uint8_t level_game) -> void {
    // вычисляем размер поля относительно уровня игры
    int size_filed = GetSizeFiledFromLevelGame(level_game);

    horisontal_layout = new QHBoxLayout();
    horisontal_layout->setSpacing(9);
    horisontal_layout->setContentsMargins(9, 9, 9, 9);
    //
    button_under_field = new QButtonGroup(horisontal_layout);
    for (uint8_t index = 0, id_button = 0; index < size_filed; ++index) {
      if (index % 2 == 0) {
        QPushButton *button = new QPushButton;
        //
        QSizePolicy size_policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        size_policy.setHorizontalStretch(0);
        size_policy.setVerticalStretch(0);
        size_policy.setHeightForWidth(button->sizePolicy().hasHeightForWidth());
        button->setSizePolicy(size_policy);
        button->setMinimumSize(kIconSizeLevel[level_game - 1].width() + 5,
                               kIconSizeLevel[level_game - 1].height() + 5);
        button->setMaximumSize(button->minimumSize());
        button->setIconSize(kIconSizeLevel[level_game - 1]);
        //                button->setStyleSheet(StyleHelper::getButtonStyleUnderGameField());
        //
        button_under_field->addButton(button, id_button);
        ++id_button;
        horisontal_layout->addWidget(button);
      } else {
        QSpacerItem *h_spacer = new QSpacerItem(70, 20, QSizePolicy::Expanding,
                                                QSizePolicy::Expanding);
        horisontal_layout->addSpacerItem(h_spacer);
      }
    }
    //
    grid_layout = new QGridLayout();
    grid_layout->setVerticalSpacing(18);
    grid_layout->setHorizontalSpacing(9);
    grid_layout->setContentsMargins(9, 9, 9, 9);
    //
    button_playing_field = new QButtonGroup(grid_layout);
    for (int row = 0, id_button = 0; row < size_filed; ++row) {
      for (int column = 0; column < size_filed; ++column) {
        QPushButton *button = new QPushButton;
        //
        QSizePolicy size_policy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        size_policy.setHorizontalStretch(0);
        size_policy.setVerticalStretch(0);
        size_policy.setHeightForWidth(button->sizePolicy().hasHeightForWidth());
        //
        button->setSizePolicy(size_policy);
        button->setMinimumSize(kIconSizeLevel[level_game - 1].width() + 5,
                               kIconSizeLevel[level_game - 1].height() + 5);
        button->setMaximumSize(button->minimumSize());
        button->setIconSize(kIconSizeLevel[level_game - 1]);
        //                button->setStyleSheet(StyleHelper::getButtonUnFocusStyle());
        button->setCursor(QCursor(Qt::OpenHandCursor));
        //
        button_playing_field->addButton(button, id_button);
        ++id_button;
        //
        grid_layout->addWidget(button, row, column, 1, 1);
      }
    }
    //        button_playing_field->button(0)->setStyleSheet(StyleHelper::getButtonFocusStyle());
    //
    vertical_layout = new QVBoxLayout(Widget);
    vertical_layout->setSpacing(0);
    vertical_layout->setContentsMargins(9, 9, 9, 9);
    //
    vertical_layout->addLayout(horisontal_layout);
    vertical_layout->addLayout(grid_layout);
    //
    Widget->setLayout(vertical_layout);
    Widget->setMinimumSize(vertical_layout->totalMinimumSize());
    Widget->setMaximumSize(vertical_layout->totalMinimumSize());

    Widget->setStyleSheet(StyleHelper::getBackgroundPlayingField());

    QMetaObject::connectSlotsByName(Widget);
  }  // setupUi

 public:
  QHBoxLayout *horisontal_layout;
  QButtonGroup *button_under_field;
  QGridLayout *grid_layout;
  QButtonGroup *button_playing_field;
  QVBoxLayout *vertical_layout;
};
}  // namespace InSearchOfTreasure_2_0
QT_END_NAMESPACE

#endif  // UI_PLAYING_FIELD_H
