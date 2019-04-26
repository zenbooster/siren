#include "stdafx.h"
#include "cShortEnd.h"
#include "cSine.h"
#include "common.h"
#include <string>

#define FMIN (float)(cSettings::cfg.getRoot()["fx"]["short_end"]["fmin"])
#define FMAX (float)(cSettings::cfg.getRoot()["fx"]["short_end"]["fmax"])

cShortEnd::cShortEnd(double dt)
{
	fmin = FMIN;
	fmax = FMAX;
	dt_i = isnan(dt) ? SEC2DELTA((float)(cSettings::cfg.getRoot()["fx"]["short_end"]["fsec"]), fmin, fmax) : dt;
}

cShortEnd::~cShortEnd()
{
	Close();
}

bool cShortEnd::init()
{
	SndInfo buffer;
	ALenum		format;
	ALuint		freq;
	ALsizei size = 0;
	ALuint BufID = 0;

	mLooped = false;

	DefaultSource();

	std::string Filename = "*short_end";
	// Fill buffer struct
	buffer.Filename = Filename;
	// Check for existance of sound
	for (TBuf::iterator i = Buffers.begin(); i != Buffers.end(); i++)
	{
		if (i->second.Filename == Filename) BufID = i->first;
	}

	// If Such buffer not exist yet
	if (!BufID)
	{
		//double fmin = FMIN;
		//double fmax = FMAX;
		double i;
		cSine sine;
		double pf = 0.0;
		//sz_pdata += 2 * ((FMAX - FMIN + dt_i) / dt_i);
		//for (i = FMAX/2; i >= FMIN || sine.get_phase() > 0.01; i -= dt_i)
		for (i = fmin; i <= fmax || sine.get_phase() > 0.01; i += dt_i)
		{
			//double v = sine.next(i);
			//sz_pdata++;
			double f = (1 - abs(sin(pf))) * (fmax - fmin) + fmin;
			pf = fmod(0.5*PI * (i - fmin) / (fmax - fmin), 2 * PI);
			double v = sine.next(f);
			size++;
		}

		char *data = new char[size];
		ZeroMemory(data, size);
		char *p = data;
		sine.reset();
		pf = 0.0;

		//for (i = FMAX/2; i >= FMIN || sine.get_phase() > 0.01; i -= dt_i)
		for (i = fmin; i <= fmax || sine.get_phase() > 0.01; i += dt_i)
		{
			//double v = sine.next(i);
			//*p++ = (v > 0) ? MAX_SAMPLE_VAL : 0; // meander
			//*p++ = (1 + v) * 0.5 * MAX_SAMPLE_VAL; // sine
			double f = (1 - abs(sin(pf))) * (fmax - fmin) + fmin;
			pf = fmod(0.5*PI * (i - fmin) / (fmax - fmin), 2 * PI);
			double v = sine.next(f);
			*p++ = (v > 0.01) ? MAX_SAMPLE_VAL : 0; // meander
																							//*p++ = (1 + v) * 0.5 * MAX_SAMPLE_VAL; // sin
		}

		// Generate buffers
		alGenBuffers(1, &buffer.ID);
		if (!CheckALError()) return false;

		format = AL_FORMAT_MONO8;
		buffer.Format = format;
		freq = FREQUENCY;
		buffer.Rate = freq;
		// Fill buffer
		alBufferData(buffer.ID, format, data, size, freq);
		delete[] data;
		if (!CheckALError()) return false;

		Buffers[buffer.ID] = buffer;
	}
	else
		buffer = Buffers[BufID];

	alSourcei(mSourceID, AL_BUFFER, buffer.ID);

	return true;
}