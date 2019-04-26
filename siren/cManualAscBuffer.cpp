#include "stdafx.h"
#include "cManualAscBuffer.h"
#include "remSnd.h"
#include "cSine.h"
#include "common.h"
#include <string>

ALsizei cManualAscBuffer::size;

cManualAscBuffer::cManualAscBuffer(double dt):
	dt_i(dt)	
{
}

cManualAscBuffer::~cManualAscBuffer()
{
}

bool cManualAscBuffer::init()
{
	SndInfo buffer;
	ALenum		format;
	ALuint		freq;
	ALuint BufID = 0;

	//mLooped = false;

	//DefaultSource();

	std::string Filename = "*manual_asc";
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
		size = 0;
		double fmin = FMIN;
		double fmax = FMAX;
		double i;
		cSine sine;
		double pf = 0.0;

		for (i = fmin; i <= fmax || sine.get_phase() > 0.01; i += dt_i)
		{
			double f = (abs(sin(pf))) * (fmax - fmin) + fmin;
			pf = fmod(0.5*PI * (i - fmin) / (fmax - fmin), 2 * PI);
			double v = sine.next(f);
			size++;
		}

		char *data = new char[size];
		ZeroMemory(data, size);
		char *p = data;
		sine.reset();
		pf = 0.0;

		for (i = fmin; i <= fmax || sine.get_phase() > 0.01; i += dt_i)
		{
			double f = (abs(sin(pf))) * (fmax - fmin) + fmin;
			pf = fmod(0.5*PI * (i - fmin) / (fmax - fmin), 2 * PI);
			double v = sine.next(f);
			*p++ = (v > 0.01) ? MAX_SAMPLE_VAL : 0; // meander
		}

		// Generate buffers
		alGenBuffers(1, &buffer.ID);
		if (!remSnd::CheckALError()) return false;

		format = AL_FORMAT_MONO8;
		buffer.Format = format;
		freq = FREQUENCY;
		buffer.Rate = freq;
		// Fill buffer
		alBufferData(buffer.ID, format, data, size, freq);
		delete[] data;
		if (!remSnd::CheckALError()) return false;

		Buffers[buffer.ID] = buffer;
		BuffersByName[Filename] = buffer.ID;
	}
	else
		buffer = Buffers[BufID];

	//alSourcei(mSourceID, AL_BUFFER, buffer.ID);
	//QueueBuffer(Filename);

	return true;
}

ALsizei cManualAscBuffer::get_size() const
{
	return size;
}