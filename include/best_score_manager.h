#ifndef BEST_SCORE_MANAGER_H
#define BEST_SCORE_MANAGER_H

#include <cstdint>
#include <filesystem>
#include <fstream>

class BestScoreManager
{
   public:
    using Score = uint32_t;

   private:
    Score origin_, current_;
    const char *filename_target_;

   public:
    BestScoreManager(const char *filename) : origin_(0), current_(0), filename_target_(filename)
    {
        if (!std::filesystem::exists(filename_target_))
        {
            return;
        }
        std::ifstream fin(filename_target_, std::ios::binary);
        if (!fin.is_open())
        {
            return;
        }

        fin.read(reinterpret_cast<char *>(&origin_), sizeof(origin_));
        current_ = origin_;
    }

    Score current() const noexcept { return current_; }

    BestScoreManager &operator=(Score new_score) noexcept;
    void Write();
};

#endif  // !BEST_SCORE_MANAGER_H
