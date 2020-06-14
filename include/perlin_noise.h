/*
    Want perlin noise 1D and 2D, will probably do 3D out of pure masochism
    look at:
    https://en.wikipedia.org/wiki/Perlin_noise
    https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
    the wood texture part of this is interesting
    https://www.youtube.com/watch?v=wbpMiKiSKm8

    Usage

    I want to either generate a 1D or 2D float array based either randomly
    or using a seed provided dimensions to work with
*/
#pragma once

#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <numeric>      /* std::iota   */
#include <algorithm>    /* std::shuffle */
#include <time.h>       /* time */

#include "vec.h"

class PerlinNoise {
	// The permutation vector (not sure what this is)
	std::vector<int> p;
public:
	// Initialize with the reference values for the permutation vector
	PerlinNoise() {};
	// Generate a new permutation vector based on the value of seed
	PerlinNoise(unsigned int seed);
    //as input this should take a seed and array size return an array
    float* noise1D(int size, float* seedArray, int nOctaves, int pBias=2);
    float* noise2D(int width, int height, float* seedArray, int nOctaves, int pBias=2);
	// Get a noise value, for 2D images z can have any value
	double noise3D(double x, double y, double z);

private:
	double fade(double t);
	double grad(int hash, double x, double y, double z);
};

//https://www.geeksforgeeks.org/inline-functions-cpp/
inline float* RandomArray(int size, int seed)
{
    float* seedArray = new float[size];

    if( seed < 0 )
        srand(time(NULL));
    else
        srand(seed);

    for (int i = 0; i < size; i++)
    {
        seedArray[i] = (float) rand() / (float)RAND_MAX;
    }

    return seedArray;
}