#ifndef BITS_AI_AI_ACCELERATION_H
#define BITS_AI_AI_ACCELERATION_H

/*
 * AI Acceleration Module
 * 
 * Leverages ARM Cortex-M7 hardware acceleration:
 * - SIMD instructions for vector operations
 * - Hardware FPU for fast floating-point math
 * - ARM CMSIS-DSP library for optimized DSP operations
 * - Cache optimization for ML inference
 */

#include <stdint.h>
#include <arm_math.h>

namespace BITS {
namespace AI {

class AIAcceleration {
public:
    static void init();
    
    // Vector operations (SIMD optimized)
    static void vectorAdd(const float* a, const float* b, float* result, uint32_t length);
    static void vectorMultiply(const float* a, const float* b, float* result, uint32_t length);
    static float vectorDotProduct(const float* a, const float* b, uint32_t length);
    static void vectorScale(const float* input, float scale, float* output, uint32_t length);
    
    // Matrix operations (optimized)
    static void matrixMultiply(const float* a, const float* b, float* result, 
                              uint32_t rowsA, uint32_t colsA, uint32_t colsB);
    
    // FFT operations (hardware accelerated)
    static void fft(float* input, float* output, uint32_t fftSize);
    static void ifft(float* input, float* output, uint32_t fftSize);
    
    // Filtering operations
    static void firFilter(const float* input, float* output, const float* coeffs, 
                         uint32_t blockSize, uint32_t numTaps);
    static void iirFilter(const float* input, float* output, const float* bCoeffs,
                         const float* aCoeffs, uint32_t blockSize, uint32_t numTaps);
    
    // Autocorrelation (optimized)
    static void autocorrelation(const float* input, float* output, uint32_t length, uint32_t maxLag);
    
    // Statistics
    static float vectorMean(const float* input, uint32_t length);
    static float vectorStdDev(const float* input, uint32_t length);
    static void vectorNormalize(float* input, uint32_t length);

private:
    static bool initialized;
    static arm_rfft_fast_instance_f32 fftInstance;
    static arm_fir_instance_f32 firInstance;
};

} // namespace AI
} // namespace BITS

#endif // BITS_AI_AI_ACCELERATION_H
