#ifndef FREE_CELL_HPP
#define FREE_CELL_HPP

#include "abstract_cell.hpp"
#include "constans.hpp"

namespace TreasureHunt_2_0 {
/*=== Класс Свободных Клеток Игрового Поля ===*/
class FreeCell : public AbstractCell {
 public:
  FreeCell() : AbstractCell(TypeCell::kFree) {}
  auto GetColor() const -> int override { return ColorStone::kNoColor; }
  ~FreeCell() = default;
};
}  // namespace TreasureHunt_2_0

#endif  // FREE_CELL_HPP
