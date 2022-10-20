//#include "view_game.h"

//#include <QMessageBox>
//#include <QStyleOption>
//#include <QTabBar>

//#include "style_helper.h"
//#include "ui_view_game.h"

//QT_BEGIN_NAMESPACE
//namespace TreasureHunt_2_0 {

//constexpr QSize icon_size = {55, 55};

//ViewGame::ViewGame(QWidget *parent)
//    : QWidget(parent), ui(new Ui::Widget), m_victory_dialog(new VictoryDialog) {
//  ui->setupUi(this);

//  // скрыл вкладки выбора и устанвил первую страницу таб бара
//  ui->tabWidget->tabBar()->hide();
//  ui->tabWidget->setCurrentIndex(0);

//  // добавления кнопок в группу
//  addButtonsPlayingFieldToGroup_();

//  // настройки по размеру иконок на игровом поле
//  mainSettings_();

//  // устанвока стиля игровог ополя главного меню
//  installStylePlayingFieldInMainMenu_();

//  connect(&m_group, SIGNAL(idClicked(int)), this,
//          SLOT(on_button_in_playing_field_clicked(int)));
//  connect(m_victory_dialog, SIGNAL(restart()), this,
//          SLOT(on_dialog_button_restart_clicked()));
//  connect(m_victory_dialog, SIGNAL(end()), this,
//          SLOT(on_dialog_button_end_clicked()));

//  // в другой поток уходит генерация игрового поля в главном меню
//  m_th = new std::thread([this] { dinamicMainMenu_(); });
//}

//ViewGame::~ViewGame() {
//  delete ui;
//  delete m_victory_dialog;
//  delete m_th;
//}

//auto ViewGame::paintEvent(QPaintEvent *event) -> void {
//  QStyleOption opt;
//  opt.initFrom(this);
//  QPainter p(this);
//  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
//  QWidget::paintEvent(event);
//}

//void ViewGame::closeEvent(QCloseEvent *event) {
//    m_programm_exit = true;

//    // разбудить поток галвного меню
//    m_is_game_start = false;
//    m_cv.notify_one();

//    // дожидаемся закрытия потока
//    m_th->join();
//    event->accept();
//}

//auto ViewGame::addButtonsPlayingFieldToGroup_() -> void {
//  // первый ряд игрового поля
//  m_group.addButton(ui->cell_0_0, CoordsPlayingField::k00);
//  m_group.addButton(ui->cell_0_1, CoordsPlayingField::k01);
//  m_group.addButton(ui->cell_0_2, CoordsPlayingField::k02);
//  m_group.addButton(ui->cell_0_3, CoordsPlayingField::k03);
//  m_group.addButton(ui->cell_0_4, CoordsPlayingField::k04);
//  // второй ряд игрового поля
//  m_group.addButton(ui->cell_1_0, CoordsPlayingField::k10);
//  m_group.addButton(ui->cell_1_1, CoordsPlayingField::k11);
//  m_group.addButton(ui->cell_1_2, CoordsPlayingField::k12);
//  m_group.addButton(ui->cell_1_3, CoordsPlayingField::k13);
//  m_group.addButton(ui->cell_1_4, CoordsPlayingField::k14);
//  // третий ряд игрового поля
//  m_group.addButton(ui->cell_2_0, CoordsPlayingField::k20);
//  m_group.addButton(ui->cell_2_1, CoordsPlayingField::k21);
//  m_group.addButton(ui->cell_2_2, CoordsPlayingField::k22);
//  m_group.addButton(ui->cell_2_3, CoordsPlayingField::k23);
//  m_group.addButton(ui->cell_2_4, CoordsPlayingField::k24);
//  // четвертый ряд игрового поля
//  m_group.addButton(ui->cell_3_0, CoordsPlayingField::k30);
//  m_group.addButton(ui->cell_3_1, CoordsPlayingField::k31);
//  m_group.addButton(ui->cell_3_2, CoordsPlayingField::k32);
//  m_group.addButton(ui->cell_3_3, CoordsPlayingField::k33);
//  m_group.addButton(ui->cell_3_4, CoordsPlayingField::k34);
//  // пятый ряд игрового поля
//  m_group.addButton(ui->cell_4_0, CoordsPlayingField::k40);
//  m_group.addButton(ui->cell_4_1, CoordsPlayingField::k41);
//  m_group.addButton(ui->cell_4_2, CoordsPlayingField::k42);
//  m_group.addButton(ui->cell_4_3, CoordsPlayingField::k43);
//  m_group.addButton(ui->cell_4_4, CoordsPlayingField::k44);
//}

