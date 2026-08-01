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
 * Screen height for filling all terminal height for game
 */
inline constexpr unsigned int HEIGHT_CRITICAL = 6;
static_assert(HEIGHT_CRITICAL >= HEIGHT_MIN);

/*
 * Visual of bird at screen
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
 * Pillow with width = 5:
 * [---]
 * [---]
 *
 * [---]
 * ....
 */
inline constexpr char PILLOW_START_PICTURE = '[';
inline constexpr char PILLOW_MIDDLE_PICTURE = '-';
inline constexpr char PILLOW_END_PICTURE = ']';

/* Screen width per pillow */
inline constexpr unsigned int RATIO_WIDTH_PER_PILLOW = 25U;

/*
 * -------------- <- upper border of screen
 *       |
 *       |  } - it's pillow up empty (current = 3)
 *       |
 *  [----]
 *  [----]
 *  ....
 */
inline constexpr std::pair<unsigned int, unsigned int> PILLOW_UP_EMPTY_RANGE = {0U, 0U};
static_assert(PILLOW_UP_EMPTY_RANGE.first <= PILLOW_UP_EMPTY_RANGE.second);

/*
 * ---------------- <- upper border of screen
 * ...
 *
 * [----]
 * [----]
 *      |
 *      |   } - it's pillow down empty (current = 2)
 * ----------------
 */
inline constexpr std::pair<unsigned int, unsigned int> PILLOW_DOWN_EMPTY_RANGE = {0U, 0U};
static_assert(PILLOW_DOWN_EMPTY_RANGE.first <= PILLOW_DOWN_EMPTY_RANGE.second);

/*
 * pilloq heights:
 * [--]  |
 * [--]  |  } - it's nonempty up height
 * [--]  |
 *       \ } - it's empty height
 * [--]  /
 * [--]  / } - it's nonempty down height
 */
inline constexpr std::pair<unsigned int, unsigned int> PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE = {
    2U, 4U};
static_assert(PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE.first <=
              PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE.second);

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_EMPTY_HEIGHT_RATIOS_RANGE = {2U, 4U};
static_assert(PILLOW_EMPTY_HEIGHT_RATIOS_RANGE.first <= PILLOW_EMPTY_HEIGHT_RATIOS_RANGE.second);

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE = {
    3U, 5U};
static_assert(PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE.first <=
              PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE.second);

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_WIDTH_RANGE = {4U, 10U};
static_assert(PILLOW_WIDTH_RANGE.first <= PILLOW_WIDTH_RANGE.second &&
              PILLOW_WIDTH_RANGE.second < RATIO_WIDTH_PER_PILLOW);

#endif  // !CONFIG_H
