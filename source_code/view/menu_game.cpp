#include "menu_game.h"

#include <QMessageBox>
#include <QStyleOption>
#include <QTabBar>

#include "style_helper.h"
#include "ui_menu_game.h"

QT_BEGIN_NAMESPACE
namespace TreasureHunt_2_0 {

ViewMenuGame::ViewMenuGame(QWidget *parent)
    : QWidget{parent},
      m_programm_exit{false},
      ui{new Ui::Widget},
      m_th{nullptr},
      m_playing_field{nullptr} {
    srand(time(0));
  //
  ui->setupUi(this);

  // скрыл вкладки выбора и устанвил первую страницу таб бара
  ui->tabWidget->tabBar()->hide();
  ui->tabWidget->setCurrentIndex(0);

  // добавления кнопок в группу
  addButtonsPlayingFieldToGroup_();

  // настройки по размеру иконок на игровом поле
  installMainSettings_();

  // в другой поток уходит генерация игрового поля в главном меню
  m_th = new std::thread([this] { dinamicMainMenu_(); });
}

ViewMenuGame::~ViewMenuGame() {
  delete ui;
  delete m_th;
  delete m_playing_field;
}

auto ViewMenuGame::paintEvent(QPaintEvent *event) -> void {
  QStyleOption opt;
  opt.initFrom(this);
  QPainter p(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
  QWidget::paintEvent(event);
}

void ViewMenuGame::closeEvent(QCloseEvent *event) {
  m_programm_exit = true;
  m_cv.notify_one();

  // дожидаемся закрытия потока
  m_th->join();
  event->accept();
}

auto ViewMenuGame::addButtonsPlayingFieldToGroup_() -> void {
  // первый ряд игрового поля
  m_group.addButton(ui->cell_0_0, 0);
  m_group.addButton(ui->cell_0_1, 1);
  m_group.addButton(ui->cell_0_2, 2);
  m_group.addButton(ui->cell_0_3, 3);
  m_group.addButton(ui->cell_0_4, 4);
  m_group.addButton(ui->cell_0_5, 5);
  // второй ряд игрового поля
  m_group.addButton(ui->cell_1_0, 6);
  m_group.addButton(ui->cell_1_1, 7);
  m_group.addButton(ui->cell_1_2, 8);
  m_group.addButton(ui->cell_1_3, 9);
  m_group.addButton(ui->cell_1_4, 10);
  m_group.addButton(ui->cell_1_5, 11);
  // третий ряд игрового поля
  m_group.addButton(ui->cell_2_0, 12);
  m_group.addButton(ui->cell_2_1, 13);
  m_group.addButton(ui->cell_2_2, 14);
  m_group.addButton(ui->cell_2_3, 15);
  m_group.addButton(ui->cell_2_4, 16);
  m_group.addButton(ui->cell_2_5, 17);
  // четвертый ряд игрового поля
  m_group.addButton(ui->cell_3_0, 18);
  m_group.addButton(ui->cell_3_1, 19);
  m_group.addButton(ui->cell_3_2, 20);
  m_group.addButton(ui->cell_3_3, 21);
  m_group.addButton(ui->cell_3_4, 22);
  m_group.addButton(ui->cell_3_5, 23);
  // пятый ряд игрового поля
  m_group.addButton(ui->cell_4_0, 24);
  m_group.addButton(ui->cell_4_1, 25);
  m_group.addButton(ui->cell_4_2, 26);
  m_group.addButton(ui->cell_4_3, 27);
  m_group.addButton(ui->cell_4_4, 28);
  m_group.addButton(ui->cell_4_5, 29);
  // шестой ряд игрового поля
  m_group.addButton(ui->cell_5_0, 30);
  m_group.addButton(ui->cell_5_1, 31);
  m_group.addButton(ui->cell_5_2, 32);
  m_group.addButton(ui->cell_5_3, 33);
  m_group.addButton(ui->cell_5_4, 34);
  m_group.addButton(ui->cell_5_5, 35);
}

void ViewMenuGame::installMainSettings_() {
  // установка заднего фона
  installBackgroundImage_();

  // установка размера иконок игрового поля
  for (int id_button = 0; id_button < 36; ++id_button) {
    m_group.button(id_button)->setIconSize(QSize(55, 55));
    m_group.button(id_button)->setStyleSheet(
        StyleHelper::getButtonUnFocusStyle());
  }
  installStyleButtonStartGame_();
  installStyleButtonAboutGame_();
  installStyleButtonBack_();
  installStyleLabelGameDescription_();
}

auto ViewMenuGame::installBackgroundImage_() -> void {
  this->setStyleSheet(StyleHelper::getBackgroundWidget());
  ui->tab_1->setStyleSheet(StyleHelper::getBackgroundPlayingField());
  ui->tab_2->setStyleSheet(StyleHelper::getBackgroundPlayingField());
}

auto ViewMenuGame::installStyleButtonStartGame_() -> void {
  // установка стиля кнопки
  ui->button_start_game->setStyleSheet(
      StyleHelper::getStartGameAboutGameButtonStyle());
  // установка иконоки для кнопки
  ui->button_start_game->setIcon(
      QIcon(StyleHelper::getPathStartGameButtonIcon()));
  ui->button_start_game->setIconSize(QSize(40, 40));
}

auto ViewMenuGame::installStyleButtonAboutGame_() -> void {
  // установка стиля кнопки
  ui->button_about_game->setStyleSheet(
      StyleHelper::getStartGameAboutGameButtonStyle());
  // установка иконоки для кнопки
  ui->button_about_game->setIcon(
      QIcon(StyleHelper::getPathAboutGameButtonIcon()));
  ui->button_about_game->setIconSize(QSize(40, 40));
}

auto ViewMenuGame::installStyleButtonBack_() -> void {
  // установка стиля кнопки
  ui->button_back->setStyleSheet(StyleHelper::getUndoButtonStyle());
  // установка иконоки для кнопки
  ui->button_back->setIcon(QIcon(StyleHelper::getPathToUndoIcon()));
  ui->button_back->setIconSize(QSize(40, 40));
}

auto ViewMenuGame::installStyleLabelGameDescription_() -> void {
  ui->label->setStyleSheet(StyleHelper::getStyleLabelAboutGame());
}

auto ViewMenuGame::dinamicMainMenu_() -> void {
  while (!m_programm_exit) {
    std::unique_lock<std::mutex> ul(m_mutex);

    m_cv.wait_for(ul, std::chrono::milliseconds(400),
                  [this] { return m_programm_exit; });

    int color = rand() % (kNoColor + 1);
    int id_button = rand() % 36;

    auto button = m_group.button(id_button);
    if (color == ColorStone::kBlue) {
      button->setIcon(QIcon(StyleHelper::getPathToBlueStoneIcon()));
    } else if (color == ColorStone::kGreen) {
      button->setIcon(QIcon(StyleHelper::getPathToGreenStoneIcon()));
    } else if (color == ColorStone::kRed) {
      button->setIcon(QIcon(StyleHelper::getPathToRedStoneIcon()));
    } else if (color == ColorStone::kYellow) {
      button->setIcon(QIcon(StyleHelper::getPathToYellowStoneIcon()));
    } else if (color == ColorStone::kPerple) {
      button->setIcon(QIcon(StyleHelper::getPathToPerpleStoneIcon()));
    } else if (color == ColorStone::kGray) {
      button->setIcon(QIcon(StyleHelper::getPathToGreyStoneIcon()));
    } else if (color == ColorStone::kSky) {
      button->setIcon(QIcon(StyleHelper::getPathToSkyStoneIcon()));
    } else if (color == ColorStone::kLightGreen) {
      button->setIcon(QIcon(StyleHelper::getPathToLightGreenStoneIcon()));
    } else if (color == ColorStone::kBlack) {
      button->setIcon(QIcon(StyleHelper::getPathToBlackStoneIcon()));
    } else if (color == ColorStone::kWhite) {
      button->setIcon(QIcon(StyleHelper::getPathToWhiteStoneIcon()));
    } else {
      bool free_cell = rand() % 2;
      if (free_cell) {
        button->setIcon(QIcon());
      } else {
        button->setIcon(QIcon(StyleHelper::getPathToBoxIcon()));
      }
    }
  }
}

void ViewMenuGame::on_button_about_game_clicked() {
  ui->tabWidget->setCurrentIndex(1);
  ui->button_about_game->hide();
  ui->button_start_game->hide();
  ui->comboBox->hide();
}

void ViewMenuGame::on_button_back_clicked() {
  ui->tabWidget->setCurrentIndex(0);
  ui->button_about_game->show();
  ui->button_start_game->show();
  ui->comboBox->show();
}

void ViewMenuGame::on_button_start_game_clicked() {
  // кнопка активна если включена страница с игровым полем
  if (m_playing_field) {
    delete m_playing_field;
  }

  int lvl_game = ui->comboBox->currentIndex() + 1;
  m_playing_field = new ViewPlayingField(lvl_game);

  connect(m_playing_field, &ViewPlayingField::close_playing_field, this,
          [this] {
            m_mutex.unlock();
            this->show();
          });

  m_mutex.lock();
  this->hide();

  m_playing_field->show();
}

}  // namespace TreasureHunt_2_0
QT_END_NAMESPACE
