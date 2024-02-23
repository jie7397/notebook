#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cuda_runtime.h>
#include "xpu/runtime.h"

#include <cuda.h>

void malloc_cuda(int id) {
    int dev_id;

    // 拿到当前device id
    auto error1 = cudaGetDevice(&dev_id);

    std::cout << "current device id is: " << dev_id << std::endl;
    // 设置目标id
    auto error1111 = cudaSetDevice(id);
    // auto error3 = xpu_set_device(id);
    // std::cout << "cudaSetDevice error is: " << cudaGetErrorString(error1) << std::endl;
    size_t size = 1024 * 1024 * 200 * sizeof(int);
    void* device_ptr = nullptr;
    auto error2 = cudaMalloc(&device_ptr, size);
    // std::cout << "cudaMalloc error is: " << cudaGetErrorString(error2) << std::endl;

    if (device_ptr != nullptr) {
        std::cout << "CUDA memory allocation successful." << std::endl;
        sleep(5);
    } else {
        std::cerr << "CUDA memory allocation failed." << std::endl;
    }

    // 恢复之前id
    auto error11 = cudaSetDevice(dev_id);
}

int main() {

    malloc_cuda(1);
    malloc_cuda(1);

}