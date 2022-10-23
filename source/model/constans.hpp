#ifndef CONSTANS_H
#define CONSTANS_H

#include <stdint.h>
#include <stdexcept>

namespace TreasureHunt_2_0 {

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
    kWhite,
    kNoColor
};  // цвет игровой фишки

// кол-во различных цветов игровых фишек
constexpr u_int8_t kNumbersOfColorsChips = 10;

enum LevelGame {
    kFirst,
    kSecond,
    kThird,
    kFourth,
    kFifth,
    kSixth,
    kSeventh,
    kEighth
}; // уровни игры

// кол-во различных уровней игры
constexpr u_int8_t kLevelsGame = 8;

static int GetSizeFiledFromLevelGame(int level_game) {
    if (level_game > kLevelsGame && level_game < 0) {
        throw std::invalid_argument("Incorrect level");
    }
    int size_field = 2 * (level_game + 1) + 1;
    return size_field;
}

static int GetLevelGameFromSizeField(int size_field) {
    int level_game = (size_field - 1) / 2 - 1;
    if (level_game > kLevelsGame && level_game < 0) {
        throw std::invalid_argument("Size Field");
    }
    return level_game;
}

//  размерность игрового поля с 1 до 9 уровней игры (поле квадратное)
constexpr u_int8_t kFieldSizeInTheLevel[kLevelsGame] {
    5, 7, 9, 11, 13, 15, 17, 19
};

// общее кол-во клеток игрового поля с 1 до 9 уровней игры
constexpr u_int16_t kTotalNumberOfCellsInTheLevel[kLevelsGame] {
    25, 49, 81, 121, 169, 225, 289, 361
};

// кол-во игровых фишек различного цвета с 1 до 9 уровней игры
constexpr u_int8_t kNumberOfDifferentChipsInTheLevel[kLevelsGame] {
    3, 4, 5, 6, 7, 8, 9, 10
};

// суммарное кол-во фишек каждого цвета с 1 до 9 уровней
constexpr u_int8_t kNumberOfTitlesEachColorInTheLevel[kLevelsGame] {
    5, 7, 9, 11, 13, 15, 17, 19
};

// кол-во неактивных клеток с 1 до 9 уровней
constexpr u_int8_t kNumberOfInactiveCellsInTheLevel[kLevelsGame] {
    6, 12, 20, 30, 42, 56, 72, 90
};

// кол-во свободных клеток с 1 до 9 уровней
constexpr u_int8_t kNumberOfFreeCellsInTheLevel[kLevelsGame] {
    4, 9, 16, 25, 36, 49, 64, 81
};

}  // namespace TreasureHunt_2_0

#endif // CONSTANS_H
