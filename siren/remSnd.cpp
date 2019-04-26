//===========================================================
//  Filename: remSnd.cpp
//      Sound class.
//      Copyright 2003
//      Created by Roman E. Marchenko, vortex@library.ntu-kpi.kiev.ua
//      $Id: remSnd.cpp,v 1.2 2003/09/26 21:04:57 vortex Exp $
//===========================================================
#include "stdafx.h"
#include "remSnd.h"
#include <windows.h>
#include "AL/alc.h"
//#include "alu.h"
//#include "alut.h"
#include "tstring.h"
#include <sstream>
#include "common.h"
//#include "vorbis/codec.h"

//#pragma comment(lib, "alut.lib")
#pragma comment(lib, "AL/OpenAL32.lib")

using namespace std;

// Defines
#define NUM_OF_DYNBUF	2		// num buffers in queue
#define DYNBUF_SIZE		10024	// Buffer size

// Typedefs
/*typedef struct 
{
	unsigned int	ID;
    string		Filename;
	unsigned int	Rate;
	unsigned int	Format;
} SndInfo;
*/

//typedef map<ALuint, SndInfo> TBuf;

// Vars
TBuf Buffers;
std::map<string, ALuint> BuffersByName;
ALCdevice	*pDevice;
ALCcontext	*pContext;

string ExtractFileExt(const string &Filename)
{
    if (Filename.size() == 0) return "";

    int i = Filename.size();
    string buf;
    while((Filename[i] != '.') && (i > 0))
    {
        buf = Filename[i--] + buf;
    }
    //_strupr((char *)buf.c_str());
		_strupr_s((char *)buf.c_str(), buf.length());
    return buf;
}

//-----------------------------------------------------------
//    Standalone OpenAL function implementation
//-----------------------------------------------------------
ALboolean CheckALCError()
{
	ALenum ErrCode;
	tstring Err = _T("ALC error: ");
	if ((ErrCode = alcGetError(pDevice)) != ALC_NO_ERROR)
	{
		std::string s_err = (char*)alcGetString(pDevice, ErrCode);
		Err += tstring(s_err.begin(), s_err.end());
		ERRMSG(Err.data());
		return AL_FALSE;
	} 
	return AL_TRUE;
}

ALboolean remSnd::CheckALError()
{
	ALenum ErrCode;
	tstring Err = _T("OpenAL error: ");
	if ((ErrCode = alGetError()) != AL_NO_ERROR)
	{
		std::string s_err = (char *)alGetString(ErrCode);
		Err += tstring(s_err.begin(), s_err.end());
		ERRMSG(Err.data());
		return AL_FALSE;
	}
	return AL_TRUE;
}

bool InitializeOpenAL()
{

	// Position of the Listener.
	ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };

	// Velocity of the Listener.
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };

	// Orientation of the Listener. (first 3 elements are "at", second 3 are "up")
	// Also note that these should be units of '1'.
	ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

	// Open default sound device
	pDevice = alcOpenDevice(NULL);
	// Check for errors
	if (!pDevice)
	{
		ERRMSG(_T("Default sound device not present"));
		return false;
	}
	// Creating rendering context
	pContext = alcCreateContext(pDevice, NULL);
	if (!CheckALCError()) return false;

	alcMakeContextCurrent(pContext);

	// Set listener properties
  alListenerfv(AL_POSITION,    ListenerPos);
  alListenerfv(AL_VELOCITY,    ListenerVel);
  alListenerfv(AL_ORIENTATION, ListenerOri);
	return true;
}

void DestroyOpenAL()
{
	// Clear all buffers and sources
	for (TBuf::iterator i = Buffers.begin(); i != Buffers.end(); i++)
		alDeleteBuffers(1, &i->second.ID);

	// Shut down context
	alcMakeContextCurrent(NULL);
	// Destroy context
	alcDestroyContext(pContext);
	// Close sound device
	alcCloseDevice(pDevice);
}

