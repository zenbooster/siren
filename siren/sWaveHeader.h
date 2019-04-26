#pragma once

#pragma pack(1) 
typedef struct sWaveHeader
{
  char  RiffSig[4];
  long  WaveformChunkSize;
  char  WaveSig[4];
  char  FormatSig[4];
  long  FormatChunkSize;
  short FormatTag;
  short Channels;
  long  SampleRate;
  long  BytesPerSec;
  short BlockAlign;
  short BitsPerSample;
  char  DataSig[4];
  long  DataSize;
} sWaveHeader;
#pragma pack() 