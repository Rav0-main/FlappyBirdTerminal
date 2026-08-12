#include "best_score_manager.h"

BestScoreManager &BestScoreManager::operator=(Score new_score) noexcept
{
    current_ = new_score;
    return *this;
}

void BestScoreManager::Write()
{
    if (origin_ == current_)
    {
        return;
    }
    std::ofstream fout(filename_target_, std::ios::binary);
    if (fout.is_open())
    {
        fout.write(reinterpret_cast<char *>(&current_), sizeof(current_));
    }
}
