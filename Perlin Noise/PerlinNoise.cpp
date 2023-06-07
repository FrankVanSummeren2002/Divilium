#include "src\DiviliumPrecomp.h"
#include "PerlinNoise/PerlinNoise.h"
#include "iostream"
#include "Core/Hash.h"
#include "Core/Math.h"
#include "Random/RandomSeries.h"
// code is based on
//https://adrianb.io/2014/08/09/perlinnoise.html



using namespace divil;

PerlinNoise::PerlinNoise()
{
    mRandomSerie = new RandomSeries();
    for (int i = 0; i < mPsize; i++)
    {
        mP[i] = i % 256;
    }
    Randomize();
}

void divil::PerlinNoise::SetSeed(int aSeed)
{
    mRandomSerie->SetSeed(glm::max(0, aSeed));
}

void PerlinNoise::Randomize()
{
    divil::RandomSeries random;
    int halfSize = mPsize / 2;
    for (int j = 0; j < 2; j++)
    {
        int seed = random.Next();
        for (int i = 0; i < halfSize; i++)
        {
            int index = (int)(divil::Hash::Permute(i, halfSize, seed));
            mP[j * halfSize + i] = index;

            if (index < 0 || index > 255)
            {
                assert(!"the thingy doesnt work");
            }
        }
    }
}

//this is called an ease curve to make it a bit smoother
//the actual function that we are using right now is this
//6t^5 - 15t^4 + 10t^3
double PerlinNoise::Fade(double val)
{
    return val * val * val * (val * (val * 6 - 15) + 10);
}
//linear interpolation

//just add one to the value
//and check if we are looping around
int PerlinNoise::Increment(int val)
{
    val++;
    if (mRepeat > 0) val %= mRepeat;

    return val;
}

double PerlinNoise::Grad(int hash, double x, double y, double z)
{
    //returns the dot product of the two vectors
    //one vector is x y z
    // the other one is a randomly chosen. it has 2 ones (+/-)and 1 zero
    //since it always has 2 ones and 1 zero we can skip a lot of the dot product calculations and do this instead
    switch (hash & 0xF)
    {
    case 0x0: return  x + y;
    case 0x1: return -x + y;
    case 0x2: return  x - y;
    case 0x3: return -x - y;
    case 0x4: return  x + z;
    case 0x5: return -x + z;
    case 0x6: return  x - z;
    case 0x7: return -x - z;
    case 0x8: return  y + z;
    case 0x9: return -y + z;
    case 0xA: return  y - z;
    case 0xB: return -y - z;
    case 0xC: return  y + x;
    case 0xD: return -y + z;
    case 0xE: return  y - x;
    case 0xF: return -y - z;
    default: return 0;
    }
}

double PerlinNoise::aPerlin(double x, double y, double z)
{
    //make the list repeat itself if it goes out of bounds
    if (mRepeat > 0) {
        x = fmod(x, mRepeat);
        y = fmod(y, mRepeat);
        z = fmod(z, mRepeat);
    }
    //the unit cube left bound the right bound is that of the values plus one
    int xBounds = static_cast<int>(x) & 255;
    int yBounds = static_cast<int>(y) & 255;
    int zBounds = static_cast<int>(z) & 255;
    //point inside of the cube which is between 0 and 1
    double xPoint = x - static_cast<int>(x);
    double yPoint = y - static_cast<int>(y);
    double zPoint = z - static_cast<int>(z);

    //We use a fade on these values so that it does not look as linear
    double u = Fade(xPoint);
    double v = Fade(yPoint);
    double w = Fade(zPoint);

    //get the 8 values surrounding the unit cube
    int aaa, aba, aab, abb, baa, bba, bab, bbb;
    aaa = mP[mP[mP[xBounds] + yBounds] + zBounds];
    aba = mP[mP[mP[xBounds] + Increment(yBounds)] + zBounds];
    aab = mP[mP[mP[xBounds] + yBounds] + Increment(zBounds)];
    abb = mP[mP[mP[xBounds] + Increment(yBounds)] + Increment(zBounds)];
    baa = mP[mP[mP[Increment(xBounds)] + yBounds] + zBounds];
    bba = mP[mP[mP[Increment(xBounds)] + Increment(yBounds)] + zBounds];
    bab = mP[mP[mP[Increment(xBounds)] + yBounds] + Increment(zBounds)];
    bbb = mP[mP[mP[Increment(xBounds)] + Increment(yBounds)] + Increment(zBounds)];

    //lerp the dot based on the distance we set earlier (u,v,w)
    double x1, x2, y1, y2;
    x1 = Lerp(Grad(aaa, xPoint, yPoint, zPoint),
        Grad(baa, xPoint - 1, yPoint, zPoint),
        u);
    x2 = Lerp(Grad(aba, xPoint, yPoint - 1, zPoint),
        Grad(bba, xPoint - 1, yPoint - 1, zPoint),
        u);
    y1 = Lerp(x1, x2, v);

    x1 = Lerp(Grad(aab, xPoint, yPoint, zPoint - 1),
        Grad(bab, xPoint - 1, yPoint, zPoint - 1),
        u);
    x2 = Lerp(Grad(abb, xPoint, yPoint - 1, zPoint - 1),
        Grad(bbb, xPoint - 1, yPoint - 1, zPoint - 1),
        u);
    y2 = Lerp(x1, x2, v);

    //make the value between 0 and 1 (instead of -1 and 1)
    return (Lerp(y1, y2, w) + 1) / 2;
}

double PerlinNoise::OctavePerlin(double aX, double aY, double aZ, int aOct, double aPer, double aFreq, double aAmpl)
{
    double total = 0;
    //frequency is the interval between checking
    //is the difference between the values
    //doesnt do a whole lot since it is capped between 0 and 1
    double maxValue = 0;
    for (int i = 0; i < aOct; i++)
    {
        total += aPerlin(aX * aFreq, aY * aFreq, aZ * aFreq) * aAmpl;

        maxValue += aAmpl;

        aAmpl *= aPer;
        aFreq *= 2.0;
    }

    return total / maxValue;
}