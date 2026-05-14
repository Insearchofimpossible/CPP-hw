#include <vector>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <iostream>

class ISetImpl {
public:
    virtual ~ISetImpl() = default;
    virtual void insert(int val) = 0;
    virtual void erase(int val) = 0;
    virtual bool has(int val) const = 0;
    virtual size_t size() const = 0;
    virtual std::vector<int> get_all() const = 0;
};

class SmallSetImpl : public ISetImpl {
private:
    std::vector<int> data;
public:
    void insert(int val) override {
        if (!has(val)) {
            data.push_back(val);
        }
    }
    void erase(int val) override {
        data.erase(std::remove(data.begin(), data.end(), val), data.end());
    }
    bool has(int val) const override {
        return std::find(data.begin(), data.end(), val) != data.end();
    }
    size_t size() const override {
        return data.size();
    }
    std::vector<int> get_all() const override {
        return data;
    }
};

class LargeSetImpl : public ISetImpl {
private:
    std::unordered_set<int> data;
public:
    void insert(int val) override {
        data.insert(val);
    }
    void erase(int val) override {
        data.erase(val);
    }
    bool has(int val) const override {
        return data.find(val) != data.end();
    }
    size_t size() const override {
        return data.size();
    }
    std::vector<int> get_all() const override {
        return std::vector<int>(data.begin(), data.end());
    }
};

class Set {
private:
    std::unique_ptr<ISetImpl> impl;
    size_t threshold;

    void check_and_switch() {
        size_t current_size = impl->size();
        if (current_size > threshold && dynamic_cast<SmallSetImpl*>(impl.get())) {
            auto items = impl->get_all();
            impl = std::make_unique<LargeSetImpl>();
            for (int item : items) impl->insert(item);
        } else if (current_size <= threshold && dynamic_cast<LargeSetImpl*>(impl.get())) {
            auto items = impl->get_all();
            impl = std::make_unique<SmallSetImpl>();
            for (int item : items) impl->insert(item);
        }
    }

public:
    Set(size_t threshold = 100) : threshold(threshold) {
        impl = std::make_unique<SmallSetImpl>();
    }

    Set(const Set& other) : threshold(other.threshold) {
        if (dynamic_cast<SmallSetImpl*>(other.impl.get())) {
            impl = std::make_unique<SmallSetImpl>();
        } else {
            impl = std::make_unique<LargeSetImpl>();
        }
        for (int item : other.impl->get_all()) {
            impl->insert(item);
        }
    }

    Set& operator=(const Set& other) {
        if (this != &other) {
            Set temp(other);
            std::swap(impl, temp.impl);
            threshold = temp.threshold;
        }
        return *this;
    }

    void add(int val) {
        impl->insert(val);
        check_and_switch();
    }

    void remove(int val) {
        impl->erase(val);
        check_and_switch();
    }

    bool contains(int val) const {
        return impl->has(val);
    }

    Set union_with(const Set& other) const {
        Set result(threshold);
        for (int item : impl->get_all()) result.add(item);
        for (int item : other.impl->get_all()) result.add(item);
        return result;
    }

    Set intersection_with(const Set& other) const {
        Set result(threshold);
        for (int item : impl->get_all()) {
            if (other.contains(item)) {
                result.add(item);
            }
        }
        return result;
    }

    void print() const {
        for (int item : impl->get_all()) {
            std::cout << item << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    Set s1(5);
    for (int i = 0; i < 7; ++i) {
        s1.add(i);
    }

    Set s2(5);
    for (int i = 4; i < 10; ++i) {
        s2.add(i);
    }

    Set u = s1.union_with(s2);
    Set i = s1.intersection_with(s2);

    u.print();
    i.print();

    return 0;
}