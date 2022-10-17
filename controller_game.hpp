#ifndef ControllerGame_GAME_HPP
#define ControllerGame_GAME_HPP

#include "model_game.hpp"

namespace InSearchOfTreasure {
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

  auto GenerateField() -> void {
    // генерация игрового поля
    m_model.GenerateField();
  }

  auto ClearField() -> void {
    // очистка игрового поля
    m_model.ClearField();
  }

  auto IsSelectedBlockCell(int id_button) const -> bool {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int row = id_button / 5;
    int column = id_button % 5;
    try {
      return m_model.IsSelectedBlockCell(row, column);
    } catch (std::exception const &e) {
      return false;  // игровое поле не инициализированно
    }
  }

  auto IsSelectedFreeCell(int id_button) const -> bool {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int row = id_button / 5;
    int column = id_button % 5;
    try {
      return m_model.IsSelectedFreeCell(row, column);
    } catch (std::exception const &e) {
      return false;  // игровое поле не инициализированно
    }
  }

  auto IsSelectedChipCell(int id_button) const -> bool {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int row = id_button / 5;
    int column = id_button % 5;
    try {
      return m_model.IsSelectedChipCell(row, column);
    } catch (std::exception const &e) {
      return false;  // игровое поле не инициализированно
    }
  }

  auto GetColorCell(int id_button) const -> int {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int row = id_button / 5;
    int column = id_button % 5;
    try {
      return m_model.GetColorCell(row, column);
    } catch (std::exception const &e) {
      return -1;  // игровое поле не инициализированно
    }
  }

  auto GetPosRedChipInExpectedRow() const -> int {
    return m_model.GetPosRedChipInExpectedRow();
  }

  auto GetPosGreenChipInExpectedRow() const -> int {
    return m_model.GetPosGreenChipInExpectedRow();
  }

  auto GetPosBlueChipInExpectedRow() const -> int {
    return m_model.GetPosBlueChipInExpectedRow();
  }

  auto ChangePossibleStepsChipInPlayingField_(int id_button) -> void {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int row = id_button / 5;
    int column = id_button % 5;
    m_model.ChangePossibleStepsChipInPlayingField(row, column);
  }

  auto IsCellPossibleStep(int id_button) const -> bool {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int row = id_button / 5;
    int column = id_button % 5;
    return m_model.IsCellPossibleStep(row, column);
  }

  auto GetCurrentCoord() const -> int {
    // конвертирую координаты бэка в id кнопки на интерфейсе
    std::pair<int, int> coord = m_model.GetCurrentCoord();
    if (coord.first == -1 && coord.second == -1) {
      // текущая ячейка не была выбрана
      return -1;
    }
    int id_button = coord.first * 5 + coord.second;
    return id_button;
  }

  auto SwapCells(int id_button) -> void {
    // конвертирую кнопку с интерфейса в координаты игрового поля
    int row = id_button / 5;
    int column = id_button % 5;
    m_model.SwapCells(row, column);
  }

  auto GetVectorPossibleSteps() const -> std::vector<int> {
    std::vector<std::pair<int, int>> coords = m_model.GetVectorPossibleSteps();
    // конвертирую координаты бэка в id кнопки на интерфейсе
    std::vector<int> id_buttons;
    for (auto &coord : coords) {
      id_buttons.push_back(coord.first * 5 + coord.second);
    }
    return id_buttons;
  }

  auto IsVictoryGame() const -> bool { return m_model.IsGameVictory(); }

 private:
  GameMechanics m_model;
};

}  // namespace InSearchOfTreasure
#endif  // ControllerGame_GAME_HPP
