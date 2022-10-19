#ifndef VIEWPLAYINGFIELD_H
#define VIEWPLAYINGFIELD_H

#include <QWidget>
#include <QPainter>
#include <QStyleOption>
#include <QCloseEvent>
#include <QPixmap>
#include <thread>
#include <QTimer>

#include "ui_playing_field.h"
#include "controller_game.hpp"
#include "victory_dialog.h"

QT_BEGIN_NAMESPACE
namespace InSearchOfTreasure_2_0 {

class ViewPlayingField : public QWidget
{
    Q_OBJECT
public:
    explicit ViewPlayingField(uint8_t level_game, QWidget *parent = nullptr)
        : QWidget{parent},
          m_click_counter{0},
          m_current_id_button{0},
          m_cell_in_focus{false},
          ui{new Ui_PlayingField},
          m_dialog{nullptr},
          m_close_game{false}{
        //
        ui->setupUi(this, level_game);
        installStylePlayingFieldInProcessGame_(level_game);
        //
        connect(ui->button_playing_field, SIGNAL(idClicked(int)), this,
                SLOT(on_buttons_playing_filed_clicked(int)));
    }

    ~ViewPlayingField() {
        qDebug() << "destructor";
        delete ui;
        delete m_dialog;
    }

protected:
    auto paintEvent(QPaintEvent *event) -> void {
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
        QWidget::paintEvent(event);
    }

    auto closeEvent(QCloseEvent *event) -> void {
        if (m_close_game) {
            event->accept();
        } else {
            QString messege = "Закрытие игры приведет к потере прогресса."
                              "\nВы действительно хотите выйти?";
            QString text_button1 = "Да";
            QString text_button2 = "Нет";
            m_dialog = new MessageWindow(messege, text_button1, text_button2);
            m_dialog->show();
            connect(m_dialog, &MessageWindow::button1, this, [this] {
                m_close_game = true;
                close();
            });
            connect(m_dialog, &MessageWindow::close_dialog, this, [this] {
                qDebug() << "close";
                delete m_dialog;
                m_dialog = nullptr;
            });
            event->ignore();
        }
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
        } else if (key == Qt::Key_Space || key == Qt::Key_Enter || key == Qt::Key_Return) {
            setFocusCell();
        }
    }


    auto stepUp_() -> void {
        int id_button_cur = m_controller.GetCurrentCoord_model();

        if (m_cell_in_focus) {
            if (m_controller.MoveChipUp_model()) {
                swapIconsButtons_(id_button_cur, m_controller.GetCurrentCoord_model());
                installPressedFocusStyleButton(m_controller.GetCurrentCoord_model());
                installUnFocusStyleButton(id_button_cur);
                // проверка на победу в игре
                if (m_controller.IsVictoryGame_model()) {
//                    m_victory_dialog = new VictoryDialog;
                }
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
                // проверка на победу в игре
                if (m_controller.IsVictoryGame_model()) {
//                    m_victory_dialog = new VictoryDialog;
//                    m_victory_dialog->show();
                }
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
                // проверка на победу в игре
                if (m_controller.IsVictoryGame_model()) {
//                    m_victory_dialog = new VictoryDialog;
//                    m_victory_dialog->show();
                }
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
                // проверка на победу в игре
                if (m_controller.IsVictoryGame_model()) {
//                    m_victory_dialog = new VictoryDialog;
//                    m_victory_dialog->show();
                }
            }
        } else if (m_controller.StepLeft_model()) {
            installFocusStyleButton(m_controller.GetCurrentCoord_model());
            installUnFocusStyleButton(id_button_cur);
        }
    }

    auto setFocusCell() -> void {
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


 private slots:
    auto on_buttons_playing_filed_clicked(int id_button) -> void {
        m_current_id_button = id_button;
        ++m_click_counter;
        if (m_click_counter == 1) {
            QTimer::singleShot(200, this, SLOT(CheckClickCounter_()));
        }
    }

    auto CheckClickCounter_() -> void {
        if (m_click_counter == 1) {
            one_click_on_button();
        } else if (m_click_counter == 2) {
            two_click_on_button();
        }
        m_click_counter = 0;
    }

 protected:
    auto one_click_on_button() -> void {
        int id_button_prev = m_controller.GetCurrentCoord_model();
        if (m_cell_in_focus && m_controller.IsCellPossibleStep_model(m_current_id_button)) {
            m_controller.SwapChip_model(m_current_id_button);
            //
            swapIconsButtons_(id_button_prev, m_current_id_button);
            installUnFocusStyleButton(id_button_prev);
            installPressedFocusStyleButton(m_current_id_button);
        } else if (!m_cell_in_focus && !m_controller.IsSelectedBlockCell_model(m_current_id_button)) {
          //
          installUnFocusStyleButton(id_button_prev);
          installFocusStyleButton(m_current_id_button);
          m_controller.ChangeCurrentCoord_model(m_current_id_button);
        }
    }

    auto two_click_on_button() -> void {
        int id_button_prev = m_controller.GetCurrentCoord_model();
        if (m_cell_in_focus && m_current_id_button == id_button_prev) {
            //
            installFocusStyleButton(m_current_id_button);
            m_controller.ClearPossibleSteps_model();
            m_cell_in_focus = false;
        } else if (m_controller.IsSelectedChipCell_model(m_current_id_button)){
            //
            installUnFocusStyleButton(id_button_prev);
            installPressedFocusStyleButton(m_current_id_button);
            m_controller.ChangePossibleStepsChipInPlayingField_model(m_current_id_button);
            m_cell_in_focus = true;
        }
    }


private:
 int m_click_counter;
 int m_current_id_button;
 bool m_cell_in_focus;
 Ui_PlayingField *ui;
 MessageWindow *m_dialog;
 ControllerGame &m_controller{ControllerGame::getInstance()};
 bool m_close_game;
};

}
QT_END_NAMESPACE

#endif // VIEWPLAYINGFIELD_H
