#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <utility>

inline constexpr unsigned short FPS = 30U;
static_assert(FPS != 0U);

/*
 * RATIO_WIDTH = SCREEN_WIDTH / STD_SCREEN_WIDTH
 */
inline constexpr double RATIO_WIDTH = 0.82;
static_assert(RATIO_WIDTH > 0.0 && RATIO_WIDTH < 1.0);

/*
 * RATIO_HEIGHT = SCREEN_WIDTH / STD_SCREEN_HEIGHT
 */
inline constexpr double RATIO_HEIGHT = 0.62;
static_assert(RATIO_HEIGHT > 0.0 && RATIO_HEIGHT < 1.0);

/*
 * Minimum screen height for launch game
 */
inline constexpr unsigned int HEIGHT_MIN = 3;
static_assert(HEIGHT_MIN >= 3);

/*
 * Screen height for filling all terminal height for game.
 */
inline constexpr unsigned int HEIGHT_CRITICAL = 6;
static_assert(HEIGHT_CRITICAL >= HEIGHT_MIN);

/*
 * Visual of bird at screen.
 */
inline constexpr std::string BIRD_PICTURE = "<->";
static_assert(BIRD_PICTURE.length() >= 2U);

/*
 * RATIO_BIRD_POSITION_X = BIRD_X / SCREEN_WIDTH
 */
inline constexpr double RATIO_BIRD_POSITION_X = 0.14;
static_assert(RATIO_BIRD_POSITION_X > 0.0 && RATIO_BIRD_POSITION_X < 1.0);

/*
 * RATIO_BIRD_POSITION_Y = BIRD_Y / SCREEN_HEIGHT
 */
inline constexpr double RATIO_BIRD_POSITION_Y = 0.45;
static_assert(RATIO_BIRD_POSITION_Y > 0.0 && RATIO_BIRD_POSITION_Y < 1.0);

/*
 * Pillow with width = 4:
 * [--]
 * [--]
 *
 * [--]
 * ....
 */
inline constexpr char PILLOW_START_PICTURE = '[';
inline constexpr char PILLOW_MIDDLE_PICTURE = '-';
inline constexpr char PILLOW_END_PICTURE = ']';

/* Screen width per pillow */
inline constexpr unsigned int RATIO_WIDTH_PER_PILLOW = 15U;

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_UP_EMPTY_RANGE = {0U, 2U};
static_assert(PILLOW_UP_EMPTY_RANGE.first <= PILLOW_UP_EMPTY_RANGE.second);

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_DOWN_EMPTY_RANGE = {0U, 2U};
static_assert(PILLOW_DOWN_EMPTY_RANGE.first <= PILLOW_DOWN_EMPTY_RANGE.second);

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE = {
    2U, 4U};

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_EMPTY_RATIOS_RANGE = {2U, 4U};

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE = {
    3U, 5U};

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_WIDTH_RANGE = {2U, 8U};

#endif  // !CONFIG_H
