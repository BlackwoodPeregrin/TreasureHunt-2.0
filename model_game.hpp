#include <iostream>
#include <map>
#include <random>
#include <set>
#include <vector>

namespace InSearchOfTreasure {

enum TypeCell { kChip, kBlock, kFree };  // тип клетки

enum ColorStone {
    kBlue,
    kGreen,
    kRed,
    kYellow,
    kPerple,
    kGray,
    kBlack,
    kSky,
    kLightGreen,
    kNoColor
};  // цвет игровой фишки

// кол-во различных цветов игровых фишек
constexpr u_int8_t kNumbersOfColorsChips = 9;

enum LevelGame {
    kFirst,
    kSecond,
    kThird,
    kFourth,
    kFifth,
    kSixth,
    kSeventh,
    kEighth,
    kNinth
}; // уровни игры

// кол-во различных уровней игры
constexpr u_int8_t kLevelsGame = 9;

//  размерность игрового поля с 1 до 9 уровней игры (поле квадратное)
constexpr u_int8_t kFieldSizeInTheLevel[kLevelsGame] {
    3, 5, 7, 9, 11, 13, 15, 17, 19
};

// общее кол-во клеток игрового поля с 1 до 9 уровней игры
constexpr u_int16_t kTotalNumberOfCellsInTheLevel[kLevelsGame] {
    9, 25, 49, 81, 121, 169, 225, 289, 361
};

// кол-во игровых фишек различного цвета с 1 до 9 уровней игры
constexpr u_int8_t kNumberOfDifferentChipsInTheLevel[kLevelsGame] {
    2, 3, 4, 5, 6, 7, 8, 9, 10
};

// суммарное кол-во фишек каждого цвета с 1 до 9 уровней
constexpr u_int8_t kNumberOfTitlesEachColorInTheLevel[kLevelsGame] {
    3, 5, 7, 9, 11, 13, 15, 17, 19
};

// кол-во неактивных клеток с 1 до 9 уровней
constexpr u_int8_t kNumberOfInactiveCellsInTheLevel[kLevelsGame] {
    2, 6, 12, 20, 30, 42, 56, 72, 90
};

// кол-во свободных клеток с 1 до 9 уровней
constexpr u_int8_t kNumberOfFreeCellsInTheLevel[kLevelsGame] {
    1, 4, 9, 16, 25, 36, 49, 64, 81
};



/*=== Абстрактный класс клетки игрового поля ===*/
class AbstractCell {
 public:
  AbstractCell(int type_cell) : m_type_cell(type_cell) {}
  virtual ~AbstractCell() = default;

  auto TypeCell() -> int { return m_type_cell; }
  virtual auto GetColor() const -> int = 0;

 private:
  int m_type_cell;
};

/*=== Класс Фишек Игрового Поля ===*/
class ChipCell : public AbstractCell {
 public:
  ChipCell(int color_chip)
      : AbstractCell(TypeCell::kChip), m_color_chip(color_chip) {}
  auto GetColor() const -> int override { return m_color_chip; }
  ~ChipCell() = default;

  auto operator==(ChipCell const &other) const -> bool {
    return m_color_chip == other.m_color_chip;
  }

  auto operator!=(ChipCell const &other) const -> bool {
    return !operator==(other);
  }

 private:
  int m_color_chip;
};

/*=== Класс Неактивных Клеток Игрового Поля ===*/
class BlockCell : public AbstractCell {
 public:
  BlockCell() : AbstractCell(TypeCell::kBlock) {}
  auto GetColor() const -> int override { return ColorStone::kNoColor; }
  ~BlockCell() = default;
};

/*=== Класс Свободных Клеток Игрового Поля ===*/
class FreeCell : public AbstractCell {
 public:
  FreeCell() : AbstractCell(TypeCell::kFree) {}
  auto GetColor() const -> int override { return ColorStone::kNoColor; }
  ~FreeCell() = default;
};

/*=== Класс Игрового Поля ===*/
template <uint8_t SIZE_FIELD>
class PlayingField {
 public:
  PlayingField() : m_rows(SIZE_FIELD), m_columns(SIZE_FIELD) { InizialiseField_(); }
  PlayingField(PlayingField const &other) = delete;
  PlayingField(PlayingField &&other) = delete;
  ~PlayingField() { ClearField_(); }

  auto operator=(PlayingField const &other) -> PlayingField & = delete;
  auto operator=(PlayingField &&other) -> PlayingField & = delete;

  auto getRows() const -> int { return m_rows; }
  auto getColumns() const -> int { return m_columns; }

  auto operator[](int index_row) -> std::vector<AbstractCell *> & {
    if (index_row >= m_rows && index_row < 0) {
      throw std::out_of_range("operator[]:");
    }
    return m_cell[index_row];
  }