//void ViewGame::mainSettings_() {
//    // установка заднего фона
//    installBackgroundImage_();

//    // установка размера иконок игрового поля
//    for (int id_button = CoordsPlayingField::k00;
//         id_button <= CoordsPlayingField::k44; ++id_button) {
//        m_group.button(id_button)->setIconSize(icon_size);
//    }

//    // установка размера иконок над игровым поле
//    ui->cell_upper_left->setIconSize(icon_size);
//    ui->cell_upper_left->setStyleSheet(
//        StyleHelper::getButtonStyleUnderGameField());

//    ui->cell_upper_medium->setIconSize(icon_size);
//    ui->cell_upper_medium->setStyleSheet(
//        StyleHelper::getButtonStyleUnderGameField());

//    ui->cell_upper_right->setIconSize(icon_size);
//    ui->cell_upper_right->setStyleSheet(
//        StyleHelper::getButtonStyleUnderGameField());

//    // установка иконок и стиля для кнопок не являющих игровым полем
//    installStyleButtonStartGame_();
//    installStyleButtonAboutGame_();
//    installStyleButtonBack_();

//    // установка стилей для лэйбла описания игры
//    installStyleLabelGameDescription_();
//}


//auto ViewGame::installBackgroundImage_() -> void {
//    this->setStyleSheet(StyleHelper::getBackgroundWidget());
//    ui->tab_1->setStyleSheet(StyleHelper::getBackgroundPlayingField());
//    ui->tab_2->setStyleSheet(StyleHelper::getBackgroundPlayingField());
//}


//auto ViewGame::installStyleButtonStartGame_() -> void {
//    // установка стиля кнопки
//    ui->button_start_game->setStyleSheet(
//        StyleHelper::getStartGameAboutGameButtonStyle());
//    // установка иконоки для кнопки
//    ui->button_start_game->setIcon(QIcon(StyleHelper::getStartGameButtonIcon()));
//    ui->button_start_game->setIconSize(QSize(40, 40));
//}

//auto ViewGame::installStyleButtonAboutGame_() -> void {
//    // установка стиля кнопки
//    ui->button_about_game->setStyleSheet(
//        StyleHelper::getStartGameAboutGameButtonStyle());
//    // установка иконоки для кнопки
//    ui->button_about_game->setIcon(QIcon(StyleHelper::getAboutGameButtonIcon()));
//    ui->button_about_game->setIconSize(QSize(40, 40));
//}

//auto ViewGame::installStyleButtonBack_() -> void {
//    // установка стиля кнопки
//    ui->button_back->setStyleSheet(
//        StyleHelper::getUndoButtonStyle());
//    // установка иконоки для кнопки
//    ui->button_back->setIcon(QIcon(StyleHelper::getPathToUndoIcon()));
//    ui->button_back->setIconSize(QSize(40, 40));
//}

//auto ViewGame::installStyleLabelGameDescription_() -> void {
//  ui->label->setStyleSheet(StyleHelper::getStyleLabelAboutGame());
//}


//auto ViewGame::dinamicMainMenu_() -> void {
//  while (!m_programm_exit) {
//    std::unique_lock<std::mutex> ul(m_mutex);

//    m_cv.wait(ul, [this] { return !m_is_game_start; });

//    int color = rand() % (Color::kRed + 1);
//    int id_button = rand() % (CoordsPlayingField::k44 + 1);

//    auto button = m_group.button(id_button);
//    if (color == Color::kBlue) {
//      button->setIcon(QIcon(StyleHelper::getPathToSaphfireIcon()));
//    } else if (color == Color::kGreen) {
//      button->setIcon(QIcon(StyleHelper::getPathToEmeraldIcon()));
//    } else if (color == Color::kRed) {
//      button->setIcon(QIcon(StyleHelper::getPathToRubyIcon()));
//    } else {
//      bool free_cell = rand() % 2;
//      if (free_cell) {
//        button->setIcon(QIcon());
//      } else {
//        button->setIcon(QIcon(StyleHelper::getPathToBoxIcon()));
//      }
//    }
//    if (!m_programm_exit) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(400));
//    }
//  }
//}

