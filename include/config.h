#ifndef CONFIG_H
#define CONFIG_H

#include <array>
#include <string>
#include <utility>

inline constexpr const char *BEST_SCORE_FILENAME = "best_score";

inline constexpr unsigned short FPS = 60;
static_assert(FPS != 0U);

/*
 * RATIO_WIDTH = SCREEN_WIDTH / STD_SCREEN_WIDTH
 */
inline constexpr double RATIO_SCREEN_WIDTH = 0.80;
static_assert(RATIO_SCREEN_WIDTH > 0.0 && RATIO_SCREEN_WIDTH < 1.0);

/*
 * RATIO_HEIGHT = SCREEN_WIDTH / STD_SCREEN_HEIGHT
 */
inline constexpr double RATIO_SCREEN_HEIGHT = 0.74;
static_assert(RATIO_SCREEN_HEIGHT > 0.0 && RATIO_SCREEN_HEIGHT < 1.0);

/*
 * Minimum screen height for launch game
 */
inline constexpr unsigned int SCREEN_MIN_HEIGHT = 3;
static_assert(SCREEN_MIN_HEIGHT >= 3);

/*
 * Screen height for filling all terminal height for game
 */
inline constexpr unsigned int SCREEN_CRITICAL_HEIGHT = 5;
static_assert(SCREEN_CRITICAL_HEIGHT >= SCREEN_MIN_HEIGHT);

/*
 * Visual of bird at screen
 */
inline constexpr std::string BIRD_PICTURE = "<->";
static_assert(BIRD_PICTURE.length() > 0U);

inline constexpr short BIRD_SPEED_0Y = -8;
inline constexpr double g = 12.98;

/*
 * RATIO_BIRD_POSITION_X = BIRD_X / SCREEN_WIDTH
 */
inline constexpr double RATIO_BIRD_POSITION_X = 0.30;
static_assert(RATIO_BIRD_POSITION_X > 0.0 && RATIO_BIRD_POSITION_X < 1.0);

/*
 * RATIO_BIRD_POSITION_Y = BIRD_Y / SCREEN_HEIGHT
 */
inline constexpr double RATIO_BIRD_POSITION_Y = 0.52;
static_assert(RATIO_BIRD_POSITION_Y > 0.0 && RATIO_BIRD_POSITION_Y < 1.0);

/*
 * Pillow with width = 5:
 * [---]
 * [---]
 *
 * [---]
 * ....
 */
inline constexpr std::array<char, 3> PILLOW_PICTURE = {'[', '=', ']'};

inline constexpr double PILLOW_SPEED_X_PER_SECOND = 13;
static_assert(PILLOW_SPEED_X_PER_SECOND > 1);

/* Screen width per pillow */
inline constexpr unsigned int RATIO_WIDTH_PER_PILLOW = 20;
static_assert(RATIO_WIDTH_PER_PILLOW != 0);

/*
 * -------------- <- upper border of screen
 *       |
 *       |  } - it's pillow up empty (current = 3)
 *       |
 *  [----]
 *  [----]
 *  ....
 */
inline constexpr std::pair<unsigned int, unsigned int> PILLOW_UP_EMPTY_RANGE = {0, 0};
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
inline constexpr std::pair<unsigned int, unsigned int> PILLOW_DOWN_EMPTY_RANGE = {0, 0};
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
inline constexpr std::pair<unsigned int, unsigned int> PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE = {2,
                                                                                                 4};
static_assert(PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE.first <=
              PILLOW_NONEMPTY_UP_HEIGHT_RATIOS_RANGE.second);

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_EMPTY_HEIGHT_RATIOS_RANGE = {3, 4};
static_assert(PILLOW_EMPTY_HEIGHT_RATIOS_RANGE.first <= PILLOW_EMPTY_HEIGHT_RATIOS_RANGE.second);

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE = {
    3, 5};
static_assert(PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE.first <=
              PILLOW_NONEMPTY_DOWN_HEIGHT_RATIOS_RANGE.second);

inline constexpr std::pair<unsigned int, unsigned int> PILLOW_WIDTH_RANGE = {6, 6};
static_assert(PILLOW_WIDTH_RANGE.first <= PILLOW_WIDTH_RANGE.second &&
              PILLOW_WIDTH_RANGE.second < RATIO_WIDTH_PER_PILLOW);

#endif  // !CONFIG_H
