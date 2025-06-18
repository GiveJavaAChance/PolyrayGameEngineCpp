/*#include <iostream>
#include <cstdint>
#include <cstring>
#include <immintrin.h>
#include <random>
#include <chrono>
#include "base/FileInputStream.h"
#include "base/AudioInputStream.h"
#include "base/SourceDataLine.h"
#include "polyray/audio/DCDBREffect.h"

long long nanoTime() {
    return std::chrono::high_resolution_clock::now().time_since_epoch().count();
}

float getRandomFloat() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

float* allocateAligned(size_t count) {
    return static_cast<float*>(_aligned_malloc(count * sizeof(float), 32));
}

int main() {
    int maxDelay = 1024 * 10;
    int bufferSize = 1024;
    int collectorSize = maxDelay + bufferSize;

    float* buffer = allocateAligned(bufferSize);

    float* outL = allocateAligned(bufferSize);
    float* outR = allocateAligned(bufferSize);

    int numSamples = 20000;
    float* volumesL = new float[numSamples];
    float* volumesR = new float[numSamples];
    uint32_t* delays = new uint32_t[numSamples];
    float maxDelayF = static_cast<float>(maxDelay);
    for(int i = 0; i < numSamples; i++) {
        float r = getRandomFloat();
        float pan = getRandomFloat();
        volumesL[i] = (1.0f - r) * pan;
        volumesR[i] = (1.0f - r) * (1.0f - pan);
        delays[i] = static_cast<uint32_t>(r * maxDelayF);
    }

    DCDBREffect effect = DCDBREffect(collectorSize);
    effect.setSamples(volumesL, volumesR, delays, numSamples);

    FileInputStream in = FileInputStream("C:/Users/alexl/Documents/Songs wav/JVM output remix.wav");
    AudioInputStream aud = AudioInputStream(in);
    AudioFormat format = aud.format;
    SourceDataLine out = SourceDataLine(format);

    int buffSize = 4096;
    uint8_t* byteBuffer = new uint8_t[buffSize];
    int16_t* samples = new int16_t[buffSize >> 1];
    int64_t bytesRead = 0;
    while((bytesRead = aud.read(byteBuffer, buffSize)) != -1) {
        if(bytesRead != buffSize) {
            std::memset(byteBuffer, 0, buffSize);
        }
        std::memcpy(samples, byteBuffer, bytesRead);
        for(int i = 0; i < 1024; i++) {
            buffer[i] = samples[i << 1] / 32768.0f;
        }
        auto start = std::chrono::steady_clock::now();
        effect.applyDCDBR(buffer, bufferSize, outR, outL);
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "Time: " << duration << " ns\n";
        for(int i = 0; i < 1024; i++) {
            int16_t sr = static_cast<int16_t>(outR[i] * 32767.0f * 0.002f);
            int16_t sl = static_cast<int16_t>(outL[i] * 32767.0f * 0.002f);
            samples[i << 1] = sr;
            samples[(i << 1) + 1] = sl;
        }
        std::memcpy(byteBuffer, samples, buffSize);
        out.write(byteBuffer, buffSize);
    }

    _aligned_free(buffer);
    delete[] volumesL;
    delete[] volumesR;
    delete[] delays;
}*/

#include <iostream>
#include <chrono>

#include "polyray/GLFWindow.h"

#include "base/AudioFormat.h"
#include "base/AudioInputStream.h"
#include "base/BufferedInputStream.h"
#include "base/BufferedReader.h"
#include "base/FileInputStream.h"
#include "base/FloatBuffer.h"
#include "base/InputStream.h"
#include "base/InputStreamReader.h"
#include "base/SourceDataLine.h"

#include "polyray/audio/DCDBREffect.h"

#include "polyray/builtin/RenderObject.h"

#include "polyray/modular/Instance.h"
#include "polyray/modular/Renderable.h"
#include "polyray/modular/RenderObjectBase.h"
#include "polyray/modular/Vertex.h"

#include "polyray/GLFramebuffer.h"
//#include "polyray/GLFWindow.h"
#include "polyray/GLTexture.h"
#include "polyray/GLTextureArray.h"
#include "polyray/Input.h"
#include "polyray/ShaderBuffer.h"
#include "polyray/ShaderProgram.h"
#include "polyray/Texture.h"
#include "polyray/TexturePainter.h"
#include "polyray/Vector2f.h"
#include "polyray/Vector3f.h"
#include "polyray/VertexBufferTemplate.h"

int main() {
    GLFWindow w = GLFWindow("title");
    w.createFrame(500, 500, false, false, false);

    w.hideCursor(true);

    while(w.isWindowOpen()) {
        if(Input::getKey(GLFW_KEY_W)) {
            std::cout << "Forward!" << std::endl;
        }
        if(Input::getKey(GLFW_KEY_A)) {
            std::cout << "Left!" << std::endl;
        }
        if(Input::getKey(GLFW_KEY_S)) {
            std::cout << "Back!" << std::endl;
        }
        if(Input::getKey(GLFW_KEY_D)) {
            std::cout << "Right!" << std::endl;
        }
        w.update();
    }
}