//void ViewGame::installStylePlayingFieldInMainMenu_() {
//    // установка стилей кнопок на игровом поле
//      for (int id_button = CoordsPlayingField::k00;
//           id_button <= CoordsPlayingField::k44; ++id_button) {
//        // установка стиля для кнопок без нажатия
//        m_group.button(id_button)->setStyleSheet(
//            StyleHelper::getInactiveButtonStyle());
//      }
//}

//void ViewGame::installStylePlayingFieldInProcessGame_() {
//  // генерация игрового поля
//  m_controller.GenerateField();

//  for (int id_button = CoordsPlayingField::k00;
//       id_button <= CoordsPlayingField::k44; ++id_button) {
//    int color_cell = m_controller.GetColorCell(id_button);
//    auto button = m_group.button(id_button);
//    button->setStyleSheet(
//        StyleHelper::getInactiveButtonStyle());
//    if (color_cell == Color::kRed) {
//      // иконка рубина
//      button->setIcon(
//          QIcon(StyleHelper::getPathToRubyIcon()));
//      // активный стиль
//      button->setStyleSheet(
//          StyleHelper::getReleasedButtonStyle());
//    } else if (color_cell == Color::kGreen) {
//      // иконка изумруда
//       button->setIcon(
//          QIcon(StyleHelper::getPathToEmeraldIcon()));
//      // активный стиль
//      button->setStyleSheet(
//          StyleHelper::getReleasedButtonStyle());
//    } else if (color_cell == Color::kBlue) {
//      // иконка сапфира
//      m_group.button(id_button)->setIcon(
//          QIcon(StyleHelper::getPathToSaphfireIcon()));
//      // активный стиль
//      button->setStyleSheet(
//          StyleHelper::getReleasedButtonStyle());
//    } else {
//        // неактивный стиль
//        button->setStyleSheet(
//            StyleHelper::getInactiveButtonStyle());
//        // если это ячейка неактивного поля, то ставлю иконку сундука
//        if (m_controller.IsSelectedBlockCell(id_button)) {
//            m_group.button(id_button)->setIcon(
//              QIcon(StyleHelper::getPathToBoxIcon()));
//        } else {
//            m_group.button(id_button)->setIcon(QIcon());
//        }
//    }
//  }

//  // установка иконок кнопок над игровым полем
//  installIconToCellInRowOverPlayingField_(
//      m_controller.GetPosRedChipInExpectedRow(),
//      StyleHelper::getPathToRubyIcon());

//  installIconToCellInRowOverPlayingField_(
//      m_controller.GetPosGreenChipInExpectedRow(),
//      StyleHelper::getPathToEmeraldIcon());

//  installIconToCellInRowOverPlayingField_(
//      m_controller.GetPosBlueChipInExpectedRow(),
//      StyleHelper::getPathToSaphfireIcon());
//}

//auto ViewGame::installIconToCellInRowOverPlayingField_(
//    int index_in_row, QString const &path_to_icon) -> void {
//  if (index_in_row == 0) {
//    ui->cell_upper_left->setIcon(QIcon(path_to_icon));
//  } else if (index_in_row == 2) {
//    ui->cell_upper_medium->setIcon(QIcon(path_to_icon));
//  } else {
//    ui->cell_upper_right->setIcon(QIcon(path_to_icon));
//  }
//}

//auto ViewGame::clearAllIcons_() -> void {
//  // очистка иконок кнопок игрово поля
//  for (int i = CoordsPlayingField::k00; i <= CoordsPlayingField::k44; ++i) {
//    m_group.button(i)->setIcon(QIcon());
//  }

//  // очистка иконок кнопок над игровым полем
//  ui->cell_upper_left->setIcon(QIcon());
//  ui->cell_upper_medium->setIcon(QIcon());
//  ui->cell_upper_right->setIcon(QIcon());
//}

//void ViewGame::on_button_about_game_clicked() {
//  ui->tabWidget->setCurrentIndex(1);
//}

//void ViewGame::on_button_back_clicked() { ui->tabWidget->setCurrentIndex(0); }

//void ViewGame::on_button_start_game_clicked() {
//  // кнопка активна если включена страница с игровым полем
//  if (ui->tabWidget->currentIndex() == 0) {
//    if (m_is_game_start == false) {

//      // разбудить поток главного меню
//      m_is_game_start = true;
//      m_cv.notify_one();

