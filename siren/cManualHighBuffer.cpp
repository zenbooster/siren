#include "stdafx.h"
#include "cManualHighBuffer.h"
#include "remSnd.h"
#include "common.h"
#include "cSine.h"
#include "cManualBufferCommon.h"

ALsizei cManualHighBuffer::size;

cManualHighBuffer::cManualHighBuffer()
{
}

cManualHighBuffer::~cManualHighBuffer()
{
}

bool cManualHighBuffer::init()
{
	SndInfo buffer;
	ALenum		format;
	ALuint		freq;
	ALuint BufID = 0;

	//mLooped = true;

	//DefaultSource();

	std::string Filename = "*manual_high";
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
		int i;
		cSine sine;

		//sz_pdata += 2 * ((fmax - fmin + dt_i) / dt_i);
		double eps = sine.get_min_phase(FMAX);
		for (i = 0; i < FREQUENCY * MANUAL_HIGH_SEC || sine.get_phase() > eps; i++)
		{
			double v = sine.next(FMAX);
			size++;
		}

		char *data = new char[size];
		ZeroMemory(data, size);
		char *p = data;
		sine.reset();

		for (i = 0; i < FREQUENCY * MANUAL_HIGH_SEC || sine.get_phase() > eps; i++)
		{
			double v = sine.next(FMAX);
			*p++ = (v > 0.01) ? MAX_SAMPLE_VAL : 0; // meander
																							//*p++ = (1 + v) * 0.5 * MAX_SAMPLE_VAL; // sine
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

ALsizei cManualHighBuffer::get_size() const
{
	return size;
}