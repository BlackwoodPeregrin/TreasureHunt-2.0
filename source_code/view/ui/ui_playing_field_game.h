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
#include <QGuiApplication>

QT_BEGIN_NAMESPACE

namespace TreasureHunt_2_0 {

// процент занимаемого экрана игрового поля с 1 до 9 уровней
constexpr float kAppScreenPercentage[kLevelsGame]{
    0.35f, 0.4f, 0.45f, 0.5f, 0.55f, 0.58f, 0.62f, 0.65f
};

class Ui_PlayingField {
 public:
    Ui_PlayingField(int w_screen, int h_screen) {
        m_screen_app = w_screen < h_screen ? w_screen : h_screen;
  }
  Ui_PlayingField(Ui_PlayingField const &other) = delete;
  Ui_PlayingField(Ui_PlayingField &&other) = delete;
  auto operator=(Ui_PlayingField const &other) = delete;
  auto operator=(Ui_PlayingField &&other) = delete;


  auto calculateGeometry(int level_game, int size_filed) -> void {
      if (level_game >= 0  && level_game < 9) {
        m_screen_app *= kAppScreenPercentage[level_game - 1];
      }
      m_size_icon = QSize((m_screen_app / size_filed),
                          (m_screen_app / size_filed));
  }

  auto setupUi(QWidget *Widget, int level_game) -> void {
    // вычисляем размер поля относительно уровня игры
    int size_filed = GetSizeFiledFromLevelGame(level_game);
    calculateGeometry(level_game, size_filed);

    horisontal_layout = new QHBoxLayout();
    horisontal_layout->setSpacing(9);
    horisontal_layout->setContentsMargins(9, 0, 9, 0);
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
        //
        button->setMinimumSize(m_size_icon);
        button->setMaximumSize(m_size_icon);
        button->setIconSize(m_size_icon - m_size_icon * 0.15);
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
        button->setSizePolicy(size_policy);
        //
        button->setMinimumSize(m_size_icon);
        button->setMaximumSize(m_size_icon);
        button->setIconSize(m_size_icon - m_size_icon * 0.15);
        button->setCursor(QCursor(Qt::OpenHandCursor));
        //
        button_playing_field->addButton(button, id_button);
        ++id_button;
        //
        grid_layout->addWidget(button, row, column, 1, 1);
      }
    }
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
    //
    Widget->setStyleSheet(StyleHelper::getBackgroundPlayingField());
    //
    QMetaObject::connectSlotsByName(Widget);
  }  // setupUi

 public:
  QHBoxLayout *horisontal_layout;
  QButtonGroup *button_under_field;
  QGridLayout *grid_layout;
  QButtonGroup *button_playing_field;
  QVBoxLayout *vertical_layout;
  QSize m_size_icon;
  int m_screen_app;
};
}  // namespace TreasureHunt_2_0
QT_END_NAMESPACE

#endif  // UI_PLAYING_FIELD_H
