#ifndef CONFIG_H
#define CONFIG_H

#include <string>

inline constexpr unsigned short FPS = 60U;
static_assert(FPS != 0U, "FPS must be != 0.");

/*
 * RATIO_WIDTH = SCREEN_WIDTH / STD_SCREEN_WIDTH
 */
inline constexpr double RATIO_WIDTH = 0.82;
static_assert(RATIO_WIDTH > 0.0 && RATIO_WIDTH < 1.0, "RATIO_WIDTH must be > 0.0 && < 1.0.");

/*
 * RATIO_HEIGHT = SCREEN_WIDTH / STD_SCREEN_HEIGHT
 */
inline constexpr double RATIO_HEIGHT = 0.62;
static_assert(RATIO_HEIGHT > 0.0 && RATIO_HEIGHT < 1.0, "RATIO_HEIGHT must be > 0.0 && < 1.0.");

/*
 * Visual of bird at screen.
 */
inline constexpr std::string BIRD_PICTURE = "<>";
static_assert(BIRD_PICTURE.length() == 2U, "BIRD_PICTURE length must be == 2.");

/*
 * RATIO_BIRD_POSITION_X = BIRD_X / SCREEN_WIDTH
 */
inline constexpr double RATIO_BIRD_POSITION_X = 0.14;
static_assert(RATIO_BIRD_POSITION_X > 0.0 && RATIO_BIRD_POSITION_X < 1.0,
              "RATIO_BIRD_POSITION_X must be > 0.0 && < 1.0.");

/*
 * RATIO_BIRD_POSITION_Y = BIRD_Y / SCREEN_HEIGHT
 */
inline constexpr double RATIO_BIRD_POSITION_Y = 0.45;
static_assert(RATIO_BIRD_POSITION_Y > 0.0 && RATIO_BIRD_POSITION_Y < 1.0,
              "RATIO_BIRD_POSITION_Y must be > 0.0 && < 1.0.");

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
inline constexpr unsigned int RATIO_WIDTH_PER_PILLOW = 30U;

#endif  // !CONFIG_H
