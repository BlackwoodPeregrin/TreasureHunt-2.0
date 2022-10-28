#ifndef CHIP_CELL_HPP
#define CHIP_CELL_HPP

#include "abstract_cell.hpp"
#include "constans.hpp"

namespace TreasureHunt_2_0 {

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
}  // namespace TreasureHunt_2_0

#endif  // CHIP_CELL_HPP