string GetSndInfo()
{
	ostringstream SOut;
	SOut << "ALC info: \n";
	SOut << "DEFAULT_DEVICE_SPECIFIER: " << (char *)alcGetString(pDevice, ALC_DEFAULT_DEVICE_SPECIFIER) << '\n';
	SOut << "DEVICE_SPECIFIER: " << (char *)alcGetString(pDevice, ALC_DEVICE_SPECIFIER) << '\n';
	SOut << "EXTENSIONS: " << (char *)alcGetString(pDevice, ALC_EXTENSIONS) << '\n' << '\n';
	SOut << "AL info: \n";
	SOut << "VERSION: " << (char *)alGetString(AL_VERSION) << '\n';
	SOut << "RENDERER: " << (char *)alGetString(AL_RENDERER) << '\n';
	SOut << "VENDOR: " << (char *)alGetString(AL_VENDOR) << '\n';
	SOut << "EXTENSIONS: " << (char *)alGetString(AL_EXTENSIONS) << '\n' << ends;
	return SOut.str();
}

//-----------------------------------------------------------
//    ov_callback specific functions
//-----------------------------------------------------------

/*size_t ReadOgg(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	istream *File = reinterpret_cast<istream*>(datasource);
	File->read((char *)ptr, size * nmemb);
	return File->gcount();
}

int SeekOgg(void *datasource, ogg_int64_t offset, int whence)
{
	istream *File = reinterpret_cast<istream*>(datasource);
	ios_base::seekdir Dir;
	File->clear();
	switch (whence) 
	{
		case SEEK_SET: Dir = ios::beg;  break;
		case SEEK_CUR: Dir = ios::cur;  break;
		case SEEK_END: Dir = ios::end;  break;
		default: return -1;
	}
	File->seekg((streamoff)offset, Dir);
	return (File->fail() ? -1 : 0);
}

long TellOgg(void *datasource)
{
	istream *File = reinterpret_cast<istream*>(datasource);
	return File->tellg();
}

int CloseOgg(void *datasource)
{
	return 0;
}*/

//-----------------------------------------------------------
//    Sound class
//-----------------------------------------------------------

remSnd::remSnd()
{
	for (int i = 0; i < 3; i++)
	{
		mPos[i] = mVel[i] = 0.0f;
	}
	//mInfo		= NULL; 
	//mVF			= NULL;
	//mComment	= NULL;
	mStreamed = false; // dbg
}

remSnd::~remSnd()
{
}

bool remSnd::Open(const string &Filename, bool Looped, bool Streamed)
{
	// Check file existance
	ifstream a(Filename.c_str());
	if (!a.is_open()) return false;
	a.close();

	mLooped		= Looped;

	// Create source
	alGenSources(1, &mSourceID);
	if (!CheckALError()) return false;

	alSourcef (mSourceID, AL_PITCH,		1.0f);
	alSourcef (mSourceID, AL_GAIN,		1.0f);
	alSourcefv(mSourceID, AL_POSITION,	mPos);
	alSourcefv(mSourceID, AL_VELOCITY,	mVel);
	alSourcei (mSourceID, AL_LOOPING,	mLooped);

	// Extract ext
	string Ext = ExtractFileExt(Filename).data();
	//if (Ext == "WAV") return LoadWavFile(Filename);
	/*if (Ext == "OGG") 
	{
		mStreamed = Streamed;
		return LoadOggFile(Filename, Streamed);
	}*/
	return false;
}

/*bool remSnd::IsStreamed()
{
	return mStreamed;
}*/

bool remSnd::DefaultSource()
{
	// Create source
	alGenSources(1, &mSourceID);
	if (!CheckALError()) return false;

	alSourcef(mSourceID, AL_PITCH, 1.0f);
	alSourcef(mSourceID, AL_GAIN, 1.0f);
	alSourcefv(mSourceID, AL_POSITION, mPos);
	alSourcefv(mSourceID, AL_VELOCITY, mVel);
	alSourcei(mSourceID, AL_LOOPING, mLooped);

	return true;
}

