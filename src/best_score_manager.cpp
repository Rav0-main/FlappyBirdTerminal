#include "best_score_manager.h"

BestScoreManager::Score BestScoreManager::EncryptCurrent() const noexcept
{
    return current_ ^ (current_ >> 1);
}

BestScoreManager::Score BestScoreManager::DecryptCurrent() const noexcept
{
    Score result = current_;
    Score mask = current_ >> 1;
    while (mask != 0)
    {
        result ^= mask;
        mask >>= 1;
    }
    return result;
}

BestScoreManager &BestScoreManager::operator=(Score new_score) noexcept
{
    current_ = new_score;
    return *this;
}

void BestScoreManager::Write()
{
    /*
     * File with best score it's 16 bytes binary file.
     * File contains:
     * ...[],,,
     * ... - 3 random bytes; [] - best score, ,,, - remaining bytes also random.
     */
    if (origin_ == current_)
    {
        return;
    }
    std::ofstream fout(filename_target_, std::ios::binary);
    if (fout.is_open())
    {
        char byte;
        for (size_t i = 0; i < 3; ++i)
        {
            byte = bytes_();
            fout.write(reinterpret_cast<char *>(&byte), sizeof(byte));
        }

        Score encrypted_current = EncryptCurrent();
        fout.write(reinterpret_cast<char *>(&encrypted_current), sizeof(encrypted_current));

        for (size_t i = 3 * sizeof(char) + sizeof(Score); i < _BEST_SCORE_FILESIZE; ++i)
        {
            byte = bytes_();
            fout.write(reinterpret_cast<char *>(&byte), sizeof(byte));
        }
    }
}
