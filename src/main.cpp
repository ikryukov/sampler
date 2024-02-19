#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <cxxopts.hpp>

const unsigned int primeNumbers[32] = {
    2,  3,  5,  7,  11, 13, 17, 19, 23, 29,  31,  37,  41,  43,  47,  53,
    59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131,
};

enum struct SampleDimension : uint32_t
{
    ePixelX,
    ePixelY,
    eLightId,
    eLightPointX,
    eLightPointY,
    eBSDF0,
    eBSDF1,
    eBSDF2,
    eBSDF3,
    eRussianRoulette,
    eNUM_DIMENSIONS
};

struct SamplerState
{
    uint32_t seed;
    uint32_t sampleIdx;
};

static float halton(const uint32_t index, const uint32_t base)
{
    const float s = 1.0f / float(base);
    unsigned int i = index;
    float result = 0.0f;
    float f = s;
    while (i)
    {
        const unsigned int digit = i % base;
        result += f * float(digit);
        i = (i - digit) / base;
        f *= s;
    }
    return result;
}

// jenkins hash
static unsigned int hash(unsigned int a)
{
    a = (a + 0x7ED55D16) + (a << 12);
    a = (a ^ 0xC761C23C) ^ (a >> 19);
    a = (a + 0x165667B1) + (a << 5);
    a = (a + 0xD3A2646C) ^ (a << 9);
    a = (a + 0xFD7046C5) + (a << 3);
    a = (a ^ 0xB55A4F09) ^ (a >> 16);
    return a;
}

static SamplerState initSampler(uint32_t linearPixelIndex, uint32_t pixelSampleIndex, uint32_t seed)
{
    SamplerState sampler;
    sampler.seed = hash(linearPixelIndex) ^ hash(seed) + pixelSampleIndex;
    sampler.sampleIdx = 0u;
    return sampler;
}

float random(SamplerState& state, const SampleDimension Dim)
{
    const uint32_t dimension = uint32_t(Dim);
    const uint32_t base = primeNumbers[dimension & 31u];
    return halton(state.seed, base);
}

int main(int argc, const char* argv[])
{
    cxxopts::Options options("sampler <params>", "commands");
    // clang-format off
    options.add_options()
        ("help", "Print usage")
        ("w, width", "screen width", cxxopts::value<uint32_t>()->default_value("800"))
        ("h, height", "screen height", cxxopts::value<uint32_t>()->default_value("600"))
        ("spp", "samples per pixel", cxxopts::value<uint32_t>()->default_value("256"))

        ("pixelX", "pixel horizontal position", cxxopts::value<uint32_t>()->default_value("0"))
        ("pixelY", "pixel vertical position", cxxopts::value<uint32_t>()->default_value("0"))

        ("dimX", "dimension to output as x", cxxopts::value<uint32_t>()->default_value("0"))
        ("dimY", "dimension to output as y", cxxopts::value<uint32_t>()->default_value("1"))
        ;
    // clang-format on

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    const auto screenWidth = result["width"].as<uint32_t>();
    const auto screenHeight = result["height"].as<uint32_t>();
    const auto spp = result["spp"].as<uint32_t>();

    static const auto dimX = static_cast<SampleDimension>(result["dimX"].as<uint32_t>());
    static const auto dimY = static_cast<SampleDimension>(result["dimY"].as<uint32_t>());

    const auto pixelX = result["pixelX"].as<uint32_t>();
    const auto pixelY = result["pixelY"].as<uint32_t>();

    const uint32_t linearPixelIndex = pixelY * screenWidth + pixelX;
    const uint32_t sampleIndex = 0;
    for (int i = 0; i < spp; ++i)
    {
        SamplerState state = initSampler(linearPixelIndex, i, 0);

        float x = random(state, dimX);
        float y = random(state, dimY);

        printf("%f %f\n", x, y);
    }
    return 0;
}
