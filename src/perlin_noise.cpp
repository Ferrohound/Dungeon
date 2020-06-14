#include "perlin_noise.h"

// Generate a new permutation vector based on the value of seed
PerlinNoise::PerlinNoise(unsigned int seed) 
{
	/*p.resize(256);

	// Fill p with values from 0 to 255
	std::iota(p.begin(), p.end(), 0);

    //perhaps this will be good enough
    //srand(time(NULL));
	srand(seed);

	// Initialize a random engine with seed
	//std::default_random_engine engine(seed);

	// Suffle  using the above random engine
	std::shuffle(p.begin(), p.end(), rand);

	// Duplicate the permutation vector
	p.insert(p.end(), p.begin(), p.end());*/
}

/*
    lacunarity controls the increase in frequency of octaves
    persistance controls decrease in amplitude of octaves

    as the octaves controls the increase in frequency, their impact on the entire
    curve decreases. This is the persistance.
*/
float* PerlinNoise::noise1D(int size, float* seedArray, int nOctaves, int pBias)
{
    float* perlinNoise1D = new float[size];

    //for every point in our curve, we need to calculate its value for 
    //every octave and sum them
    for(int i = 0; i < size; i++)
    {
        float noise = 0.0f;
        float persistance = 1.0f;
        float totalPersistance = 0;

        //for each increasing octave, we use more data points (increase frequency)
        //and weigh it less on the total landscape (decrease persistance)
        for(int o = 0; o < nOctaves; o++)
        {
            //halve the lacunarity (double the frequency)each time, assuming our 
            //noise dimension is a multiple of 2
            int lacunarity = size >> o;

            //these integer divisions tell us which data points in our array
            //are the first and second to look at
            int sample1 = (i/lacunarity) * lacunarity;
            int sample2 = (sample1 + lacunarity) % size;

            //since our values won't be equal to the exact point we're looking at
            //(losing detail), we lerp between them
            float blend = (float)(i - sample1) / (float)lacunarity;
            float sample = lerp(seedArray[sample1], seedArray[sample2], blend);

            //weaken the persistance and add it to our total persistance so we can
            //scale the output value to between 0 and 1 later
            noise += sample * persistance; 
            totalPersistance += persistance;
            persistance/=pBias;
        }

        //scale seed to range [0,1]
        perlinNoise1D[i] = noise/totalPersistance;
    }

    return perlinNoise1D;
}

float* PerlinNoise::noise2D(int width, int height, float* seedArray, 
    int nOctaves, int pBias) 
{
    float* perlinNoise2D = new float[width * height];

    //for every point in our curve, we need to calculate its value for 
    //every octave and sum them
    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            float noise = 0.0f;
            float persistance = 1.0f;
            float totalPersistance = 0;

            //for each increasing octave, we use more data points (increase frequency)
            //and weigh it less on the total landscape (decrease persistance)
            for(int o = 0; o < nOctaves; o++)
            {
                //halve the lacunarity (double the frequency)each time, assuming our 
                //noise dimension is a multiple of 2
                int lacunarity = width >> o;

                //these integer divisions tell us which data points in our array
                //are the first and second to look at
                int sampleX1 = ( x / lacunarity ) * lacunarity;
                int sampleY1 = ( y / lacunarity ) * lacunarity;

                int sampleX2 = (sampleX1 + lacunarity) % width;
                int sampleY2 = (sampleY1 + lacunarity) % width;

                //since our values won't be equal to the exact point we're looking at
                //(losing detail), we lerp between them
                float blendX = (float)(x - sampleX1) / (float)lacunarity;
                float blendY = (float)(y - sampleX1) / (float)lacunarity;

                float sample1 = 
                    lerp(seedArray[sampleY1 * width + sampleX1], 
                    seedArray[sampleY1 * width + sampleX2], blendX);
                
                float sample2 = 
                    lerp(seedArray[sampleY2 * width + sampleX1], 
                    seedArray[sampleY2 * width + sampleX2], blendX);

                float sample = lerp(sample1, sample2, blendY);

                //weaken the persistance and add it to our total persistance so we can
                //scale the output value to between 0 and 1 later
                noise += sample * persistance; 
                totalPersistance += persistance;
                persistance/=pBias;
            }
            //scale seed to range [0,1]
            perlinNoise2D[y * width + x] = noise/totalPersistance;
        }
        
    }

    return perlinNoise2D;
}

double PerlinNoise::noise3D(double x, double y, double z) 
{
	/*
    // Find the unit cube that contains the point
	int X = (int) floor(x) & 255;
	int Y = (int) floor(y) & 255;
	int Z = (int) floor(z) & 255;

	// Find relative x, y,z of point in cube
	x -= floor(x);
	y -= floor(y);
	z -= floor(z);

	// Compute fade curves for each of x, y, z
	double u = fade(x);
	double v = fade(y);
	double w = fade(z);

	// Hash coordinates of the 8 cube corners
	int A = p[X] + Y;
	int AA = p[A] + Z;
	int AB = p[A + 1] + Z;
	int B = p[X + 1] + Y;
	int BA = p[B] + Z;
	int BB = p[B + 1] + Z;

	// Add blended results from 8 corners of cube
	double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z), grad(p[BA], x-1, y, z)), lerp(u, grad(p[AB], x, y-1, z), grad(p[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(p[AA+1], x, y, z-1), grad(p[BA+1], x-1, y, z-1)), lerp(u, grad(p[AB+1], x, y-1, z-1),	grad(p[BB+1], x-1, y-1, z-1))));
	return (res + 1.0)/2.0;*/
    return 0.0;
}

double PerlinNoise::fade(double t) 
{ 
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::grad(int hash, double x, double y, double z) 
{
	int h = hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? x : y,
		   v = h < 4 ? y : h == 12 || h == 14 ? x : z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}