template <typename T>
class borrow_ptr {
public:
    borrow_ptr(T* data) : data{data} {}
    T* operator->() {
        return data;
    }

    T& operator*() {
        return *data;
    }

private:
    T* data;
};