#ifndef GAME_MECHANICS_HPP
#define GAME_MECHANICS_HPP

#include <map>
#include <random>

#include "block_cell.hpp"
#include "chip_cell.hpp"
#include "free_cell.hpp"
#include "playing_field.hpp"

namespace TreasureHunt_2_0 {

/*=== Класс Механики Игры ===*/
class GameMechanics {
 public:
  GameMechanics() : m_field(nullptr) { srand(time(0)); }
  GameMechanics(GameMechanics const &other) = delete;
  GameMechanics(GameMechanics &&other) = delete;
  ~GameMechanics() { delete m_field; }

  auto operator=(GameMechanics const &other) -> GameMechanics & = delete;
  auto operator=(GameMechanics &&other) -> GameMechanics & = delete;

  // генерация игрового поля
  auto GenerateField(uint8_t size_field) -> void {
    // очищаем поле если ранее было сгенерировано
    if (IsGenerateField()) {
      ClearField();
    }

    m_field = new PlayingField(size_field);

    // генерирую какие цвета будут и сколько в зависимости от размеров поля
    std::vector<int> colors = GenerateRandomColorsChip_();

    // заполняю ожидаемую строку (над игровым полем)
    for (int column = 0, index_color = 0; column < size_field; ++column) {
      if (column % 2 == 0) {
        m_expected_row.insert({column, colors[index_color]});
        ++index_color;
      }
    }

    // перед заполнением игрового поля каждого цвета должго быть SIZE_FIELD
    std::vector<std::pair<u_int8_t, u_int8_t>> possible_colors(colors.size());
    for (int i = 0; i < (int)possible_colors.size(); ++i) {
      possible_colors[i] = {colors[i], size_field};
    }

    // генерирую игровое поле`
    GeneratePlayingField_(possible_colors);

    // изначально выбранная ячейка в левом верхнем углу
    m_current_coord = {0, 0};
  }

  auto ClearField() -> void {
    delete m_field;
    m_field = nullptr;
    m_expected_row.clear();
  }

