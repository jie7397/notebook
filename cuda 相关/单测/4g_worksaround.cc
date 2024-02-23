#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <cuda_runtime.h>
#include "xpu/runtime.h"
#include <random>
#include <cuda.h>
//#include "xpu/xpukernel.h"
#include "xpu/xdnn.h"

//namespace api = xpytorch::xpu::api;
namespace api = baidu::xpu::api;

#define CUDA_CHECK(call) \
{ \
    cudaError_t error = call; \
    if (error != cudaSuccess) { \
        std::cerr << "CUDA error: " << cudaGetErrorString(error) << " at line " << __LINE__ << std::endl; \
        exit(1); \
    } \
}

#define XPU_CHECK(call) \
{ \
    int error = call; \
    if (error != 0) { \
        std::cerr << "XPU error: " << " at line " << __LINE__ << std::endl; \
        exit(1); \
    } \
}
    
void d2h_cuda(long long  size) {
float *srcHostArray = new float[size];
float *dstHostArray = new float[size];
float* deviceArray = nullptr;

long long half_size = size / 2;

CUDA_CHECK(cudaSetDevice(1));
CUDA_CHECK(cudaMalloc(&deviceArray, size * sizeof(float)));

// 生成随机
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> dis(0.0f, 1.0f);

for (size_t i = 0; i < size; ++i) {
    srcHostArray[i] = dis(gen);
}

// h2d
float* deviceArray3 = nullptr;
float* deviceArray4 = nullptr;

CUDA_CHECK(cudaMalloc(&deviceArray3, half_size * sizeof(float)));
CUDA_CHECK(cudaMalloc(&deviceArray4, half_size * sizeof(float)));
CUDA_CHECK(cudaMemcpy(deviceArray3, srcHostArray, half_size * sizeof(float), cudaMemcpyHostToDevice));
CUDA_CHECK(cudaMemcpy(deviceArray4, srcHostArray + half_size, half_size * sizeof(float), cudaMemcpyHostToDevice));

auto ctx = api::create_context();

XPU_CHECK(api::copy<int8_t>(ctx, reinterpret_cast<int8_t*>(deviceArray3), reinterpret_cast<int8_t*>(deviceArray), half_size * 4));
xpu_wait();
XPU_CHECK(api::copy<int8_t>(ctx, reinterpret_cast<int8_t*>(deviceArray4), reinterpret_cast<int8_t*>(deviceArray + half_size), half_size *  4));
xpu_wait();

loat* deviceArray5 = nullptr;
loat* deviceArray6 = nullptr;
CUDA_CHECK(cudaMalloc(&deviceArray5, half_size * sizeof(float)));
CUDA_CHECK(cudaMalloc(&deviceArray6, half_size * sizeof(float)));

XPU_CHECK(api::copy<int8_t>(ctx, reinterpret_cast<int8_t*>(deviceArray), reinterpret_cast<int8_t*>(deviceArray5), half_size * 4));
xpu_wait();

XPU_CHECK(api::copy<int8_t>(ctx, reinterpret_cast<int8_t*>(deviceArray + half_size), reinterpret_cast<int8_t*>(deviceArray6), half_size *  4));
xpu_wait();

// d2h
CUDA_CHECK(cudaMemcpy(dstHostArray, deviceArray5, half_size * sizeof(float), cudaMemcpyDeviceToHost));
CUDA_CHECK(cudaMemcpy(dstHostArray + half_size, deviceArray6, half_size * sizeof(float), cudaMemcpyDeviceToHost));
std::cout << "copy deviceArray5 deviceArray6 to cpu end" << std::endl;

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


int main() {
    std::cout << "start 4M copy"<< std::endl;
    d2h_cuda(1024LL * 1024);
    //std::cout << "start 4G + 2 copy" << std::endl;
    //d2h_cuda(1024LL * 1024 * 1024 + 2);
    std::cout << "start 8G copy" << std::endl;
    d2h_cuda(2LL * 1024 * 1024 * 1024);
}