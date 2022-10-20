#ifndef ABSTRACT_CELL_HPP
#define ABSTRACT_CELL_HPP

namespace InSearchOfTreasure_2_0 {
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
}  // namespace InSearchOfTreasure_2_0

#endif  // ABSTRACT_CELL_HPP