  auto GetSizeField() const -> uint8_t {
    if (!IsGenerateField()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->getSizeField();
  }

  auto StepUp() -> bool {
    int row = m_current_coord.first;
    int column = m_current_coord.second;
    if (row > 0 && !IsSelectedBlockCell(row - 1, column)) {
      m_current_coord.first -= 1;
      return true;
    }
    return false;
  }

  auto StepRight() -> bool {
    int row = m_current_coord.first;
    int column = m_current_coord.second;
    if (column < m_field->getSizeField() - 1 &&
        !IsSelectedBlockCell(row, column + 1)) {
      m_current_coord.second += 1;
      return true;
    }
    return false;
  }

  auto StepDown() -> bool {
    int row = m_current_coord.first;
    int column = m_current_coord.second;
    if (row < m_field->getSizeField() - 1 &&
        !IsSelectedBlockCell(row + 1, column)) {
      m_current_coord.first += 1;
      return true;
    }
    return false;
  }

  auto StepLeft() -> bool {
    int row = m_current_coord.first;
    int column = m_current_coord.second;
    if (column > 0 && !IsSelectedBlockCell(row, column - 1)) {
      m_current_coord.second -= 1;
      return true;
    }
    return false;
  }

  auto SwapPlacesWithTopCell() -> bool {
    int row = m_current_coord.first;
    int column = m_current_coord.second;
    if (row > 0 && IsSelectedFreeCell(row - 1, column)) {
      SwapCell(row - 1, column);
      return true;
    }
    return false;
  }

  auto SwapPlacesWithRightCell() -> bool {
    int row = m_current_coord.first;
    int column = m_current_coord.second;
    if (column < m_field->getSizeField() - 1 &&
        IsSelectedFreeCell(row, column + 1)) {
      SwapCell(row, column + 1);
      return true;
    }
    return false;
  }

  auto SwapPlacesWithBottomCell() -> bool {
    int row = m_current_coord.first;
    int column = m_current_coord.second;
    if (row < m_field->getSizeField() - 1 &&
        IsSelectedFreeCell(row + 1, column)) {
      SwapCell(row + 1, column);
      return true;
    }
    return false;
  }

  auto SwapPlacesWithLeftCell() -> bool {
    int row = m_current_coord.first;
    int column = m_current_coord.second;
    if (column > 0 && IsSelectedFreeCell(row, column - 1)) {
      SwapCell(row, column - 1);
      return true;
    }
    return false;
  }

  // поменять местами ячейки игрвого поля
  auto SwapCell(int row, int column) -> void {
    std::swap(
        m_field->operator[](m_current_coord.first)[m_current_coord.second],
        m_field->operator[](row)[column]);
    CalcStepsAroundCell(row, column);
  }

  // сохранить в вектор возможные перемещения фишки на игровом поле
  auto CalcStepsAroundCell(int row, int column) -> void {
    //
    if (IsSelectedChipCell(row, column)) {
      //  сохраняем текущие координаты
      m_current_coord = {row, column};

      // очищаем предидущие возможные перемещения
      m_possible_steps.clear();

      if ((row > 0) && IsSelectedFreeCell(row - 1, column)) {
        m_possible_steps.push_back({row - 1, column});
      }

      if ((row < m_field->getSizeField() - 1) &&
          IsSelectedFreeCell(row + 1, column)) {
        m_possible_steps.push_back({row + 1, column});
      }

      if ((column > 0) && IsSelectedFreeCell(row, column - 1)) {
        m_possible_steps.push_back({row, column - 1});
      }

      if ((column < m_field->getSizeField() - 1) &&
          IsSelectedFreeCell(row, column + 1)) {
        m_possible_steps.push_back({row, column + 1});
      }
    }
  }

  auto ChangeCurrentCoord(int row, int column) -> bool {
    int size = m_field->getSizeField();
    if (row >= 0 && row < size && column >= 0 && column < size) {
      // если это игровая фишка пересчитываем возможные шаги
      m_current_coord = {row, column};
      return true;
    }
    return false;
  }

  // получить координаты выбранной игровой фишки на поле
  auto GetCurrentCoord() const -> std::pair<int, int> {
    if (!IsGenerateField()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_current_coord;
  }

  // является ли ячейка возможным перемещением
  auto IsCellPossibleStep(int row, int column) const -> bool {
    for (auto &[m_row, m_column] : m_possible_steps) {
      if (m_row == row && m_column == column) {
        return true;
      }
    }
    return false;
  }

  // является ли выбранная ячейка неактивной
  auto IsSelectedBlockCell(int row, int column) const -> bool {
    if (!IsGenerateField()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->operator[](row)[column]->TypeCell() == TypeCell::kBlock;
  }

  // является ли выбранная ячейка свободной
  auto IsSelectedFreeCell(int row, int column) const -> bool {
    if (!IsGenerateField()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->operator[](row)[column]->TypeCell() == TypeCell::kFree;
  }

  // является ли выбранная ячейка игровой фишкой
  auto IsSelectedChipCell(int row, int column) const -> bool {
    if (!IsGenerateField()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->operator[](row)[column]->TypeCell() == TypeCell::kChip;
  }

  // получить цвет выбранной ячейки
  auto GetColorCell(int row, int column) const -> int {
    if (!IsGenerateField()) {
      throw std::out_of_range("Field is not generate");
    }
    return m_field->operator[](row)[column]->GetColor();
  }

  // получить цвет клетки ожидаемой строки по индексу
  auto GetColorExpectedRow() const -> std::map<int, int> {
    return m_expected_row;
  }

  // проверка на победу в игре
  auto IsGameVictory() const -> bool {
    for (auto &[index_column, color] : m_expected_row) {
      for (int row = 0; row < m_field->getSizeField(); ++row) {
        if (color != m_field->operator[](row)[index_column]->GetColor()) {
          return false;
        }
      }
    }
    return true;
  }

  auto IsGenerateField() const -> bool { return m_field != nullptr; }

 protected:
  auto GenerateRandomColorsChip_() -> std::vector<int> {
    std::vector<int> colors(ColorStone::kNoColor);
    for (int i = ColorStone::kBlue; i < ColorStone::kNoColor; ++i) {
      colors[i] = i;
    }

    // определяем сколько различных цветных фишек должно быть на поле
    int num_colors{};
    for (int i = LevelGame::kFirst; i <= LevelGame::kEighth; ++i) {
      if (kFieldSizeInTheLevel[i] == m_field->getSizeField()) {
        num_colors = kNumberOfDifferentChipsInTheLevel[i];
        break;
      }
    }
    //      std::cout << "wtf " << num_colors << std::endl;

    std::vector<int> result_colors(num_colors);
    for (int i = 0; i < num_colors; ++i) {
      int index = rand() % colors.size();
      result_colors[i] = colors[index];
      auto iter = colors.begin();
      for (int i = 0; i < index; ++i) {
        ++iter;
      }
      colors.erase(iter);
    }

    return result_colors;
  }

  auto GeneratePlayingField_(
      std::vector<std::pair<u_int8_t, u_int8_t>> &possible_colors) -> void {
    int size_filed = m_field->getSizeField();
    for (int row = 0; row < size_filed; ++row) {
      for (int column = 0; column < size_filed; ++column) {
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

 private:
  std::pair<int, int> m_current_coord;
  std::vector<std::pair<int, int>> m_possible_steps;
  PlayingField *m_field;
  std::map<int, int> m_expected_row;
};

}  // namespace TreasureHunt_2_0

#endif  // GAME_MECHANICS_HPP
