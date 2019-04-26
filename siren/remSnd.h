//===========================================================
//  Filename: remSnd.h
//      Sound class.
//      Copyright 2003
//      Created by Roman E. Marchenko, vortex@library.ntu-kpi.kiev.ua
//      $Id: remSnd.h,v 1.2 2003/09/26 21:04:57 vortex Exp $
//===========================================================

#ifndef _REM_SND_H_
#define _REM_SND_H_

#include "AL/al.h"
#include <fstream>
#include <map>
//#include "vorbis/vorbisfile.h"

#define NUM_OF_DYNBUF	2		// num buffers in queue

// Typedefs
typedef struct
{
	unsigned int	ID;
	std::string		Filename;
	unsigned int	Rate;
	unsigned int	Format;
} SndInfo;

typedef std::map<ALuint, SndInfo> TBuf;

class remSnd  
{
public:
	ALfloat mVel[3];
	ALfloat mPos[3];
	bool	mLooped;
	
	// Functions
	//bool Tone(ALuint f);
	bool Open(const std::string &Filename, bool Looped, bool Streamed);
	//bool IsStreamed();
	void SetSampleOffset(ALint ofs);
	ALint GetSampleOffset();
	void Play();
	void QueueBuffer(std::string name);
	void UnqueueBuffer(ALsizei n = 1);
	int GetProcessed();
	bool IsPlaying();
	void Pause();
	void Close();
	void Update();
	void Move(float X, float Y, float Z);
	void Stop();

	// Construction/destruction
	remSnd();
	virtual ~remSnd();

	virtual bool init()
	{
		return true;
	}

	void mute(bool m);

	static ALboolean CheckALError();

protected:
	// OAL specific vars
	ALuint			mSourceID;

	//typedef std::map<ALuint, SndInfo> TBuf;
	//static TBuf Buffers;
	bool DefaultSource();

private:
	// vorbisfile specific vars
	//OggVorbis_File	*mVF;
	//vorbis_comment	*mComment;
	//vorbis_info		*mInfo;
	// Common vars
	//std::ifstream	OggFile;
	bool			mStreamed;

	// Functions
	//bool ReadOggBlock(ALuint BufID, size_t Size);
	//bool LoadOggFile (const std::string &Filename, bool Streamed);
	//bool LoadWavFile (const std::string &Filename);
};

// Public standalone functions
extern std::string	GetSndInfo();
extern bool			InitializeOpenAL();
extern void			DestroyOpenAL();
extern TBuf Buffers;
extern std::map<std::string, ALuint> BuffersByName;

#endif 
