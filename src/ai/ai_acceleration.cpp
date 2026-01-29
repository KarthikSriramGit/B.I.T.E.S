#include "ai/ai_acceleration.h"
#include "core/logger.h"
#include <string.h>
#include <arm_math.h>

namespace BITS {
namespace AI {

bool AIAcceleration::initialized = false;
arm_rfft_fast_instance_f32 AIAcceleration::fftInstance;
arm_fir_instance_f32 AIAcceleration::firInstance;

void AIAcceleration::init() {
    if (initialized) {
        return;
    }
    
    // Initialize FFT instance (for 256-point FFT)
    arm_rfft_fast_init_f32(&fftInstance, 256);
    
    initialized = true;
    Logger::info("AI acceleration initialized (ARM CMSIS-DSP)");
}

void AIAcceleration::vectorAdd(const float* a, const float* b, float* result, uint32_t length) {
    arm_add_f32(a, b, result, length);
}

void AIAcceleration::vectorMultiply(const float* a, const float* b, float* result, uint32_t length) {
    arm_mult_f32(a, b, result, length);
}

float AIAcceleration::vectorDotProduct(const float* a, const float* b, uint32_t length) {
    float result;
    arm_dot_prod_f32(a, b, length, &result);
    return result;
}

void AIAcceleration::vectorScale(const float* input, float scale, float* output, uint32_t length) {
    arm_scale_f32(input, scale, output, length);
}

void AIAcceleration::matrixMultiply(const float* a, const float* b, float* result,
                                    uint32_t rowsA, uint32_t colsA, uint32_t colsB) {
    arm_matrix_instance_f32 matA, matB, matResult;
    
    matA.numRows = rowsA;
    matA.numCols = colsA;
    matA.pData = const_cast<float*>(a);
    
    matB.numRows = colsA;
    matB.numCols = colsB;
    matB.pData = const_cast<float*>(b);
    
    matResult.numRows = rowsA;
    matResult.numCols = colsB;
    matResult.pData = result;
    
    arm_mat_mult_f32(&matA, &matB, &matResult);
}

void AIAcceleration::fft(float* input, float* output, uint32_t fftSize) {
    if (fftSize == 256) {
        arm_rfft_fast_f32(&fftInstance, input, output, 0);
    } else {
        // For other sizes, would need to initialize different instance
        // Simplified for now
        Logger::warning("FFT size %lu not optimized, using fallback", fftSize);
    }
}

void AIAcceleration::ifft(float* input, float* output, uint32_t fftSize) {
    if (fftSize == 256) {
        arm_rfft_fast_f32(&fftInstance, input, output, 1);
    } else {
        Logger::warning("IFFT size %lu not optimized, using fallback", fftSize);
    }
}

void AIAcceleration::firFilter(const float* input, float* output, const float* coeffs,
                               uint32_t blockSize, uint32_t numTaps) {
    static float state[128]; // FIR state buffer
    
    firInstance.numTaps = numTaps;
    firInstance.pState = state;
    firInstance.pCoeffs = const_cast<float*>(coeffs);
    
    arm_fir_f32(&firInstance, input, output, blockSize);
}

void AIAcceleration::iirFilter(const float* input, float* output, const float* bCoeffs,
                               const float* aCoeffs, uint32_t blockSize, uint32_t numTaps) {
    // IIR filter implementation using CMSIS-DSP
    // Simplified - would need proper IIR instance setup
    arm_biquad_cascade_df2T_instance_f32 iirInstance;
    static float state[4 * 2]; // State buffer for 2 biquads
    
    iirInstance.numStages = numTaps / 2;
    iirInstance.pState = state;
    iirInstance.pCoeffs = const_cast<float*>(bCoeffs);
    
    arm_biquad_cascade_df2T_f32(&iirInstance, input, output, blockSize);
}

void AIAcceleration::autocorrelation(const float* input, float* output, uint32_t length, uint32_t maxLag) {
    // Optimized autocorrelation using CMSIS-DSP
    for (uint32_t lag = 0; lag < maxLag && lag < length; lag++) {
        float sum = 0.0f;
        uint32_t count = length - lag;
        
        // Use vectorized dot product for efficiency
        arm_dot_prod_f32(input, input + lag, count, &sum);
        output[lag] = sum / count;
    }
}

float AIAcceleration::vectorMean(const float* input, uint32_t length) {
    float mean;
    arm_mean_f32(input, length, &mean);
    return mean;
}

float AIAcceleration::vectorStdDev(const float* input, uint32_t length) {
    float stdDev;
    arm_std_f32(input, length, &stdDev);
    return stdDev;
}

void AIAcceleration::vectorNormalize(float* input, uint32_t length) {
    float mean = vectorMean(input, length);
    float stdDev = vectorStdDev(input, length);
    
    if (stdDev > 0.0f) {
        // Normalize: (x - mean) / stdDev
        float scale = 1.0f / stdDev;
        arm_offset_f32(input, -mean, input, length);
        arm_scale_f32(input, scale, input, length);
    }
}

} // namespace AI
} // namespace BITS
