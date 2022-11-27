#include "rng.hpp"
#include <random>

namespace rng {
	float rand01() {
        static std::mt19937 mt; //rng stuff
		static float mx = (float)mt.max();
		return mt() / mx;
	}
	
    float randab(float a, float b) {
        return a + rand01() * (b-a);
    }
}
