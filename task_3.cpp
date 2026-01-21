#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <functional>
#include <chrono>
#include <string>

template <typename T> class ParallelProcessor {
    public:
        ParallelProcessor(size_t numThreads) : numThreads_(numThreads) {
            std::cout << "Processing 1,000,000 elements with 4 threads\n";
        }

        template<typename Callable>
        auto sequential_map(const std::vector<T>& data, Callable lambda_func) {
            using U = std::invoke_result_t<Callable, T>;
            std::vector<U> result;
            auto start_time = std::chrono::high_resolution_clock::now();
            for (const auto& item : data) {
                result.push_back(lambda_func(item));
            }            
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end_time - start_time;
            std::cout << "Sequential time: " << duration.count() * 1000 << " ms.\n";
            seq_time_ = duration.count();
            return result;
        }

        template<typename Callable>
        auto parallel_map(const std::vector<T>& data, Callable lambda_func) {
            using U = std::invoke_result_t<Callable, T>;
            std::vector<U> result(data.size());
            std::vector<std::thread> threads;
            size_t data_size = data.size();
            size_t chunk_size = data_size / this->numThreads_;

            auto start_time = std::chrono::high_resolution_clock::now();
            for (size_t i = 0; i < this->numThreads_; ++i) {
                size_t start_idx = i * chunk_size;
                size_t end_idx = (i == this->numThreads_ - 1) ? data_size : start_idx + chunk_size;
                threads.emplace_back(&ParallelProcessor::worker<U, Callable>, this, std::ref(data), std::ref(result), start_idx, end_idx, lambda_func);
            }

            for (auto& t : threads) {
                t.join();
            }
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration = end_time - start_time;
            std::cout << "Parallel time: " << duration.count() * 1000 << " ms.\n";
            par_time_ = duration.count();
            return result;
        }

        void print_speedup() {
            std::cout << "SpeedUp: " << seq_time_ / par_time_ << "x\n";
            std::cout <<"\n";
        }
    private:
        size_t numThreads_;
        double seq_time_;
        double par_time_;

        template<typename U, typename Callable>
        void worker(const std::vector<T>& data, std::vector<U>& result, size_t start_idx, size_t end_idx, Callable lambda_func) {
            for (size_t i = start_idx; i < end_idx; ++i) {
                result[i] = lambda_func(data[i]);
            }
        }
};

int main() {
    ParallelProcessor<int> processor(4);
    
    std::vector<int> pixelData(1000000);
    std::iota(pixelData.begin(), pixelData.end(), 0);

    processor.sequential_map(pixelData, [](int pixel) {
        return std::min(255, pixel + 50);
    });
    auto brightenedImage = processor.parallel_map(pixelData, [](int pixel) {
        return std::min(255, pixel + 50);
    });
    processor.print_speedup();

    processor.sequential_map(pixelData, [](int pixel) -> std::string {
        return "pixel_" + std::to_string(pixel);
    });
    auto pixelStrings = processor.parallel_map(pixelData, [](int pixel) -> std::string {
        return "pixel_" + std::to_string(pixel);
    });
    processor.print_speedup();

    processor.sequential_map(pixelData, [](int pixel) {
        return pixel * pixel;
    });
    auto squaredPixels = processor.parallel_map(pixelData, [](int pixel) {
        return pixel * pixel;
    });
    processor.print_speedup();

    std::cout << brightenedImage[0] << std::endl;
    std::cout << brightenedImage[1] << std::endl;
    std::cout << brightenedImage[100] << std::endl;
    std::cout << brightenedImage[999999] << std::endl;

    std::cout << pixelStrings[0] << std::endl;
    std::cout << pixelStrings[1] << std::endl;
    std::cout << pixelStrings[100] << std::endl;

    std::cout << squaredPixels[0] << std::endl;
    std::cout << squaredPixels[1] << std::endl;
    std::cout << squaredPixels[10] << std::endl;

    return 0;
}