#ifndef CONTROLLER_GAME_HPP
#define CONTROLLER_GAME_HPP

#include "../model/game_mechanics.hpp"

namespace InSearchOfTreasure_2_0 {

// контроллер реализован в паттерне Singletone
class ControllerGame {
 private:
  ControllerGame() = default;
  ControllerGame(ControllerGame const &other) = delete;
  ControllerGame(ControllerGame &&other) = delete;
  ~ControllerGame() = default;

  ControllerGame &operator=(ControllerGame const &other) = delete;
  ControllerGame &operator=(ControllerGame &&other) = delete;

 public:
  //
  static auto getInstance() -> ControllerGame & {
    static ControllerGame controller{};
    return controller;
  }
  //

  auto GenerateField_model(uint8_t level_game) -> void {
    // генерация игрового поля
    int size_filed = GetSizeFiledFromLevelGame(level_game);
    m_model.GenerateField(size_filed);
  }

  auto ClearField_model() -> void {
    // очистка игрового поля
    m_model.ClearField();
  }

  auto GetSizeField_model() const -> int {
    if (m_model.IsGenerateField()) {
      return m_model.GetSizeField();
    }
    return -1;
  }

  auto CalcStepsAroundCell_model(int id_button) -> void {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int size_filed = m_model.GetSizeField();
    int row = id_button / size_filed;
    int column = id_button % size_filed;
    m_model.CalcStepsAroundCell(row, column);
  }

  auto ChangeCurrentCoord_model(int id_button) -> bool {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int size_filed = m_model.GetSizeField();
    int row = id_button / size_filed;
    int column = id_button % size_filed;
    return m_model.ChangeCurrentCoord(row, column);
  }

  auto IsCellPossibleStep_model(int id_button) const -> bool {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int size_filed = m_model.GetSizeField();
    int row = id_button / size_filed;
    int column = id_button % size_filed;
    return m_model.IsCellPossibleStep(row, column);
  }

  auto GetCurrentCoord_model() const -> int {
    if (m_model.IsGenerateField()) {
      // конвертирую кнопку с интерфейса в координаты игрового поля
      std::pair<int, int> coord = m_model.GetCurrentCoord();
      int id_button = coord.first * m_model.GetSizeField() + coord.second;
      return id_button;
    }
    return -1;  // игровое поле не инициализированно
  }

  auto GetColorExpectedRow_model() const -> std::vector<int> {
    std::map<int, int> colors = std::move(m_model.GetColorExpectedRow());
    std::vector<int> result_colors;
    for (auto &i : colors) {
      result_colors.push_back(i.second);
    }
    return result_colors;
  }

  auto StepUp_model() -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.StepUp();
    }
    return false;
  }

  auto StepRight_model() -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.StepRight();
    }
    return false;
  }

  auto StepDown_model() -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.StepDown();
    }
    return false;
  }

  auto StepLeft_model() -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.StepLeft();
    }
    return false;
  }

  auto SwapPlacesWithTopCell_model() -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.SwapPlacesWithTopCell();
    }
    return false;
  }

  auto SwapPlacesWithRightCell_model() -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.SwapPlacesWithRightCell();
    }
    return false;
  }

  auto SwapPlacesWithBottomCell_model() -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.SwapPlacesWithBottomCell();
    }
    return false;
  }

  auto SwapPlacesWithLeftCell_model() -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.SwapPlacesWithLeftCell();
    }
    return false;
  }

  auto SwapChip_model(int id_button) -> bool {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int size_filed = m_model.GetSizeField();
    int row = id_button / size_filed;
    int column = id_button % size_filed;

    if (row >= 0 && row < size_filed && column >= 0 && column < size_filed) {
      m_model.SwapCell(row, column);
      return true;
    }
    return false;
  }

  auto IsSelectedBlockCell_model(int id_button) const -> bool {
    if (m_model.IsGenerateField()) {
      // конвертирую кнопку с интерфейса в координаты игрового поля
      int size_filed = m_model.GetSizeField();
      int row = id_button / size_filed;
      int column = id_button % size_filed;
      return m_model.IsSelectedBlockCell(row, column);
    }
    return false;  // игровое поле не инициализированно
  }

  auto IsSelectedFreeCell_model(int id_button) const -> bool {
    if (m_model.IsGenerateField()) {
      // конвертирую кнопку с интерфейса в координаты игрового поля
      int size_filed = m_model.GetSizeField();
      int row = id_button / size_filed;
      int column = id_button % size_filed;
      return m_model.IsSelectedFreeCell(row, column);
    }
    return false;  // игровое поле не инициализированно
  }

  auto IsSelectedChipCell_model(int id_button) const -> bool {
    if (m_model.IsGenerateField()) {
      // конвертирую кнопку с интерфейса в координаты игрового поля
      int size_filed = m_model.GetSizeField();
      int row = id_button / size_filed;
      int column = id_button % size_filed;
      return m_model.IsSelectedChipCell(row, column);
    }
    return false;  // игровое поле не инициализированно
  }

  auto GetColorCell_model(int id_button) const -> int {
    if (m_model.IsGenerateField()) {
      // конвертирую кнопку с интерфейса в координаты игрового поля
      int size_filed = m_model.GetSizeField();
      int row = id_button / size_filed;
      int column = id_button % size_filed;
      return m_model.GetColorCell(row, column);
      ;
    }
    return -1;  // игровое поле не инициализированно
  }

  auto IsVictoryGame_model() const -> bool {
    if (m_model.IsGenerateField()) {
      return m_model.IsGameVictory();
    }
    return false;
  }

 private:
  GameMechanics m_model;
};

}  // namespace InSearchOfTreasure_2_0

#endif  // CONTROLLER_GAME_HPP
