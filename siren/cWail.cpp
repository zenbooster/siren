#include "stdafx.h"
#include <string>
#include "cWail.h"
#include "cSine.h"
#include "common.h"

cWail::cWail(double dt, double fmin, double fmax, char* fname)
{
	init(dt, fmin, fmax, fname);
}

cWail::~cWail(void)
{
	Close();
}

bool cWail::init(double dt, double fmin, double fmax, char* fname)
{
	this->fname = fname;
	this->fmin = isnan(fmin) ? (float)(cSettings::cfg.getRoot()["fx"]["wail"]["fmin"]) : fmin;
	this->fmax = isnan(fmax) ? (float)(cSettings::cfg.getRoot()["fx"]["wail"]["fmax"]) : fmax;
	dt_i = isnan(dt) ? SEC2DELTA((float)(cSettings::cfg.getRoot()["fx"]["wail"]["fsec"]), this->fmin, this->fmax) : dt;

	return true;
}

bool cWail::init()
{
	SndInfo buffer;
	ALenum		format;
	ALuint		freq;
	ALsizei size = 0;
	ALuint BufID = 0;

	mLooped = true;

	DefaultSource();

	std::string Filename = "*wail_" + std::to_string(fmin) + "_" + std::to_string(fmax) + "_" + std::to_string(dt_i);
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
		double i;
		cSine sine;
		double pf = 0.0;

		//sz_pdata += 2 * ((fmax - fmin + dt_i) / dt_i);
		for (i = fmin; i <= fmax || sine.get_phase() > 0.1; i += dt_i)
		{
			double f = (1 - abs(sin(pf + PI * 0.5))) * (fmax - fmin) + fmin;
			pf = fmod(PI * (i - fmin) / (fmax - fmin), 2 * PI);
			double v = sine.next(f);
			size++;
		}

		char *data = new char[size];
		ZeroMemory(data, size);
		char *p = data;
		sine.reset();

		pf = 0.0;

		for (i = fmin; i <= fmax || sine.get_phase() > 0.1; i += dt_i)
		{
			double f = (1 - abs(sin(pf + PI * 0.5))) * (fmax - fmin) + fmin;
			pf = fmod(PI * (i - fmin) / (fmax - fmin), 2 * PI);
			double v = sine.next(f);
			*p++ = (v > 0.01) ? MAX_SAMPLE_VAL : 0; // meander
			//*p++ = (1 + v) * 0.5 * MAX_SAMPLE_VAL; // sine
		}

		if (fname)
		{
			FILE *f;
			errno_t e = fopen_s(&f, fname, "wb");
			if (!e)
			{
				fwrite(data, 1, size, f);
				fclose(f);
			}
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