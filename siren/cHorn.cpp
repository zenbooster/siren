#include "stdafx.h"
#include "cHorn.h"
#include "cMeander.h"
#include "common.h"
#include <string>

int cHorn::refs = 0;
short *cHorn::p_horn = NULL;
int cHorn::i_horn_len = 0;

cHorn::cHorn(void)
{
	refs++;
	if (!p_horn)
	{
		const Setting &table = cSettings::cfg.getRoot()["fx"]["horn"]["table"];
		i_horn_len = table.getLength();
		p_horn = new short[i_horn_len];
		for (int i = 0; i < i_horn_len; i++)
		{
			p_horn[i] = (int)(table[i]);
		}
	}
}

cHorn::~cHorn(void)
{
	if (!--refs)
	{
		delete[] p_horn;
		p_horn = NULL;
	}
	Close();
}

bool cHorn::init()
{
	SndInfo buffer;
	ALenum		format;
	ALuint		freq;
	ALsizei size = 0;
	ALuint BufID = 0;

	mLooped = true;

	DefaultSource();

	std::string Filename = "*horn";
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
		for (int i = 0; i < i_horn_len; i++)
		{
			size += US2SIZE(p_horn[i]);
		}

		char *data = new char[size];
		cMeander m(data);

		for (int i = 0; i < i_horn_len;)
		{
			m.high();
			m.delay(p_horn[i++]);
			m.low();
			m.delay(p_horn[i++]);
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