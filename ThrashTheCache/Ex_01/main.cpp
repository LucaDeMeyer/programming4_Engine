#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    const size_t numElements = 1ULL << 26;
	std::vector<int> buffer(numElements, 1);
	const int numSamples = 12;

    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        std::vector<float> timings;

        for (int n = 0; n < numSamples; ++n)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < numElements; i += stepsize)
            {
                buffer[i] *= 2;
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            timings.push_back(elapsed / 1000.f);
        }

        std::sort(timings.begin(), timings.end());
        float sum = std::accumulate(timings.begin() + 1, timings.end() - 1, 0.0f);
        float average = sum / (timings.size() - 2);

        std::cout << std::left << std::setw(12) << stepsize
            << std::fixed << std::setprecision(3) << average << " ms" << std::endl;
    }


	return 0;
}