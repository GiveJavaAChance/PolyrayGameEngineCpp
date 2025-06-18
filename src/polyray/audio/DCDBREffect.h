#ifndef DCDBREFFECT_H_INCLUDED
#define DCDBREFFECT_H_INCLUDED

#pragma once
#include <cstdint>
#include <cstring>
#include <immintrin.h>

class DCDBREffect {
private:
    float* collectorRight = nullptr;
    float* collectorLeft = nullptr;
    int collectorSize = 0;

    float* volumesR = nullptr;
    float* volumesL = nullptr;
    uint32_t* delaysI = nullptr;
    int sampleCount = 0;

public:
    DCDBREffect(int maxLength) : collectorSize(maxLength) {
        int len = collectorSize * sizeof(float);
        collectorRight = static_cast<float*>(_mm_malloc(len, 32));
        collectorLeft = static_cast<float*>(_mm_malloc(len, 32));
        std::memset(collectorRight, 0, len);
        std::memset(collectorLeft, 0, len);
    }

    ~DCDBREffect() {
        _mm_free(collectorRight);
        _mm_free(collectorLeft);
    }

    void setSamples(float* volumesLeft, float* volumesRight, uint32_t* delays, int numSamples) {
        volumesL = volumesLeft;
        volumesR = volumesRight;
        delaysI = delays;
        sampleCount = numSamples;
    }

    void applyDCDBR(float* buffer, int bufferSize, float* outputRight, float* outputLeft) {
        for (int s = 0; s < sampleCount; s++) {
            int delay = delaysI[s];
            float vR = volumesR[s];
            float vL = volumesL[s];

            float* dstR = collectorRight + delay;
            float* dstL = collectorLeft + delay;

            float* src = buffer;

            int unaligned = (8 - (delay & 7)) & 7;
            int processed = 0;

            __m256 mulR = _mm256_set1_ps(vR);
            __m256 mulL = _mm256_set1_ps(vL);

            for (int i = 0; i < unaligned && i < bufferSize; i++) {
                dstR[i] += src[i] * vR;
                dstL[i] += src[i] * vL;
                processed++;
            }

            dstR += processed;
            dstL += processed;
            src  += processed;

            int simdEnd = (bufferSize - processed) & ~7;

            #pragma GCC unroll 4
            for (int i = 0; i < simdEnd; i += 8) {
                __m256 in = _mm256_loadu_ps(src);
                _mm256_store_ps(dstR, _mm256_fmadd_ps(in, mulR, _mm256_load_ps(dstR)));
                _mm256_store_ps(dstL, _mm256_fmadd_ps(in, mulL, _mm256_load_ps(dstL)));
                dstR += 8;
                dstL += 8;
                src  += 8;
            }

            int tailStart = processed + simdEnd;
            for (int i = tailStart; i < bufferSize; i++) {
                collectorRight[delay + i] += buffer[i] * vR;
                collectorLeft[delay + i] += buffer[i] * vL;
            }
        }

        int len = bufferSize * sizeof(float);
        int len2 = (collectorSize - bufferSize) * sizeof(float);

        std::memcpy(outputRight, collectorRight, len);
        std::memcpy(outputLeft, collectorLeft, len);

        std::memmove(collectorRight, collectorRight + bufferSize, len2);
        std::memmove(collectorLeft, collectorLeft + bufferSize, len2);

        std::memset(collectorRight + (collectorSize - bufferSize), 0, len);
        std::memset(collectorLeft + (collectorSize - bufferSize), 0, len);
    }

};

#endif
