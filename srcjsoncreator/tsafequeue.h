#pragma once

#include <queue>
#include <memory>
#include <mutex>
#include <utility>


template<typename T>
concept ManagedPointer = requires(T t) {
    { *t } -> std::convertible_to<typename std::pointer_traits<T>::element_type&>;
};

/// <summary>
///    for multithreaded usage
/// </summary>
/// <typeparam name="T"> can be unique_ptr or copyable/movable type</typeparam>
template <typename T>
class TSafeQueue
{
    TSafeQueue(const TSafeQueue&) = delete;
    TSafeQueue& operator=(const TSafeQueue&) = delete;
    TSafeQueue(TSafeQueue&&) = delete;
    TSafeQueue& operator=(TSafeQueue&&) = delete;
public:
    TSafeQueue() = default;

    template<typename U = T>
    void Add(U&& item) {
        std::lock_guard<std::mutex> lock(guard_);
        sequence_.emplace(std::forward<U>(item));
    }

    //template<typename U = T>
    //requires (!ManagedPointer<U>)
    //std::pair<bool, U> Get() {
    //    std::lock_guard<std::mutex> lock(guard_);
    //    if (!sequence_.empty()) {
    //        U item = std::move(sequence_.front());
    //        sequence_.pop();
    //        return {true, std::move(item)};
    //    }
    //    return {false, U()};
    //}

    template<typename U = T>
//    requires ManagedPointer<U>
    U Get() {
        U item;
        {
            std::lock_guard<std::mutex> lock(guard_);
            if (!sequence_.empty()) {
                item = std::move(sequence_.front());
                sequence_.pop();
            }
        }
        return item;
    }

protected:
    std::mutex guard_;
    std::queue<T> sequence_;
};
