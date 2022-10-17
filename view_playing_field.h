#ifndef VIEWPLAYINGFIELD_H
#define VIEWPLAYINGFIELD_H

#include <QWidget>
#include "ui_playing_field.h"
#include <QPainter>
#include <QStyleOption>
#include <QCloseEvent>

#include "controller_game.hpp"

QT_BEGIN_NAMESPACE
namespace InSearchOfTreasure {

class ViewPlayingField : public QWidget
{
    Q_OBJECT
public:
    explicit ViewPlayingField(uint8_t level_game, QWidget *parent = nullptr)
        : QWidget(parent), ui(new Ui_PlayingField) {
        ui->setupUi(this, level_game);
    }
    ~ViewPlayingField() { delete ui; }

protected:
    auto paintEvent(QPaintEvent *event) -> void {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        QWidget::paintEvent(event);
    }

    auto closeEvent(QCloseEvent *event) -> void {
        event->accept();
    }

    auto installStylePlayingFieldInProcessGame_() -> void {
      // генерация игрового поля
      m_controller.GenerateField();
      auto buttons_playing_field = ui->button_playing_field->buttons();
      for (int id_button = 0;
           id_button < buttons_playing_field.size(); ++id_button) {

      }


      for (int id_button = CoordsPlayingField::k00;
           id_button <= CoordsPlayingField::k44; ++id_button) {
        int color_cell = m_controller.GetColorCell(id_button);
        auto button = m_group.button(id_button);
        button->setStyleSheet(
            StyleHelper::getInactiveButtonStyle());
        if (color_cell == Color::kRed) {
          // иконка рубина
          button->setIcon(
              QIcon(StyleHelper::getPathToRubyIcon()));
          // активный стиль
          button->setStyleSheet(
              StyleHelper::getReleasedButtonStyle());
        } else if (color_cell == Color::kGreen) {
          // иконка изумруда
           button->setIcon(
              QIcon(StyleHelper::getPathToEmeraldIcon()));
          // активный стиль
          button->setStyleSheet(
              StyleHelper::getReleasedButtonStyle());
        } else if (color_cell == Color::kBlue) {
          // иконка сапфира
          m_group.button(id_button)->setIcon(
              QIcon(StyleHelper::getPathToSaphfireIcon()));
          // активный стиль
          button->setStyleSheet(
              StyleHelper::getReleasedButtonStyle());
        } else {
            // неактивный стиль
            button->setStyleSheet(
                StyleHelper::getInactiveButtonStyle());
            // если это ячейка неактивного поля, то ставлю иконку сундука
            if (m_controller.IsSelectedBlockCell(id_button)) {
                m_group.button(id_button)->setIcon(
                  QIcon(StyleHelper::getPathToBoxIcon()));
            } else {
                m_group.button(id_button)->setIcon(QIcon());
            }
        }
      }

      // установка иконок кнопок над игровым полем
      installIconToCellInRowOverPlayingField_(
          m_controller.GetPosRedChipInExpectedRow(),
          StyleHelper::getPathToRubyIcon());

      installIconToCellInRowOverPlayingField_(
          m_controller.GetPosGreenChipInExpectedRow(),
          StyleHelper::getPathToEmeraldIcon());

      installIconToCellInRowOverPlayingField_(
          m_controller.GetPosBlueChipInExpectedRow(),
          StyleHelper::getPathToSaphfireIcon());
    }

private:
 Ui_PlayingField *ui;
 ControllerGame &m_controller{ControllerGame::getInstance()};
};

}
QT_END_NAMESPACE

#endif // VIEWPLAYINGFIELD_H
