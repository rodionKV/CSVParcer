
#ifndef CSVPARCER_CSVITERATOR_H
#define CSVPARCER_CSVITERATOR_H

#include <iterator>
#include <utility>
#include "CSVParser.h"
#include "TupleS.h"

template<class ...Args>
struct CSVIterator {
    typedef std::tuple<Args...> T;
    using value_type = T;
    using pointer = value_type *;
    using reference = value_type &;

    using iterator = CSVIterator<Args...>;
    CSVParser<Args...> *g_;

    CSVIterator(CSVParser<Args...> *g) : g_(g) {}

    // Требования Iterator:

    CSVIterator(const iterator &) = default;

    iterator &operator=(const iterator &) = default;

    ~CSVIterator() = default;

    friend void swap(iterator &a, iterator &b) { std::swap(a.g_, b.g_); }

    friend bool operator==(const iterator &lhs, const iterator &rhs) { return lhs.g_ == rhs.g_; }

    friend bool operator!=(const iterator &lhs, const iterator &rhs) { return !(lhs == rhs); }

    reference operator*() const {
        return g_->get_current_line_tuple();
    }

    pointer operator->() const { return std::addressof(**this); }

    iterator &operator++() {
        try {
            g_->next();
        } catch (std::invalid_argument &invalid_argument) {
            std::cout << invalid_argument.what() << std::endl;
            g_ = nullptr;
            return *this;
        }
        if (g_->is_finish()) g_ = nullptr;
        return *this;
    }

    struct proxy {
        T value_;

        proxy(T &value) : value_(std::move(value)) {}

        reference operator*() { return value_; }
    };

    proxy operator++(int) {
        proxy old{**this};
        ++*this;
        return old;
    }
};

template<class ...Args>
CSVIterator<Args...> begin(CSVParser<Args...> &gen) {
    CSVIterator<Args...> it{&gen};
    return ++it;
}

template<class ...Args>
CSVIterator<Args...> end(CSVParser<Args...> &) {
    return {nullptr};
}


#endif //CSVPARCER_CSVITERATOR_H