/*bool remSnd::Tone(ALuint f)
{
	SndInfo		buffer;
	ALenum		format;
	ALvoid		*data;
	ALsizei		size;
	ALsizei		freq;
	ALboolean	loop;
	ALuint		BufID = 0;

	mLooped = true;

	// Create source
	alGenSources(1, &mSourceID);
	if (!CheckALError()) return false;

	alSourcef(mSourceID, AL_PITCH, 1.0f);
	alSourcef(mSourceID, AL_GAIN, 1.0f);
	alSourcefv(mSourceID, AL_POSITION, mPos);
	alSourcefv(mSourceID, AL_VELOCITY, mVel);
	alSourcei(mSourceID, AL_LOOPING, mLooped);
	////////

	std::string Filename = "*tone_" + std::to_string(f);
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
		// Generate buffers
		alGenBuffers(1, &buffer.ID);
		if (!CheckALError()) return false;
		// Load wav data
		//alutLoadWAVFile((ALbyte *)Filename.data(), &format, &data, &size, &freq, &loop);
		freq = 22050;
		size = freq / f;
		data = new char[size];
		ALsizei half_size = size >> 1;
		memset(data, 0xff, half_size);
		ZeroMemory((char*)data + half_size, size - half_size);
		if (!CheckALError()) return false;

		format = AL_FORMAT_MONO8;
		buffer.Format = format;
		buffer.Rate = freq;
		// Fill buffer
		alBufferData(buffer.ID, format, data, size, freq);
		if (!CheckALError()) return false;
		// Unload wav
		//alutUnloadWAV(format, data, size, freq);
		delete[] data;
		//if (!CheckALError()) return false;

		Buffers[buffer.ID] = buffer;
	}
	else
		buffer = Buffers[BufID];

	alSourcei(mSourceID, AL_BUFFER, buffer.ID);

	// Place buffer to Buffers map
	return true;
}*/

/*bool remSnd::LoadWavFile(const string &Filename)
{
	SndInfo		buffer;
	ALenum		format;
	ALvoid		*data;
	ALsizei		size;
	ALsizei		freq;
	ALboolean	loop;
	ALuint		BufID = 0;

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
		// Generate buffers
		alGenBuffers(1, &buffer.ID);
		if (!CheckALError()) return false;
		// Load wav data
		alutLoadWAVFile((ALbyte *)Filename.data(), &format, &data, &size, &freq, &loop);
		if (!CheckALError()) return false;
	
		buffer.Format			= format;
		buffer.Rate				= freq;
		// Fill buffer
		alBufferData(buffer.ID, format, data, size, freq);
		if (!CheckALError()) return false;
		// Unload wav
		alutUnloadWAV(format, data, size, freq);
		if (!CheckALError()) return false;

		Buffers[buffer.ID] = buffer;
	}
	else 
		buffer = Buffers[BufID];

	alSourcei (mSourceID, AL_BUFFER,	buffer.ID);

	// Place buffer to Buffers map
	return true;
}*/

/*bool remSnd::LoadOggFile(const string &Filename, bool Streamed)
{
	int				i, DynBuffs = 1, BlockSize;
	// OAL specific
	SndInfo			buffer;
	ALuint			BufID = 0;
	// OggVorbis specific structures
	ov_callbacks	cb;

	// Fill cb struct
	cb.close_func	= CloseOgg;
	cb.read_func	= ReadOgg;
	cb.seek_func	= SeekOgg;
	cb.tell_func	= TellOgg;

	// Create OggVorbis_File struct
	mVF = new OggVorbis_File;

	// Open Ogg file
	OggFile.open(Filename.c_str(), ios_base::in | ios_base::binary);

	// Generate local buffers
	if (ov_open_callbacks(&OggFile, mVF, NULL, -1, cb) < 0)
	{
		// This is not ogg bitstream. Return
		return false;
	}

	// Check for existance of sound
	if (!Streamed)
	{
		for (TBuf::iterator i = Buffers.begin(); i != Buffers.end(); i++)
		{
			if (i->second.Filename == Filename) BufID = i->first;
		}
		BlockSize = ov_pcm_total(mVF, -1) * 4;
	}
	else
	{
		BlockSize	= DYNBUF_SIZE;
		DynBuffs	= NUM_OF_DYNBUF;
		alSourcei(mSourceID, AL_LOOPING, AL_FALSE);
	}
	
	// Return vorbis_comment and vorbis_info structures
	mComment		= ov_comment(mVF, -1);
	mInfo			= ov_info(mVF, -1);

	// Fill buffer infos
	buffer.Rate		= mInfo->rate;
	buffer.Filename	= Filename;
	buffer.Format = (mInfo->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;

	// Fill buffers with data each block by DYNBUF_SIZE bytes
	if (Streamed || !BufID)
	{
		for (i = 0; i < DynBuffs; i++)
		{
			// Create buffers
			alGenBuffers(1, &buffer.ID);
			if (!CheckALError()) return false;
			Buffers[buffer.ID] = buffer;
			// Read amount (DYNBUF_SIZE) data into each buffer
			ReadOggBlock(buffer.ID, BlockSize);
			if (!CheckALError()) return false;

			if (Streamed) // Place buffer to queue
			{
				alSourceQueueBuffers(mSourceID, 1, &buffer.ID);
				if (!CheckALError()) return false;
			}
			else 
				alSourcei(mSourceID, AL_BUFFER, buffer.ID);
		}
	}
	else
	{
		alSourcei(mSourceID, AL_BUFFER, Buffers[BufID].ID);
	}

	return true;
}

bool remSnd::ReadOggBlock(ALuint BufID, size_t Size)
{
	// vars
	char		eof = 0;
	int			current_section;
	long		TotalRet = 0, ret;
	char		*PCM;

	if (Size < 1) return false;
	PCM = new char[Size];

	// Read loop
	while (TotalRet < Size) 
	{
		ret = ov_read(mVF, PCM + TotalRet, Size - TotalRet, 0, 2, 1, &current_section);

		// if end of file or read limit exceeded
		if (ret == 0) break;
		else if (ret < 0) 		// Error in bitstream
		{
			//
		}
		else
		{
			TotalRet += ret;
		}
	}
	if (TotalRet > 0)
	{
		alBufferData(BufID, Buffers[BufID].Format, (void *)PCM, 
					 TotalRet, Buffers[BufID].Rate);
		CheckALError();
	}
	delete [] PCM;
	return (ret > 0);
}*/

