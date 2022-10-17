#ifndef VIEW_GAME_H
#define VIEW_GAME_H

#include <QButtonGroup>
#include <QPainter>
#include <QPushButton>
#include <QWidget>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "controller_game.hpp"
#include "victory_dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace InSearchOfTreasure {

enum CoordsPlayingField {
  k00,  // 0
  k01,  // 1
  k02,  // 2
  k03,  // 3
  k04,  // 4
  k10,  // 5
  k11,  // 6
  k12,  // 7
  k13,  // 8
  k14,  // 9
  k20,  // 10
  k21,  // 11
  k22,  // 12
  k23,  // 13
  k24,  // 14
  k30,  // 15
  k31,  // 16
  k32,  // 17
  k33,  // 18
  k34,  // 19
  k40,  // 20
  k41,  // 21
  k42,  // 22
  k43,  // 23
  k44   // 24
};

class ViewGame : public QWidget {
  Q_OBJECT

 public:
  ViewGame(QWidget *parent = nullptr);
  ~ViewGame();

 protected:
  auto paintEvent(QPaintEvent *event) -> void;

  auto closeEvent(QCloseEvent *event) -> void;

  // функция добавляет все кнопки находящиеся на игровом поле в группу
  auto addButtonsPlayingFieldToGroup_() -> void;

  // основные настройки внешнего вида
  auto mainSettings_() -> void;

  // функция устанавливает задний фон
  auto installBackgroundImage_() -> void;

  // функция устанавливает стиль и иконку для кнопки start_game
  auto installStyleButtonStartGame_() -> void;
  // функция устанавливает стиль и иконку для кнопки about_game
  auto installStyleButtonAboutGame_() -> void;
  // функция устанавливает стиль и иконку для кнопки back
  auto installStyleButtonBack_() -> void;

  // функция устанавливает стили для лэйбла описания игры
  auto installStyleLabelGameDescription_() -> void;

  // функция для отображения динамического главного меню, постоянно находится в другом потоке
  auto dinamicMainMenu_() -> void;


  // функция устанавливает стили для кнопок игрового поля
  auto installStylePlayingFieldInMainMenu_() -> void;
  auto installStylePlayingFieldInProcessGame_() -> void;

  // функция устанавливает иконки для строки над игровым полем
  auto installIconToCellInRowOverPlayingField_(int index_in_row,
                                               QString const &path_to_icon) -> void;

  // функция очищает все иконки находящиеся на игровом поле
  auto clearAllIcons_() -> void;


 private slots:
  void on_button_about_game_clicked();
  void on_button_back_clicked();
  void on_button_start_game_clicked();
  void on_button_in_playing_field_clicked(int id_button);
  void on_dialog_button_restart_clicked();
  void on_dialog_button_end_clicked();

 protected:
  // функции для смены стиля отобрадения игрового поля
  auto changeStylePossibleStepsButtons_() -> void;
  auto changeStyleInactiveButtons_() -> void;

 private:
  Ui::Widget *ui;
  bool m_is_game_start{false};
  ControllerGame &m_controller{ControllerGame::getInstance()};
  VictoryDialog *m_victory_dialog;
  QButtonGroup m_group;
  bool m_programm_exit{false};
  std::thread *m_th{nullptr};
  std::condition_variable m_cv;
  std::mutex m_mutex;
};
}  // namespace InSearchOfTreasure
QT_END_NAMESPACE
#endif  // VIEW_GAME_H
