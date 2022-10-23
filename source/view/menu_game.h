#ifndef VIEW_GAME_H
#define VIEW_GAME_H

#include <QButtonGroup>
#include <QPainter>
#include <QPushButton>
#include <QWidget>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "playing_field_game.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

QT_BEGIN_NAMESPACE
namespace TreasureHunt_2_0 {

class ViewMenuGame : public QWidget {
  Q_OBJECT

 public:
  ViewMenuGame(QWidget *parent = nullptr);
  ~ViewMenuGame();

 protected:
  auto paintEvent(QPaintEvent *event) -> void;

  auto closeEvent(QCloseEvent *event) -> void;

  // функция добавляет все кнопки находящиеся на игровом поле в группу
  auto addButtonsPlayingFieldToGroup_() -> void;

  // основные настройки внешнего вида
  auto installMainSettings_() -> void;

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

  // функция для отображения динамического главного меню, постоянно находится в
  // другом потоке
  auto dinamicMainMenu_() -> void;

 private slots:
  void on_button_about_game_clicked();
  void on_button_back_clicked();
  void on_button_start_game_clicked();

 private:
  bool m_programm_exit;
  Ui::Widget *ui;
  std::thread *m_th;
  ViewPlayingField *m_playing_field;
  QButtonGroup m_group;
  std::condition_variable m_cv;
  std::mutex m_mutex;
};
}  // namespace TreasureHunt_2_0
QT_END_NAMESPACE
#endif  // VIEW_GAME_H
