#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cuda_runtime.h>
#include <random>
#include <cuda.h>

#define CUDA_CHECK(call) \
{ \
    cudaError_t error = call; \
    if (error != cudaSuccess) { \
        std::cerr << "CUDA error: " << cudaGetErrorString(error) << " at line " << __LINE__ << std::endl; \
        exit(1); \
    } \
}


void d2h_cuda(long long  size) {
    float *srcHostArray = new float[size];
    float *dstHostArray = new float[size];
    void* deviceArray = nullptr;

    CUDA_CHECK(cudaSetDevice(1));
    CUDA_CHECK(cudaMalloc(&deviceArray, size * sizeof(float)));

    // 生成随机值
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    for (size_t i = 0; i < size; ++i) {
        srcHostArray[i] = dis(gen);
    }

    void* deviceArray2 = nullptr;
    CUDA_CHECK(cudaMalloc(&deviceArray2, 2 * sizeof(float)));
    //memset(srcHostArray, 1.0f, size * sizeof(float));
    //memset(dstHostArray, 0.0f, size * sizeof(float));


    // h2d
    CUDA_CHECK(cudaMemcpy(deviceArray, srcHostArray, (size) * sizeof(float), cudaMemcpyHostToDevice));

    // d2h
    CUDA_CHECK(cudaMemcpy(dstHostArray, deviceArray, (size) * sizeof(float), cudaMemcpyDeviceToHost));

    bool isEqual = true;
    for (long long i = 0; i < size; ++i) {
        if (srcHostArray[i] != dstHostArray[i]) {
            std::cout << "wrong index:" << i << std::endl;
            isEqual = false;
            break;
        }
    }

    if (!isEqual) {
        std::cout << "D2H or H2D fail!!!" << std::endl;
    } else {
        std::cout << "D2H and H2D pass!!!" << std::endl;
    }

    CUDA_CHECK(cudaFree(deviceArray));
    delete[] srcHostArray;
    delete[] dstHostArray;

}

void copy_twice() {
    size_t total_size = 4 * 1024 * 1024 * 1024; // 4GB
    float *host_ptr = new float[total_size];

    // 分成两半
    size_t half_size = total_size / 2;
    float *host_ptr_half1 = host_ptr;
    float *host_ptr_half2 = host_ptr + half_size;

    // 在CUDA设备上分配4GB的内存
    float *device_ptr_half1, *device_ptr_half2;
    CUDA_CHECK(cudaMalloc(&device_ptr_half1, half_size * sizeof(float)));
    CUDA_CHECK(cudaMalloc(&device_ptr_half2, half_size * sizeof(float)));

    // 将主机内存的两部分分别复制到CUDA设备内存
    CUDA_CHECK(cudaMemcpy(device_ptr_half1, host_ptr_half1, half_size * sizeof(float), cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(device_ptr_half2, host_ptr_half2, half_size * sizeof(float), cudaMemcpyHostToDevice));

    // 从CUDA设备内存分两次复制回主机内存
    CUDA_CHECK(cudaMemcpy(host_ptr_half1, device_ptr_half1, half_size * sizeof(float), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaMemcpy(host_ptr_half2, device_ptr_half2, half_size * sizeof(float), cudaMemcpyDeviceToHost));

    // 释放主机内存和CUDA设备内存
    delete[] host_ptr;
    CUDA_CHECK(cudaFree(device_ptr_half1));
    CUDA_CHECK(cudaFree(device_ptr_half2));

}


int main() {
   // std::cout << "start 4M copy"<< std::endl;
   // d2h_cuda(1024LL * 1024);
   // std::cout << "start 4G copy" << std::endl;
   // d2h_cuda(1024LL * 1024 * 1024);
    std::cout << "start 4G + 2 copy" << std::endl;
    d2h_cuda(1024LL * 1024 * 1024 + 2);
    // std::cout << "start 8G copy" << std::endl;
    // d2h_cuda(2LL * 1024 * 1024 * 1024);
}