 protected:
  auto InizialiseField_() -> void {
    for (int row = 0; row < m_rows; ++row) {
      m_cell.push_back(std::vector<AbstractCell *>(m_columns));
      for (int column = 0; column < m_columns; ++column) {
        m_cell[row][column] = nullptr;
      }
    }
  }

  auto ClearField_() -> void {
    for (auto &row: m_cell) {
      for (auto &column : row) {
        delete column;
      }
    }
  }

 private:
  int m_rows;
  int m_columns;
  // клетки на игровом поле
  std::vector<std::vector<AbstractCell *>> m_cell;
};

/*=== Класс Механики Игры ===*/
template <uint8_t SIZE_FIELD>
class GameMechanics {
 public:
  GameMechanics() : m_field(nullptr), m_field_generate(false) {
    srand(time(0));
  }
  GameMechanics(GameMechanics const &other) = delete;
  GameMechanics(GameMechanics &&other) = delete;
  ~GameMechanics() { delete m_field; }

  auto operator=(GameMechanics const &other) -> GameMechanics & = delete;
  auto operator=(GameMechanics &&other) -> GameMechanics & = delete;

  // генерация игрового поля
  auto GenerateField() -> void {
    // очищаем поле если ранее было сгенерировано
    if (IsGenerateField_()) {
      delete m_field;
    }
    m_field = new PlayingField<SIZE_FIELD>;

    // генериурем какие цвета будут и сколько в зависимости от размеров поля
    std::vector<std::pair<u_int8_t, u_int8_t>> possible_colors;
    GenerateRandomColorsChip_(possible_colors);

    std::vector<u_int8_t> colors(possible_colors.size());
    for (int i = 0; i < possible_colors.size(); ++i) {
        colors[i] = possible_colors[i].first;
    }

    // генерация игрового поля
    for (int row = 0; row < SIZE_FIELD; ++row) {
      for (int column = 0; column < SIZE_FIELD; ++column) {
        if (column % 2 == 0) {
            // генерируем цветную фишку
            m_field->operator[](row)[column] =
                new ChipCell(GenerateColorChip_(possible_colors));
        } else if (column % 2 != 0 && row % 2 == 0) {
            // генерируем неактивную клетку
            m_field->operator[](row)[column] = new BlockCell;
        } else {
            // генерируем свободную клетку
            m_field->operator[](row)[column] = new FreeCell;
        }
      }
    }

    // генерация финальной строки над полем
    GenerateExpectedRow_(colors);

    m_field_generate = true;
  }

  auto ClearField() -> void {
    delete m_field;
    m_field = nullptr;
    m_field_generate = false;
  }

  // поменять местами ячейки игрвого поля
  auto SwapCells(int row, int column) -> void {
    if (IsSelectedFreeCell(row, column)) {
      std::swap(
          m_field->operator[](m_current_coord.first)[m_current_coord.second],
          m_field->operator[](row)[column]);
      // очищаем предидущие возможные перемещения
      m_possible_steps.clear();
      m_current_coord = {-1, -1};
    }
  }

  // сохранить в вектор возможные перемещения фишки на игровом поле
  auto ChangePossibleStepsChipInPlayingField(int row, int column) -> void {
    //
    if (IsSelectedChipCell(row, column)) {
      //  сохраняем текущие координаты
      m_current_coord = {row, column};
      // очищаем предидущие возможные перемещения
      m_possible_steps.clear();

      if ((row > 0) && IsSelectedFreeCell(row - 1, column)) {
        m_possible_steps.push_back({row - 1, column});
      }

      if ((row < kRowsField - 1) && IsSelectedFreeCell(row + 1, column)) {
        m_possible_steps.push_back({row + 1, column});
      }

      if ((column > 0) && IsSelectedFreeCell(row, column - 1)) {
        m_possible_steps.push_back({row, column - 1});
      }

      if ((column < kColumnsField - 1) && IsSelectedFreeCell(row, column + 1)) {
        m_possible_steps.push_back({row, column + 1});
      }
    }
  }

  auto GetVectorPossibleSteps() const -> std::vector<std::pair<int, int>> {
    return m_possible_steps;
  }

  // получить координаты выбранной игровой фишки на поле
  auto GetCurrentCoord() const -> std::pair<int, int> {
    return m_current_coord;
  }

  // является ли ячейка возможным перемещением
  auto IsCellPossibleStep(int row, int column) const -> bool {
    for (auto &coord : m_possible_steps) {
      if (coord.first == row && coord.second == column) {
        return true;
      }
    }
    return false;
  }

