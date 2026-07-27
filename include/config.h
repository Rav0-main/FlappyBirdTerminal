#ifndef CONFIG_H
#define CONFIG_H

#include <string>

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

#endif  // !CONFIG_H
