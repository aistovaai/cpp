#include <iostream>
#include <vector>
#include <unordered_set>
#include <memory>
#include <algorithm>

using namespace std;

constexpr size_t switch_threshold = 10;

// Абстрактный интерфейс реализации
class set_impl {
public:
    virtual void add(int element) = 0;
    virtual void remove(int element) = 0;
    virtual bool contains(int element) const = 0;
    virtual std::vector<int> elements() const = 0;
    virtual size_t size() const = 0;
    virtual ~set_impl() = default;
};


class small_set_impl : public set_impl {
    std::vector<int> data_;

public:
    void add(int element) override {
        if (!contains(element))
            data_.push_back(element);
    }

    void remove(int element) override {
        data_.erase(std::remove(data_.begin(), data_.end(), element), data_.end());
    }

    bool contains(int element) const override {
        return std::find(data_.begin(), data_.end(), element) != data_.end();
    }

    std::vector<int> elements() const override {
        return data_;
    }

    size_t size() const override {
        return data_.size();
    }
};


class large_set_impl : public set_impl {
    std::unordered_set<int> data_;

public:
    void add(int element) override {
        data_.insert(element);
    }

    void remove(int element) override {
        data_.erase(element);
    }

    bool contains(int element) const override {
        return data_.find(element) != data_.end();
    }

    std::vector<int> elements() const override {
        return {data_.begin(), data_.end()};
    }

    size_t size() const override {
        return data_.size();
    }
};


class set {
    std::unique_ptr<set_impl> impl_;

    void check_switch() {
        size_t sz = impl_->size();

        if (dynamic_cast<small_set_impl*>(impl_.get()) && sz > switch_threshold) {
            auto new_impl = std::make_unique<large_set_impl>();
            for (int e : impl_->elements()) new_impl->add(e);
            impl_ = std::move(new_impl);
        }
        else if (dynamic_cast<large_set_impl*>(impl_.get()) && sz <= switch_threshold) {
            auto new_impl = std::make_unique<small_set_impl>();
            for (int e : impl_->elements()) new_impl->add(e);
            impl_ = std::move(new_impl);
        }
    }

public:
    set() : impl_(std::make_unique<small_set_impl>()) {}

    void add(int element) {
        impl_->add(element);
        check_switch();
    }

    void remove(int element) {
        impl_->remove(element);
        check_switch();
    }

    bool contains(int element) const {
        return impl_->contains(element);
    }

    set union_with(const set& other) const {
        set result;
        for (const auto& e : impl_->elements()) result.add(e);
        for (const auto& e : other.impl_->elements()) result.add(e);
        return result;
    }

    set intersect_with(const set& other) const {
        set result;
        for (const auto& e : impl_->elements())
            if (other.contains(e)) result.add(e);
        return result;
    }

    void print() const {
        std::cout << "{ ";
        for (const auto& e : impl_->elements()) std::cout << e << " ";
        std::cout << "}" << std::endl;
    }
};

int main() {
    set a;
    for (int i = 0; i < 12; ++i)
        a.add(i);

    set b;
    b.add(9140);
    b.add(7);
    b.add(12);

    std::cout << "A = "; a.print();
    std::cout << "B = "; b.print();

    auto c = a.union_with(b);
    std::cout << "A ∪ B = "; c.print();

    auto d = a.intersect_with(b);
    std::cout << "A ∩ B = "; d.print();

    return 0;
}