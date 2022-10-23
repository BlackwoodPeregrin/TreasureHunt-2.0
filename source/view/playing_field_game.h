#ifndef VIEWPLAYINGFIELD_H
#define VIEWPLAYINGFIELD_H

#include <QCloseEvent>
#include <QPainter>
#include <QPixmap>
#include <QStyleOption>
#include <QTimer>
#include <QWidget>
#include <functional>
#include <thread>

#include "../controller/controller_game.hpp"
#include "message_dialog.h"
#include "ui/ui_playing_field_game.h"

QT_BEGIN_NAMESPACE
namespace TreasureHunt_2_0 {

class ViewPlayingField : public QWidget {
  Q_OBJECT
 public:
  explicit ViewPlayingField(uint8_t level_game, QWidget *parent = nullptr)
      : QWidget{parent},
        m_click_counter{0},
        m_current_id_button{0},
        m_cell_in_focus{false},
        ui{new Ui_PlayingField},
        m_dialog{nullptr},
        m_close_game{false},
        m_level_game(level_game) {
    //
    ui->setupUi(this, level_game);
    this->setWindowTitle("Treasure Hunt");
    installStylePlayingFieldInProcessGame_(level_game);
    //
    connect(ui->button_playing_field, SIGNAL(idClicked(int)), this,
            SLOT(on_buttons_playing_filed_clicked(int)));
  }

  ~ViewPlayingField() { delete ui; }

signals:
    void close_playing_field();

 protected:
  auto paintEvent(QPaintEvent *event) -> void override {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
  }

