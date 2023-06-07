#pragma once
// code is based on
//https://adrianb.io/2014/08/09/perlinnoise.html
namespace divil
{
	class RandomSeries;
	class PerlinNoise
	{
	public:
		PerlinNoise();
	private:
		RandomSeries* mRandomSerie;
	
		int mRepeat = -1;
		static const int mPsize = 512;
		int mP[mPsize];

		double Fade(double aVal);
		int Increment(int aVal);
		double Grad(int aHash, double aX, double aY, double aZ);
	public:
		void SetSeed(int aSeed);
		void Randomize();
		double aPerlin(double aX, double aY, double aZ);
		double OctavePerlin(double aX, double aY, double aZ, int aOct, double aPer, double aFreq, double aAmpl);
	};
}