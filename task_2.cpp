#include <iostream>
#include <thread>
#include <deque>
#include <mutex>

template <class T, class Container = std::deque<T>> class CircularBuffer {
    public:
        CircularBuffer(size_t size) : max_size(size) {
            std::cout << "Created Circular Buffer instance.\n";
            buffer.resize(max_size);
            it = buffer.begin();
            head = buffer.begin();
            tail = buffer.begin();
        }

        typename std::deque<T>::iterator begin() {
            return it;
        }

        typename std::deque<T>::iterator end() {
            return it + buffer.size();
        }

        typename std::deque<T>::iterator begin() const {
            return it;
        }

        typename std::deque<T>::iterator end() const {
            return it + buffer.size();
        }

        T* front() {
            return &buffer[0];
        }

        T* back() {
            return &buffer[0] + buffer.size();
        }

        T* front() const {
            return &buffer[0];
        }

        T* back() const {
            return &buffer[0] + buffer.size();
        }

        size_t size() {
            return buffer.size();
        }

        size_t size() const {
            return buffer.size();
        }

        size_t capacity() {
            return max_size;
        }

        size_t capacity() const {
            return max_size;
        }

        bool empty() {
            return buffer.empty();
        }

        bool empty() const{
            return buffer.empty();
        }

        void push_back(const T& item) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (buffer.size() < max_size) {
                buffer.push_back(item);
                it = buffer.begin();
            } else {
                *it = item;
                it++;
                if (it == buffer.end()) {
                    it = buffer.begin();
                }
            }
        }

        void push_back(const T& item) const {
            std::lock_guard<std::mutex> lock(mutex_);
            if (buffer.size() < max_size) {
                buffer.push_back(item);
                it = buffer.begin();
            } else {
                *it = item;
                it++;
                if (it == buffer.end()) {
                    it = buffer.begin();
                }
            }
        }

        void pop_front() {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!buffer.empty()) {
                buffer.pop_front();
                it = buffer.begin();
            } else {
                std::cerr << "Buffer is empty, cannot pop front.\n";
            }
        }

        void pop_front() const {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!buffer.empty()) {
                buffer.pop_front();
                it = buffer.begin();
            } else {
                std::cerr << "Buffer is empty, cannot pop front.\n";
            }
        }

    private:
        mutable std::deque<T> buffer;
        mutable typename std::deque<T>::iterator it, head, tail;
        size_t max_size;
        mutable std::mutex mutex_;
};

int main() {
    const CircularBuffer<double> cb_obj(2);
    cb_obj.push_back(1);
    cb_obj.push_back(2);
    std::cout << cb_obj.capacity() << std::endl;
    std::cout << cb_obj.size() << std::endl;
    cb_obj.push_back(1);
    cb_obj.pop_front();
    std::cout << cb_obj.capacity() << std::endl;
    std::cout << cb_obj.size() << std::endl;
    return 0;
}
