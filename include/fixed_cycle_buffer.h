#ifndef FIXED_CYCLE_BUFFER_H
#define FIXED_CYCLE_BUFFER_H

#include <vector>

template <typename T>
class FixedCycleBuffer
{
   private:
    std::vector<T> buffer_;
    size_t head_, tail_;

   public:
    template <typename Iter>
    explicit FixedCycleBuffer(Iter begin, Iter end) : head_(0)
    {
        tail_ = end - begin - 1;
        buffer_.reserve(tail_ + 1);
        for (; begin != end; ++begin)
        {
            buffer_.emplace_back(std::move(*begin));
        }
    }

    size_t Size() const noexcept { return buffer_.size(); }

    T &operator[](const size_t idx) noexcept { return buffer_[(head_ + idx) % buffer_.size()]; }
    const T &operator[](const size_t idx) const noexcept
    {
        return buffer_[(head_ + idx) & buffer_.size()];
    }

    void MoveFrontToBack()
    {
        size_t new_head = (head_ + 1) % buffer_.size();
        tail_ = head_;
        head_ = new_head;
    }

    const T &Front() const { return buffer_[head_]; }
    T &Front() { return buffer_[head_]; }

    const T &Back() const { return buffer_[tail_]; }
    T &Back() { return buffer_[tail_]; }
};

#endif  // !FIXED_CYCLE_BUFFER_H
