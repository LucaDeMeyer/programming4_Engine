#ifndef THRASH_THE_CACHE_H
#define THRASH_THE_CACHE_H
#include <algorithm>
#include <chrono>
#include <numeric>
#include <vector>

struct TestResult
{
	std::vector<float> stepSizes;
	std::vector<float> results;
};


struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
};

class GameObject3D
{
public:
	Transform transform;
	int ID;
};

class GameObject3DAlt
{
public:
	Transform* transform;
	int ID;
};

    class ThrashTheCache final
    {
    public:
        static TestResult RunExercise1(int numSamples)
        {
            TestResult testResult;
            const size_t numElements = 1ULL << 26;
            std::vector<int> buffer(numElements, 1);

            for (int stepsize = 1; stepsize <= 1024; stepsize *= 2) {
                std::vector<float> timings;
                for (int n = 0; n < numSamples; ++n) {
                    auto start = std::chrono::high_resolution_clock::now();
                    for (size_t i = 0; i < numElements; i += stepsize) {
                        buffer[i] *= 2;
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    timings.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f);
                }
                std::sort(timings.begin(), timings.end());
                testResult.results.push_back(std::accumulate(timings.begin() + 1, timings.end() - 1, 0.0f) / 10.0f);
                testResult.stepSizes.push_back((float)stepsize);
            }
            return testResult;
        };
        static TestResult RunExercise2(int numSamples)
        {
            TestResult testResult;
            const size_t numElements = 1ULL << 26;
            std::vector<GameObject3D> buffer(numElements);

            for (int stepsize = 1; stepsize <= 1024; stepsize *= 2) {
                std::vector<float> timings;
                for (int n = 0; n < numSamples; ++n) {
                    auto start = std::chrono::high_resolution_clock::now();
                    for (size_t i = 0; i < numElements; i += stepsize) {
                        buffer[i].ID *= 2;
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    timings.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f);
                }
                std::sort(timings.begin(), timings.end());
                testResult.results.push_back(std::accumulate(timings.begin() + 1, timings.end() - 1, 0.0f) / 10.0f);
                testResult.stepSizes.push_back((float)stepsize);
            }
            return testResult;
        }
        static TestResult RunExercise2Alt(int numSamples)
        {
            TestResult testResult;
            const size_t numElements = 1ULL << 26;
            std::vector<GameObject3DAlt> buffer(numElements);
            for (size_t i = 0; i < numElements; ++i) {
                buffer[i].transform = new Transform();
                buffer[i].ID = (int)i;
            }

            for (int stepsize = 1; stepsize <= 1024; stepsize *= 2) {
                std::vector<float> timings;
                for (int n = 0; n < numSamples; ++n) {
                    auto start = std::chrono::high_resolution_clock::now();
                    for (size_t i = 0; i < numElements; i += stepsize) {
                        buffer[i].ID *= 2;
                    }
                    auto end = std::chrono::high_resolution_clock::now();
                    timings.push_back(std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000.f);
                }
                std::sort(timings.begin(), timings.end());
                testResult.results.push_back(std::accumulate(timings.begin() + 1, timings.end() - 1, 0.0f) / 10.0f);
                testResult.stepSizes.push_back((float)stepsize);
            }
            for (auto& obj : buffer) delete obj.transform;

            return testResult;
        }
    };

#endif
