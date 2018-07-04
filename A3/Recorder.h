#pragma once

#define EXPORT __declspec (dllexport)

//EXPORT void CALLBACK runDll();

EXPORT void setupBufferIn();
EXPORT BYTE** setupWaveIn();

EXPORT void setupBufferOut(PBYTE* samples);
EXPORT void setupWaveOut(PBYTE* samples);
EXPORT void callbackWaveOut(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR waveHdr, DWORD_PTR dwParam2);

EXPORT void pauseWave();
EXPORT void resumeWave();

EXPORT int getLengthOfSamples();
EXPORT void setLengthOfSamples(DWORD length);

EXPORT BYTE** initializeSamples(int numBytes);


//EXPORT void CALLBACK callbackWaveOut(HWAVEIN hWaveIn, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

EXPORT BOOL* endRecording();
