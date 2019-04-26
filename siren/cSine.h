#pragma once
#define PI 3.141592654

class cSine
{
	double phase;

public:
	cSine();
	~cSine();

	double next(double freq, double pm = 0.0);
	void reset();
	double get_min_phase(double freq) const;
	double get_phase() const;
};

