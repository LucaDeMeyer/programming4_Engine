#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>


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

int main()
{

	const size_t numElements = 1ULL << 26;
	const int numSamples = 12;
	std::vector<GameObject3DAlt> buffer(numElements);

	for (size_t i = 0; i < numElements; ++i) {
		buffer[i].transform = new Transform();
		buffer[i].ID = static_cast<int>(i);
	}
    for (int stepsize = 1; stepsize <= 1024; stepsize *= 2)
    {
        std::vector<float> samples;

        for (int run = 0; run < numSamples; ++run)
        {
            auto start = std::chrono::high_resolution_clock::now();

            for (size_t i = 0; i < buffer.size(); i += stepsize)
            {
                buffer[i].ID *= 2;
                for (int m = 0; m < 16; ++m) {
                    buffer[i].transform->matrix[m] *= 2.0f;
                }
            }

            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            samples.push_back(elapsed / 1000.f);
        }

        std::sort(samples.begin(), samples.end());
        float sum = std::accumulate(samples.begin() + 1, samples.end() - 1, 0.0f);
        float average = sum / (samples.size() - 2);

        std::cout << std::left << std::setw(12) << stepsize
            << std::fixed << std::setprecision(3) << average << " ms" << std::endl;
    }

    for (size_t i = 0; i < numElements; ++i) {
        delete buffer[i].transform;
    }

	return 0;
}