  // является ли выбранная ячейка неактивной
  auto IsSelectedBlockCell(int row, int column) const -> bool {
    if (!IsGenerateField_()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->operator[](row)[column]->TypeCell() == TypeCell::kBlock;
  }

  // является ли выбранная ячейка свободной
  auto IsSelectedFreeCell(int row, int column) const -> bool {
    if (!IsGenerateField_()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->operator[](row)[column]->TypeCell() == TypeCell::kFree;
  }

  // является ли выбранная ячейка игровой фишкой
  auto IsSelectedChipCell(int row, int column) const -> bool {
    if (!IsGenerateField_()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->operator[](row)[column]->TypeCell() == TypeCell::kChip;
  }

  // получить цвет выбранной ячейки
  auto GetColorCell(int row, int column) const -> int {
    if (!IsGenerateField_()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->operator[](row)[column]->GetColor();
  }

  auto GetPosRedChipInExpectedRow() const -> int {
    auto iter = m_expected_row.find(ColorStone::kRed);
    return (*iter).second;
  }

  auto GetPosGreenChipInExpectedRow() const -> int {
    auto iter = m_expected_row.find(ColorStone::kGreen);
    return (*iter).second;
  }

  auto GetPosBlueChipInExpectedRow() const -> int {
    auto iter = m_expected_row.find(ColorStone::kBlue);
    return (*iter).second;
  }

  // проверка на победу в игре
  auto IsGameVictory() const -> bool {
    for (auto [color, index_column] : m_expected_row) {
      for (int row = 0; row < kRowsField; ++row) {
        if (color != m_field->operator[](row)[index_column]->GetColor()) {
          return false;
        }
      }
    }
    return true;
  }

 protected:

  auto GenerateRandomColorsChip_(std::vector<std::pair<u_int8_t, u_int8_t>> &possible_colors) -> void {
      std::set<u_int8_t> colors;
      for (int color = ColorStone::kBlue;
           color < ColorStone::kNoColor; ++color) {
          colors.insert(color);
      }

      // определяем ссколько цветов должно быть на поле
      u_int8_t num_colors{};
      for (int i = LevelGame::kFirst; i <= LevelGame::kNinth; ++i) {
          if (kFieldSizeInTheLevel[i] == SIZE_FIELD) {
              num_colors = kNumberOfDifferentChipsInTheLevel[i];
              break;
          }
      }

      std::vector<std::pair<u_int8_t, u_int8_t>> result(num_colors);
      for (int i = 0; i < num_colors; ++i) {
          int index_color = rand() % colors.size();
          auto iter = colors.find(index_color);
          result[i] = {*iter, SIZE_FIELD};
          colors.erase(iter);
      }

      // переносим данные в поступивший вектор
      possible_colors = std::move(result);
  }

  auto GenerateColorChip_(
      std::vector<std::pair<u_int8_t, u_int8_t>> &possible_colors) -> u_int8_t {
    //
    u_int8_t index_color = rand() % possible_colors.size();
    u_int8_t color = possible_colors[index_color].first;
    if (--possible_colors[index_color].second == 0) {
      auto iter = possible_colors.begin();
      for (int i = 0; i < index_color; ++i) {
        ++iter;
      }
      possible_colors.erase(iter);
    }
    return color;
  }

  auto GenerateExpectedRow_(std::vector<std::pair<u_int8_t, u_int8_t>> &possible_colors) -> void {
    // если была сгенерирвоана, то очищаем предидущую генерацию
    if (!m_expected_row.empty()) {
      m_expected_row.clear();
    }
    //


    int colors[ColorStone::kRed]{ColorStone::kBlue, ColorStone::kGreen, ColorStone::kRed};
    std::set<int> pos_chip_in_row({0, 2, 4});

    for (int i = 0; i < ColorStone::kRed; ++i) {
      int r_index = rand() % pos_chip_in_row.size();
      auto iter = pos_chip_in_row.begin();
      for (int j = 0; j < r_index; ++j) {
        ++iter;
      }
      m_expected_row.insert({colors[i], *iter});
      // удаление элемента из списка генерации
      pos_chip_in_row.erase(iter);
    }
  }

  auto IsBlockCell_(int row, int column) const -> bool {
    for (auto coord : kCoordBlockCells) {
      if (coord.first == row && coord.second == column) {
        return true;
      }
    }
    return false;
  }

  auto IsFreeCell_(int row, int column) const -> bool {
    for (auto coord : kCoordFreeCells) {
      if (coord.first == row && coord.second == column) {
        return true;
      }
    }
    return false;
  }

  auto IsGenerateField_() const -> bool { return m_field_generate; }

 private:
  std::pair<int, int> m_current_coord{-1, -1};
  std::vector<std::pair<int, int>> m_possible_steps;
  PlayingField<SIZE_FIELD> *m_field;
  std::map<int, int> m_expected_row;
  bool m_field_generate;
};

}  // namespace InSearchOfTreasure
