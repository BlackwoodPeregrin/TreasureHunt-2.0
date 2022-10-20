#ifndef PLAYING_FIELD_HPP
#define PLAYING_FIELD_HPP

#include <stdint.h>

#include <stdexcept>
#include <vector>

#include "abstract_cell.hpp"

namespace InSearchOfTreasure_2_0 {

/*=== Класс Игрового Поля ===*/
class PlayingField {
 public:
  PlayingField(uint8_t size_field) : m_size_field(size_field) {
    InizialiseField_();
  }
  PlayingField(PlayingField const &other) = delete;
  PlayingField(PlayingField &&other) = delete;
  ~PlayingField() { ClearField_(); }

  auto operator=(PlayingField const &other) -> PlayingField & = delete;
  auto operator=(PlayingField &&other) -> PlayingField & = delete;

  auto getSizeField() const -> uint8_t { return m_size_field; }

  auto operator[](int index_row) -> std::vector<AbstractCell *> & {
    if (index_row >= m_size_field && index_row < 0) {
      throw std::out_of_range("operator[]:");
    }
    return m_cell[index_row];
  }

 protected:
  auto InizialiseField_() -> void {
    for (int row = 0; row < m_size_field; ++row) {
      m_cell.push_back(std::vector<AbstractCell *>(m_size_field));
      for (int column = 0; column < m_size_field; ++column) {
        m_cell[row][column] = nullptr;
      }
    }
  }

  auto ClearField_() -> void {
    for (auto &row : m_cell) {
      for (auto &column : row) {
        delete column;
      }
    }
    m_size_field = 0;
  }

 private:
  // клетки на игровом поле
  uint8_t m_size_field;
  std::vector<std::vector<AbstractCell *>> m_cell;
};

}  // namespace InSearchOfTreasure_2_0

#endif  // PLAYING_FIELD_HPP