//      // очистка поля
//      clearAllIcons_();
//      // замена иконки кнопки start_game
//      ui->button_start_game->setIcon(
//          QIcon(StyleHelper::getStopGameButtonIcon()));
//      // генерируем игровоое поле
//      installStylePlayingFieldInProcessGame_();
//    } else {
//      // очистка всех иконок игрового поля
//      clearAllIcons_();
//      // установка стиля главного меню
//      installStylePlayingFieldInMainMenu_();

//      // замена иконки кнопки start_game
//      ui->button_start_game->setIcon(
//          QIcon(StyleHelper::getStartGameButtonIcon()));

//      // разбудить поток галвного меню
//      m_is_game_start = false;
//      m_cv.notify_one();
//    }
//  }
//}


//void ViewGame::on_button_in_playing_field_clicked(int id_button) {
//  if (m_is_game_start == true) {
//    // если ячейка является возможным шагом
//    if (m_controller.IsCellPossibleStep(id_button) == true) {
//      //  сохраняем координаты предидущего клика
//      int id_button_prev = m_controller.GetCurrentCoord();

//      // меняю стиль подсвеченных клеток возможных шагов на обычный
//      changeStyleInactiveButtons_();

//      //  меняем местами клетки в модели
//      m_controller.SwapCells(id_button);

//      // меняем иконки местами
//      m_group.button(id_button)
//          ->setStyleSheet(StyleHelper::getReleasedButtonStyle());
//      m_group.button(id_button_prev)
//          ->setStyleSheet(StyleHelper::getInactiveButtonStyle());

//      m_group.button(id_button)->setIcon(
//          m_group.button(id_button_prev)->icon());
//      m_group.button(id_button_prev)->setIcon(QIcon());

//      // проверяем наступила ли победа в игре
//      if (m_controller.IsVictoryGame()) {
//        // вызываем диалоговое окно с ифнормацией о завершении игры
//        m_victory_dialog->exec();
//      }
//    } else {
//      // меняем текущую выбранную ячейку и пересчитываем возможные шаги
//      if (m_controller.IsSelectedChipCell(id_button)) {
//        int id_button_prev = m_controller.GetCurrentCoord();

//        // если ячейка былы выбрана, то меняем ей стили на стандартный
//        if (id_button_prev != -1) {
//          m_group.button(id_button_prev)
//              ->setStyleSheet(StyleHelper::getReleasedButtonStyle());
//        }

//        // меняю стиль подсвеченных клеток возможных шагов на обычный
//        changeStyleInactiveButtons_();

//        // меняем стиль новой выбранной кнопки
//        m_group.button(id_button)->setStyleSheet(
//            StyleHelper::getPressedButtonChooseStyle());

//        // рассчитываем новые возможные шаги
//        m_controller.ChangePossibleStepsChipInPlayingField_(id_button);
//        // меняем их стиль отобрадения на поле
//        changeStylePossibleStepsButtons_();
//      }
//    }
//  }
//}

//void ViewGame::on_dialog_button_restart_clicked() {
//  // очистка поля
//  clearAllIcons_();
//  // генерируем игрвоое поле
//  installStylePlayingFieldInProcessGame_();
//  // скрываем диалоговое окно
//  m_victory_dialog->hide();
//}

//void ViewGame::on_dialog_button_end_clicked() {
//  // очистка поля
//  clearAllIcons_();

//  // замена иконки кнопки start_game
//  ui->button_start_game->setIcon(QIcon(StyleHelper::getStartGameButtonIcon()));

//  // разбудить поток галвного меню
//  m_is_game_start = false;
//  m_cv.notify_one();

//  // скрываем диалоговое окно
//  m_victory_dialog->hide();
//}

//void ViewGame::changeStylePossibleStepsButtons_() {
//  std::vector<int> id_buttons = m_controller.GetVectorPossibleSteps();
//  for (int &id : id_buttons) {
//    m_group.button(id)->setStyleSheet(StyleHelper::getPressedButtonStyle());
//  }
//}

//void ViewGame::changeStyleInactiveButtons_() {
//  std::vector<int> id_buttons = m_controller.GetVectorPossibleSteps();
//  for (int &id : id_buttons) {
//    m_group.button(id)->setStyleSheet(StyleHelper::getInactiveButtonStyle());
//  }
//}

//}  // namespace TreasureHunt_2_0
//QT_END_NAMESPACE