  auto closeEvent(QCloseEvent *event) -> void override {
    event->ignore();
    if (m_close_game) {
      event->accept();
    } else {
      GenerateDialogWidnow(StyleHelper::getMessageExitGame(), "Exit",
                           QIcon(StyleHelper::getPathToYesIcon()),
                           QSize(45, 45), QIcon(StyleHelper::getPathToNoIcon()),
                           QSize(35, 35), [this] {
                             m_close_game = true;
                             emit close_playing_field();
                             this->close();
                           });
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
      if (color_cell == ColorStone::kNoColor &&
          m_controller.IsSelectedBlockCell_model(id_button)) {
        button->setIcon(QIcon(StyleHelper::getPathToBoxIcon()));
      } else {
        button->setIcon(GetIconFromColor_(color_cell));
      }
      button->setStyleSheet(StyleHelper::getButtonUnFocusStyle());
    }

    // в начале игры выбранная ячейка верхняя слева по умолчанию
    ui->button_playing_field->button(0)->setStyleSheet(
        StyleHelper::getButtonFocusStyle());

    // расстановка иконок над игрового поля (ожидаемый результат)
    std::vector<int> colors =
        std::move(m_controller.GetColorExpectedRow_model());
    sum_buttons = ui->button_under_field->buttons().size();
    for (int id_button = 0; id_button < sum_buttons; ++id_button) {
      QAbstractButton *button = ui->button_under_field->button(id_button);
      button->setIcon(GetIconFromColor_(colors[id_button]));
      button->setStyleSheet(StyleHelper::getButtonStyleUnderGameField());
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

  auto keyPressEvent(QKeyEvent *event) -> void override {
    int key = event->key();
    if (key == Qt::Key_W || key == Qt::Key_Up) {
      stepUp_();
    } else if (key == Qt::Key_D || key == Qt::Key_Right) {
      stepRight_();
    } else if (key == Qt::Key_S || key == Qt::Key_Down) {
      stepDown_();
    } else if (key == Qt::Key_A || key == Qt::Key_Left) {
      stepLeft_();
    } else if (key == Qt::Key_Space || key == Qt::Key_Enter ||
               key == Qt::Key_Return) {
      setFocusCell();
    }
  }

  auto stepUp_() -> void {
    int id_but_cur = m_controller.GetCurrentCoord_model();
    //
    if (m_cell_in_focus) {
      if (m_controller.SwapPlacesWithTopCell_model()) {
        int id_but_new_cur = m_controller.GetCurrentCoord_model();
        // меняю местами иконки ячеек
        swapIconsButtons_(id_but_cur, id_but_new_cur);
        // меняю фокус на новую клетку
        installPressedFocusStyleButton(id_but_new_cur);
        // убираю фокус с предидущей
        installUnFocusStyleButton(id_but_cur);
        // проверка на победу в игре
        checkVictoryGame_();
      }
    } else if (m_controller.StepUp_model()) {
      int id_but_new_cur = m_controller.GetCurrentCoord_model();
      // меняю фокус на новую клетку
      installFocusStyleButton(id_but_new_cur);
      // убираю фокус с предидущей
      installUnFocusStyleButton(id_but_cur);
    }
  }

  auto stepRight_() -> void {
    int id_but_cur = m_controller.GetCurrentCoord_model();
    //
    if (m_cell_in_focus) {
      if (m_controller.SwapPlacesWithRightCell_model()) {
        int id_but_new_cur = m_controller.GetCurrentCoord_model();
        // меняю местами иконки ячеек
        swapIconsButtons_(id_but_cur, id_but_new_cur);
        // меняю фокус на новую клетку
        installPressedFocusStyleButton(id_but_new_cur);
        // убираю фокус с предидущей
        installUnFocusStyleButton(id_but_cur);
        // проверка на победу в игре
        checkVictoryGame_();
      }
    } else if (m_controller.StepRight_model()) {
      int id_but_new_cur = m_controller.GetCurrentCoord_model();
      // меняю фокус на новую клетку
      installFocusStyleButton(id_but_new_cur);
      // убираю фокус с предидущей
      installUnFocusStyleButton(id_but_cur);
    }
  }

  auto stepDown_() -> void {
    int id_but_cur = m_controller.GetCurrentCoord_model();
    //
    if (m_cell_in_focus) {
      if (m_controller.SwapPlacesWithBottomCell_model()) {
        int id_but_new_cur = m_controller.GetCurrentCoord_model();
        // меняю местами иконки ячеек
        swapIconsButtons_(id_but_cur, id_but_new_cur);
        // меняю фокус на новую клетку
        installPressedFocusStyleButton(id_but_new_cur);
        // убираю фокус с предидущей
        installUnFocusStyleButton(id_but_cur);
        // проверка на победу в игре
        checkVictoryGame_();
      }
    } else if (m_controller.StepDown_model()) {
      int id_but_new_cur = m_controller.GetCurrentCoord_model();
      // меняю фокус на новую клетку
      installFocusStyleButton(id_but_new_cur);
      // убираю фокус с предидущей
      installUnFocusStyleButton(id_but_cur);
    }
  }

  auto stepLeft_() -> void {
    int id_but_cur = m_controller.GetCurrentCoord_model();
    //
    if (m_cell_in_focus) {
      if (m_controller.SwapPlacesWithLeftCell_model()) {
        int id_but_new_cur = m_controller.GetCurrentCoord_model();
        // меняю местами иконки ячеек
        swapIconsButtons_(id_but_cur, id_but_new_cur);
        // меняю фокус на новую клетку
        installPressedFocusStyleButton(id_but_new_cur);
        // убираю фокус с предидущей
        installUnFocusStyleButton(id_but_cur);
        // проверка на победу в игре
        checkVictoryGame_();
      }
    } else if (m_controller.StepLeft_model()) {
      int id_but_new_cur = m_controller.GetCurrentCoord_model();
      // меняю фокус на новую клетку
      installFocusStyleButton(id_but_new_cur);
      // убираю фокус с предидущей
      installUnFocusStyleButton(id_but_cur);
    }
  }

  auto setFocusCell() -> void {
    int id_but_cur = m_controller.GetCurrentCoord_model();
    // если клетка является игровой фишкой
    if (m_controller.IsSelectedChipCell_model(id_but_cur)) {
      if (m_cell_in_focus) {
        installFocusStyleButton(id_but_cur);
        m_cell_in_focus = false;
      } else {
        installPressedFocusStyleButton(id_but_cur);
        m_controller.CalcStepsAroundCell_model(id_but_cur);
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
      one_click_on_cell_field();
    } else if (m_click_counter == 2) {
      two_click_on_cell_field();
    }
    m_click_counter = 0;
  }

 protected:
  auto one_click_on_cell_field() -> void {
    int id_button_prev = m_controller.GetCurrentCoord_model();
    //
    if (m_cell_in_focus) {
      if (m_controller.IsCellPossibleStep_model(m_current_id_button)) {
        //
        m_controller.SwapChip_model(m_current_id_button);
        // меняю местами иконки ячеек
        swapIconsButtons_(id_button_prev, m_current_id_button);
        // меняю фокус на новую клетку
        installUnFocusStyleButton(id_button_prev);
        // убираю фокус с предидущей
        installPressedFocusStyleButton(m_current_id_button);
        // проверка на победу в игре
        checkVictoryGame_();
      }
    } else {
      if (!m_controller.IsSelectedBlockCell_model(m_current_id_button)) {
        m_controller.ChangeCurrentCoord_model(m_current_id_button);
        // меняю фокус на новую клетку
        installUnFocusStyleButton(id_button_prev);
        // убираю фокус с предидущей
        installFocusStyleButton(m_current_id_button);
      }
    }
  }

  auto two_click_on_cell_field() -> void {
    int id_button_prev = m_controller.GetCurrentCoord_model();
    //
    if (m_cell_in_focus) {
      if (m_current_id_button == id_button_prev) {
        m_cell_in_focus = false;
        installFocusStyleButton(m_current_id_button);
      } else if (m_controller.IsSelectedChipCell_model(m_current_id_button)) {
        installUnFocusStyleButton(id_button_prev);
        installPressedFocusStyleButton(m_current_id_button);
        m_controller.CalcStepsAroundCell_model(m_current_id_button);
        //
      } else if (m_controller.IsCellPossibleStep_model(m_current_id_button)) {
        //
        m_controller.SwapChip_model(m_current_id_button);
        // меняю местами иконки ячеек
        swapIconsButtons_(id_button_prev, m_current_id_button);
        // меняю фокус на новую клетку
        installUnFocusStyleButton(id_button_prev);
        // убираю фокус с предидущей
        installPressedFocusStyleButton(m_current_id_button);
        // проверка на победу в игре
        checkVictoryGame_();
      }
    } else {
      if (m_controller.IsSelectedChipCell_model(m_current_id_button)) {
        m_controller.CalcStepsAroundCell_model(m_current_id_button);
        installUnFocusStyleButton(id_button_prev);
        installPressedFocusStyleButton(m_current_id_button);
        m_cell_in_focus = true;
      }
    }
  }

  auto checkVictoryGame_() -> void {
    if (m_controller.IsVictoryGame_model()) {
      m_cell_in_focus = false;
      GenerateDialogWidnow(
          StyleHelper::getMessageVictory(), "Victory",
          QIcon(StyleHelper::getPathToUndoIcon()), QSize(40, 40),
          QIcon(StyleHelper::getPathRepeatButtonIcon()), QSize(40, 40),
          [this] {
            m_close_game = true;
            emit close_playing_field();
            this->close();
          },
          [this] {
            this->installStylePlayingFieldInProcessGame_(m_level_game);
          });
    }
  }

  auto GenerateDialogWidnow(
      QString const &message, QString const &title, QIcon const &button_accept,
      QSize const &button_accept_icon, QIcon const &button_reject,
      QSize const &button_reject_icon, std::function<void()> slot_accept,
      std::function<void()> slot_reject = [] {}) -> void {
    //
    if (m_dialog) {
      delete m_dialog;
    }
    m_dialog =
        new MessageWindow(message, title, button_accept, button_accept_icon,
                          button_reject, button_reject_icon, this);

    connect(m_dialog, &MessageWindow::accepted, this, slot_accept);
    connect(m_dialog, &MessageWindow::rejected, this, slot_reject);

    m_dialog->exec();
  }

 private:
  int m_click_counter;
  int m_current_id_button;
  bool m_cell_in_focus;
  Ui_PlayingField *ui;
  MessageWindow *m_dialog;
  ControllerGame &m_controller{ControllerGame::getInstance()};
  bool m_close_game;
  int m_level_game;
};

}  // namespace TreasureHunt_2_0
QT_END_NAMESPACE

#endif  // VIEWPLAYINGFIELD_H
