#include <iostream>
#include <mutex>

template <typename T>
class AllocatorSingleton {
public:
    static AllocatorSingleton& GetInstance() {
        static AllocatorSingleton instance;
        return instance;
    }

    T* Allocate() {
        return new T();
    }

    void Deallocate(T* ptr) {
        delete ptr;
    }

    AllocatorSingleton(const AllocatorSingleton&) = delete;
    AllocatorSingleton& operator=(const AllocatorSingleton&) = delete;

private:
    AllocatorSingleton() {}
};

template <typename T>
class Singleton {
private:
    static std::mutex mutex_;
    static T* data_;

public:
    static T& Get() {
        if (data_ == nullptr) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (data_ == nullptr) {
                data_ = AllocatorSingleton<T>::GetInstance().Allocate();
            }
        }
        return *data_;
    }

    static void Free() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (data_) {
            AllocatorSingleton<T>::GetInstance().Deallocate(data_);
            data_ = nullptr;
        }
    }
};

template <typename T>
std::mutex Singleton<T>::mutex_;

template <typename T>
T* Singleton<T>::data_ = nullptr;

int main() {
    int& singletonInt = Singleton<int>::Get();
    singletonInt = 42;
    std::cout << "Singleton<int> value: " << Singleton<int>::Get() << std::endl;
    Singleton<int>::Free();
    return 0;
}
