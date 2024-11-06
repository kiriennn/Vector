#pragma once

#include <algorithm>
#include <compare>
#include <cstddef>
#include <cstdlib>
#include <initializer_list>

template <typename ValueType>
class Vector {
public:
    using SizeType = size_t;
    using DifferenceType = std::ptrdiff_t;

    template <bool IsConst>
    class Iterator {
        using Reference = std::conditional_t<IsConst, const ValueType&, ValueType&>;
        using Pointer = std::conditional_t<IsConst, const ValueType*, ValueType*>;

        Pointer current_;

    public:
        Iterator() : current_(nullptr) {
        }
        explicit Iterator(Pointer ptr) : current_(ptr) {
        }

        Reference operator*() const {
            return *current_;
        }
        Pointer operator->() const {
            return current_;
        }

        Iterator& operator++() {
            ++current_;
            return *this;
        }
        Iterator operator++(int) {
            Iterator copy_iterator(*this);
            ++current_;
            return copy_iterator;
        }
        Iterator& operator--() {
            --current_;
            return *this;
        }
        Iterator operator--(int) {
            Iterator copy_iterator(*this);
            --current_;
            return copy_iterator;
        }
        Iterator& operator+=(DifferenceType shift) {
            current_ += shift;
            return *this;
        }
        Iterator operator+(DifferenceType shift) const {
            Iterator new_iterator(*this);
            new_iterator += shift;
            return new_iterator;
        }
        Iterator& operator-=(DifferenceType shift) {
            current_ -= shift;
            return *this;
        }
        DifferenceType operator-(const Iterator& other) const {
            return current_ - other.current_;
        }
        bool operator==(const Iterator& other) const {
            return current_ == other.current_;
        }
        std::strong_ordering operator<=>(const Iterator& other) const {
            return current_ <=> other.current_;
        }
    };

    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    Vector() : size_(0), capacity_(0), data_(nullptr) {
    }

    explicit Vector(size_t size) : size_(size), capacity_(size), data_(nullptr) {
        try {
            data_ = new ValueType[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = ValueType{};
            }
        } catch (...) {
            delete[] data_;
            throw;
        }
    }
    Vector(std::initializer_list<ValueType> list)
        : size_(list.size()), capacity_(list.size()), data_(nullptr) {
        try {
            data_ = new ValueType[capacity_];
            size_t index = 0;
            for (const auto& element : list) {
                data_[index] = element;
                ++index;
            }
        } catch (...) {
            delete[] data_;
            throw;
        }
    }

    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_), data_(nullptr) {
        try {
            data_ = new ValueType[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        } catch (...) {
            delete[] data_;
            throw;
        }
    }

    Vector(Vector&& other) noexcept
        : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
        other.size_ = 0;
        other.capacity_ = 0;
        other.data_ = nullptr;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            try {
                data_ = new ValueType[capacity_];
                for (size_t i = 0; i < size_; ++i) {
                    data_[i] = other.data_[i];
                }
            } catch (...) {
                delete[] data_;
                throw;
            }
        }
        return *this;
    }
    Vector& operator=(Vector&& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = other.data_;
            other.size_ = 0;
            other.capacity_ = 0;
            other.data_ = nullptr;
        }
        return *this;
    }

    ValueType& operator[](size_t position) noexcept {
        return data_[position];
    }
    const ValueType& operator[](size_t position) const noexcept {
        return data_[position];
    }

    SizeType Size() const noexcept {
        return size_;
    }
    SizeType Capacity() const noexcept {
        return capacity_;
    }
    const ValueType* Data() const noexcept {
        return data_;
    }

    ~Vector() {
        delete[] data_;
    }

    bool operator==(const Vector& other) const {
        if (size_ != other.size_) {
            return false;
        }
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other.data_[i]) {
                return false;
            }
        }
        return true;
    }
    std::strong_ordering operator<=>(const Vector& other) const {
        for (size_t i = 0; i < std::min(size_, other.size_); ++i) {
            if (data_[i] < other.data_[i]) {
                return std::strong_ordering::less;
            }
            if (data_[i] > other.data_[i]) {
                return std::strong_ordering::greater;
            }
        }
        return size_ <=> other.size_;
    }

    void Reserve(SizeType new_capacity) {
        if (capacity_ >= new_capacity) {
            return;
        }
        ValueType* new_data = new ValueType[new_capacity];
        try {
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = std::move(data_[i]);
            }
        } catch (...) {
            delete[] new_data;
            throw;
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void Clear() noexcept {
        size_ = 0;
    }

    void PushBack(const ValueType& new_element) {
        if (size_ == capacity_) {
            int is_zero = static_cast<int>(capacity_ == 0);
            Reserve(2 * capacity_ + is_zero);
        }
        data_[size_] = new_element;
        ++size_;
    }

    void PopBack() noexcept {
        if (size_ > 0) {
            --size_;
        }
    }

    void Swap(Vector& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
        std::swap(data_, other.data_);
    }

    iterator begin() noexcept {
        return iterator(data_);
    }

    const_iterator begin() const noexcept {
        return const_iterator(data_);
    }

    iterator end() noexcept {
        return iterator(data_ + size_);
    }

    const_iterator end() const noexcept {
        return const_iterator(data_ + size_);
    }

private:
    SizeType size_;
    SizeType capacity_;

    ValueType* data_;
};
