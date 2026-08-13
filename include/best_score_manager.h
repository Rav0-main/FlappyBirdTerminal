#ifndef BEST_SCORE_MANAGER_H
#define BEST_SCORE_MANAGER_H

#include <climits>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include "randomizer.h"

inline constexpr size_t _BEST_SCORE_FILESIZE = 16;
static_assert(_BEST_SCORE_FILESIZE >= 8);

class BestScoreManager
{
   public:
    using Score = uint32_t;

   private:
    Score origin_, current_;
    const char *filename_target_;
    Randomizer<char> bytes_;

    Score EncryptCurrent() const noexcept;
    Score DecryptCurrent() const noexcept;

   public:
    BestScoreManager(const char *filename)
        : origin_(0), current_(0), filename_target_(filename), bytes_(CHAR_MIN, CHAR_MAX)
    {
        if (!std::filesystem::exists(filename_target_))
        {
            return;
        }
        if (std::filesystem::file_size(filename_target_) != _BEST_SCORE_FILESIZE)
        {
            return;
        }

        std::ifstream fin(filename_target_, std::ios::binary);
        if (!fin.is_open())
        {
            return;
        }

        fin.seekg(3, std::ios::beg);
        fin.read(reinterpret_cast<char *>(&origin_), sizeof(origin_));

        current_ = origin_;
        current_ = DecryptCurrent();
        origin_ = current_;
    }

    Score current() const noexcept { return current_; }

    BestScoreManager &operator=(Score new_score) noexcept;
    void Write();

    ~BestScoreManager() { Write(); }
};

#endif  // !BEST_SCORE_MANAGER_H