void remSnd::SetSampleOffset(ALint ofs)
{
	alSourcei(mSourceID, AL_SAMPLE_OFFSET, ofs);
}

ALint remSnd::GetSampleOffset()
{
	ALint ofs = 0;
	alGetSourcei(mSourceID, AL_SAMPLE_OFFSET, &ofs);
	return ofs;
}

void remSnd::Play()
{
	alSourcePlay(mSourceID);
}

void remSnd::QueueBuffer(string name)
{
	ALuint id = BuffersByName[name];
	alSourceQueueBuffers(mSourceID, 1, &id);
	CheckALError();
}

void remSnd::UnqueueBuffer(ALsizei n)
{
	ALuint *p_id = new ALuint[n];
	alSourceUnqueueBuffers(mSourceID, n, p_id);
	delete[] p_id;
	CheckALError();
}

int remSnd::GetProcessed()
{
	int Processed = 0;
	alGetSourcei(mSourceID, AL_BUFFERS_PROCESSED, &Processed);

	return Processed;
}

bool remSnd::IsPlaying()
{
	ALenum state;
	alGetSourcei(mSourceID, AL_SOURCE_STATE, &state);
	return (state == AL_PLAYING);
}

void remSnd::Pause()
{
	alSourcePause(mSourceID);
}

void remSnd::Close()
{
	if (alIsSource(mSourceID))
	{
		Stop();
		alDeleteSources(1, &mSourceID);
	}
	/*if (!mVF)
	{	
		ov_clear(mVF);
		delete mVF;
	}*/
}

void remSnd::Update()
{
	if (!mStreamed) return;
	
	int				Processed = 0;
	ALuint			BufID;

	alGetSourcei(mSourceID, AL_BUFFERS_PROCESSED, &Processed);

	// We still have processed buffers
	while (Processed--)
	{
		alSourceUnqueueBuffers(mSourceID, 1, &BufID);
		if (!CheckALError()) return;
		/*if (ReadOggBlock(BufID, DYNBUF_SIZE) != 0)
		{
			alSourceQueueBuffers(mSourceID, 1, &BufID);
			if (!CheckALError()) return;
		}
		else
		{
			ov_pcm_seek(mVF, 0);
			alSourceQueueBuffers(mSourceID, 1, &BufID);
			if (!CheckALError()) return;

			if (!mLooped) Stop();
		}*/
	}
}

void remSnd::Stop()
{
	alSourceStop(mSourceID);
}

void remSnd::Move(float X, float Y, float Z)
{
	ALfloat Pos[3] = { X, Y, Z };
	alSourcefv(mSourceID, AL_POSITION, Pos);
}

void remSnd::mute(bool m)
{
	static ALfloat mOldPos[3];

	if (m)
	{
		memcpy(mOldPos, mPos, sizeof(ALfloat[3]));
		Move(mPos[0], mPos[1], AL_MAX_DISTANCE);
	}
	else
	{
		Move(mOldPos[0], mOldPos[1], mOldPos[2]);
	}
}