#include "stdafx.h"
#include "cHiLo.h"
#include "common.h"
#include "cSine.h"

cHiLo::cHiLo(void)
{
}

cHiLo::~cHiLo(void)
{
	Close();
}

bool cHiLo::init()
{
	SndInfo buffer;
	ALenum		format;
	ALuint		freq;
	ALsizei size = 0;
	ALuint BufID = 0;

	mLooped = true;

	DefaultSource();

	std::string Filename = "*hilo";
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
		int i;
		cSine sine;

		float fsec = (float)(cSettings::cfg.getRoot()["fx"]["hilo"]["fsec"]);
		float fmin = (float)(cSettings::cfg.getRoot()["fx"]["hilo"]["fmin"]);
		float fmax = (float)(cSettings::cfg.getRoot()["fx"]["hilo"]["fmax"]);
		for (i = 0; i < FREQUENCY * fsec || sine.get_phase() > 0.1; i++)
		{
			double v = sine.next(fmax);
			size++;
		}
		for (i = 0; i < FREQUENCY * fsec || sine.get_phase() > 0.1; i++)
		{
			double v = sine.next(fmin);
			size++;
		}

		char *data = new char[size];
		ZeroMemory(data, size);
		char *p = data;
		sine.reset();

		for (i = 0; i < FREQUENCY * fsec || sine.get_phase() > 0.1; i++)
		{
			double v = sine.next(fmax);
			*p++ = (v > 0.01) ? MAX_SAMPLE_VAL : 0; // meander
			//*p++ = (1 + v) * 0.5 * MAX_SAMPLE_VAL; // sine
		}
		for (i = 0; i < FREQUENCY * fsec || sine.get_phase() > 0.1; i++)
		{
			double v = sine.next(fmin);
			*p++ = (v > 0.01) ? MAX_SAMPLE_VAL : 0; // meander
			//*p++ = (1 + v) * 0.5 * MAX_SAMPLE_VAL; // sine
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