#ifndef BLOCK_CELL_HPP
#define BLOCK_CELL_HPP

#include "abstract_cell.hpp"
#include "constans.hpp"

namespace InSearchOfTreasure_2_0 {
/*=== Класс Неактивных Клеток Игрового Поля ===*/
class BlockCell : public AbstractCell {
 public:
  BlockCell() : AbstractCell(TypeCell::kBlock) {}
  auto GetColor() const -> int override { return ColorStone::kNoColor; }
  ~BlockCell() = default;
};

}  // namespace InSearchOfTreasure_2_0

#endif  // BLOCK_CELL_HPP
