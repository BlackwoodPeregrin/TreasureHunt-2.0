#ifndef VIEWPLAYINGFIELD_H
#define VIEWPLAYINGFIELD_H

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QCloseEvent>
#include <QPixmap>
#include <thread>

#include "ui_playing_field.h"
#include "controller_game.hpp"

QT_BEGIN_NAMESPACE
namespace InSearchOfTreasure_2_0 {

class ViewPlayingField : public QWidget
{
    Q_OBJECT
public:
    explicit ViewPlayingField(uint8_t level_game, QWidget *parent = nullptr)
        : QWidget(parent), ui(new Ui_PlayingField) {
        ui->setupUi(this, level_game);
        installStylePlayingFieldInProcessGame_(level_game);
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

    auto installStylePlayingFieldInProcessGame_(uint8_t level_game) -> void {
      // генерация игрового поля
      m_controller.GenerateField_model(level_game);

      // расстановка иконок игрового поля
      int sum_buttons = ui->button_playing_field->buttons().size();

      for (int id_button = 0; id_button < sum_buttons; ++id_button) {
          int color_cell = m_controller.GetColorCell_model(id_button);
          QAbstractButton *button = ui->button_playing_field->button(id_button);
          if (color_cell == ColorStone::kNoColor
                  && m_controller.IsSelectedBlockCell_model(id_button)) {
              button->setIcon(QIcon(StyleHelper::getPathToBoxIcon()));
          } else {
              button->setIcon(GetIconFromColor_(color_cell));
          }
      }

      // расстановка иконок над игрового поля (ожидаемый результат)
      std::vector<int> colors = std::move(m_controller.GetColorExpectedRow_model());
      sum_buttons = ui->button_under_field->buttons().size();
      for (int id_button = 0; id_button < sum_buttons; ++id_button) {
          ui->button_under_field->button(id_button)->setIcon(
                      GetIconFromColor_(colors[id_button]));
      }
    }

    auto GetIconFromColor_(int color) -> QIcon {
       QIcon icon;
        switch (color) {
        case ColorStone::kBlue:
            icon = QIcon(StyleHelper::getPathToBlueStoneIcon());
            break;
        case ColorStone::kGreen:
            icon = QIcon(StyleHelper::getPathToGreenStoneIcon());
            break;
        case ColorStone::kRed:
            icon = QIcon(StyleHelper::getPathToRedStoneIcon());
            break;
        case ColorStone::kYellow:
            icon = QIcon(StyleHelper::getPathToYellowStoneIcon());
            break;
        case ColorStone::kPerple:
            icon = QIcon(StyleHelper::getPathToPerpleStoneIcon());
            break;
        case ColorStone::kGray:
            icon = QIcon(StyleHelper::getPathToGreyStoneIcon());
            break;
        case ColorStone::kBlack:
            icon = QIcon(StyleHelper::getPathToBlackStoneIcon());
            break;
        case ColorStone::kSky:
            icon = QIcon(StyleHelper::getPathToSkyStoneIcon());
            break;
        case ColorStone::kLightGreen:
            icon = QIcon(StyleHelper::getPathToLightGreenStoneIcon());
            break;
        case ColorStone::kWhite:
            icon = QIcon(StyleHelper::getPathToWhiteStoneIcon());
            break;
        default:
            break;
        }
      return icon;
    }

    auto keyPressEvent(QKeyEvent *event) -> void {
        int key = event->key();
        if (key == Qt::Key_W || key == Qt::Key_Up) {
            stepUp_();
        } else if (key == Qt::Key_D || key == Qt::Key_Right) {
            stepRight_();
        } else if (key == Qt::Key_S || key == Qt::Key_Down) {
            stepDown_();
        } else if (key == Qt::Key_A || key == Qt::Key_Left) {
            stepLeft_();
        } else if (key == Qt::Key_Space) {
            showPossibleSteps();
        }
    }


    auto stepUp_() -> void {
        int id_button_cur = m_controller.GetCurrentCoord_model();

        if (m_cell_in_focus) {
            if (m_controller.MoveChipUp_model()) {
                swapIconsButtons_(id_button_cur, m_controller.GetCurrentCoord_model());
                installPressedFocusStyleButton(m_controller.GetCurrentCoord_model());
                installUnFocusStyleButton(id_button_cur);
            }
        } else if (m_controller.StepUp_model()) {
            installFocusStyleButton(m_controller.GetCurrentCoord_model());
            installUnFocusStyleButton(id_button_cur);
        }
    }

    auto stepRight_() -> void {
        int id_button_cur = m_controller.GetCurrentCoord_model();

        if (m_cell_in_focus) {
            if (m_controller.MoveChipRight_model()) {
                swapIconsButtons_(id_button_cur, m_controller.GetCurrentCoord_model());
                installPressedFocusStyleButton(m_controller.GetCurrentCoord_model());
                installUnFocusStyleButton(id_button_cur);
            }
        } else if (m_controller.StepRight_model()) {
            installFocusStyleButton(m_controller.GetCurrentCoord_model());
            installUnFocusStyleButton(id_button_cur);
        }
    }

    auto stepDown_() -> void {
        int id_button_cur = m_controller.GetCurrentCoord_model();

        if (m_cell_in_focus) {
            if (m_controller.MoveChipDown_model()) {
                swapIconsButtons_(id_button_cur, m_controller.GetCurrentCoord_model());
                installPressedFocusStyleButton(m_controller.GetCurrentCoord_model());
                installUnFocusStyleButton(id_button_cur);
            }
        } else if (m_controller.StepDown_model()) {
            installFocusStyleButton(m_controller.GetCurrentCoord_model());
            installUnFocusStyleButton(id_button_cur);
        }
    }

    auto stepLeft_() -> void {
        int id_button_cur = m_controller.GetCurrentCoord_model();

        if (m_cell_in_focus) {
            if (m_controller.MoveChipLeft_model()) {
                swapIconsButtons_(id_button_cur, m_controller.GetCurrentCoord_model());
                installPressedFocusStyleButton(m_controller.GetCurrentCoord_model());
                installUnFocusStyleButton(id_button_cur);
            }
        } else if (m_controller.StepLeft_model()) {
            installFocusStyleButton(m_controller.GetCurrentCoord_model());
            installUnFocusStyleButton(id_button_cur);
        }
    }

    auto showPossibleSteps() -> void {
        int id_button_cur = m_controller.GetCurrentCoord_model();

        // если клетка является игровой фишкой
        if (m_controller.IsSelectedChipCell_model(id_button_cur)) {
            if (m_cell_in_focus) {
                installFocusStyleButton(id_button_cur);
                m_cell_in_focus = false;
            } else {
                installPressedFocusStyleButton(id_button_cur);
                m_cell_in_focus = true;
            }
        }
    }

    auto swapIconsButtons_(int id_b1, int id_b2) -> void {
        auto button1 = ui->button_playing_field->button(id_b1);
        auto button2 = ui->button_playing_field->button(id_b2);
        QIcon icon1 = button1->icon();
        button1->setIcon(button2->icon());
        button2->setIcon(icon1);
    }

    auto installFocusStyleButton(int id_button) -> void {
        ui->button_playing_field->button(id_button)->setStyleSheet(
                    StyleHelper::getButtonFocusStyle());
    }

    auto installPressedFocusStyleButton(int id_button) -> void {
        ui->button_playing_field->button(id_button)->setStyleSheet(
                    StyleHelper::getButtonPressedFocusStyle());
    }

    auto installUnFocusStyleButton(int id_button) -> void {
        ui->button_playing_field->button(id_button)->setStyleSheet(
                    StyleHelper::getButtonUnFocusStyle());
    }



    auto showPossibleSteps_() -> void {
      std::vector<int> id_buttons = m_controller.GetVectorPossibleSteps_model();
      for (int id : id_buttons) {
        ui->button_playing_field->button(id)
                ->setStyleSheet(StyleHelper::getPossibleStepsButtonStyle());
      }
    }

    auto hidePossibleSteps_() -> void {
      std::vector<int> id_buttons = m_controller.GetVectorPossibleSteps_model();
      for (int &id : id_buttons) {
          ui->button_playing_field->button(id)
                  ->setStyleSheet(StyleHelper::getButtonUnFocusStyle());
      }
    }


//    void ViewGame::on_button_in_playing_field_clicked(int id_button) {
//      if (m_is_game_start == true) {
//        // если ячейка является возможным шагом
//        if (m_controller.IsCellPossibleStep(id_button) == true) {
//          //  сохраняем координаты предидущего клика
//          int id_button_prev = m_controller.GetCurrentCoord();

//          // меняю стиль подсвеченных клеток возможных шагов на обычный
//          changeStyleInactiveButtons_();

//          //  меняем местами клетки в модели
//          m_controller.SwapCells(id_button);

//          // меняем иконки местами
//          m_group.button(id_button)
//              ->setStyleSheet(StyleHelper::getReleasedButtonStyle());
//          m_group.button(id_button_prev)
//              ->setStyleSheet(StyleHelper::getInactiveButtonStyle());

//          m_group.button(id_button)->setIcon(
//              m_group.button(id_button_prev)->icon());
//          m_group.button(id_button_prev)->setIcon(QIcon());

//          // проверяем наступила ли победа в игре
//          if (m_controller.IsVictoryGame()) {
//            // вызываем диалоговое окно с ифнормацией о завершении игры
//            m_victory_dialog->exec();
//          }
//        } else {
//          // меняем текущую выбранную ячейку и пересчитываем возможные шаги
//          if (m_controller.IsSelectedChipCell(id_button)) {
//            int id_button_prev = m_controller.GetCurrentCoord();

//            // если ячейка былы выбрана, то меняем ей стили на стандартный
//            if (id_button_prev != -1) {
//              m_group.button(id_button_prev)
//                  ->setStyleSheet(StyleHelper::getReleasedButtonStyle());
//            }

//            // меняю стиль подсвеченных клеток возможных шагов на обычный
//            changeStyleInactiveButtons_();

//            // меняем стиль новой выбранной кнопки
//            m_group.button(id_button)->setStyleSheet(
//                StyleHelper::getPressedButtonChooseStyle());

//            // рассчитываем новые возможные шаги
//            m_controller.ChangePossibleStepsChipInPlayingField_(id_button);
//            // меняем их стиль отобрадения на поле
//            changeStylePossibleStepsButtons_();
//          }
//        }
//      }
//    }


private:
 Ui_PlayingField *ui;
 ControllerGame &m_controller{ControllerGame::getInstance()};
 bool m_cell_in_focus{false};
};

}
QT_END_NAMESPACE

#endif // VIEWPLAYINGFIELD_H
