/*Sam Collins, A00987689
Comp 3770 A3*/

#include <windows.h>
#include "Recorder.h"

#define INP_BUFFER_SIZE 16384

static PWAVEHDR pWaveHdr1, pWaveHdr2;
static PBYTE pBuffer1, pBuffer2, pSaveBuffer, pNewBuffer;
static DWORD dwDataLength, dwRepetitions = 1;

static HWAVEIN hWaveIn;
static HWAVEOUT hWaveOut;
static WAVEFORMATEX waveform;

static BOOL bDone = FALSE;
static BOOL bRecording = FALSE;

//static BYTE buffer;
static int bufferlength = INP_BUFFER_SIZE;

EXPORT void CALLBACK callbackWaveIn(HWAVEIN hWaveIn, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);

//makes dll
int WINAPI DllMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	return TRUE;
}

EXPORT void setupBufferIn()
{
	if (pWaveHdr1)
		waveInUnprepareHeader(hWaveIn, (pWaveHdr1), sizeof(WAVEHDR));

	pBuffer1 = malloc(INP_BUFFER_SIZE);

	pWaveHdr1 = malloc(sizeof(WAVEHDR));
	pWaveHdr1->lpData = pBuffer1;
	pWaveHdr1->dwBufferLength = INP_BUFFER_SIZE;
	pWaveHdr1->dwBytesRecorded = 0;
	pWaveHdr1->dwUser = 0;
	pWaveHdr1->dwFlags = 0;
	pWaveHdr1->dwLoops = 1;
	pWaveHdr1->lpNext = 0;
	pWaveHdr1->reserved = 0;

	waveInPrepareHeader(hWaveIn, pWaveHdr1, sizeof(WAVEHDR));

	waveInAddBuffer(hWaveIn, pWaveHdr1, sizeof(WAVEHDR));

}

EXPORT BYTE** setupWaveIn()
{
	pSaveBuffer = malloc(1);

	waveform.wFormatTag = WAVE_FORMAT_PCM;
	waveform.nChannels = 1;
	waveform.nSamplesPerSec = 11025;
	waveform.nAvgBytesPerSec = 11025;	//format.nSamplesPerSec * format.nBlockAlign;
	waveform.nBlockAlign = 1;			// Convert.ToUInt16(format.nChannels * format.wBitsPerSample);
	waveform.wBitsPerSample = 8;
	waveform.cbSize = 0;

	//bufferLength = 16384;// format.nAvgBytesPerSec / 800;

	waveInOpen(&hWaveIn, WAVE_MAPPER, &waveform, (DWORD)&callbackWaveIn, 0, CALLBACK_FUNCTION);

	setupBufferIn();
	waveInStart(hWaveIn);

	return &pSaveBuffer;

}

EXPORT void CALLBACK callbackWaveIn(HWAVEIN hWaveIn, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR waveHdr, DWORD_PTR dwParam2)
{
	if (uMsg == WIM_CLOSE)
	{	//WIM_CLOSE
		
		bDone = TRUE;
		
	}
	else if (uMsg == WIM_OPEN)
	{	//WIM_OPEN
		bDone = FALSE;
		bRecording = TRUE;
		pSaveBuffer = realloc(pSaveBuffer, 1);
		waveInAddBuffer(hWaveIn, pWaveHdr1, sizeof(WAVEHDR));

		dwDataLength = 0;
		return;
	}
	else if (uMsg == WIM_DATA)
	{	//WIM_DATA

		pNewBuffer = realloc(pSaveBuffer, dwDataLength + ((PWAVEHDR)waveHdr)->dwBytesRecorded);

		if (pNewBuffer == NULL)
		{
			waveInClose(hWaveIn);
			return;
		}

		pSaveBuffer = pNewBuffer;
		CopyMemory(pSaveBuffer + dwDataLength, ((PWAVEHDR)waveHdr)->lpData, ((PWAVEHDR)waveHdr)->dwBytesRecorded);

		dwDataLength += ((PWAVEHDR)waveHdr)->dwBytesRecorded;

		//waveInAddBuffer(hWaveIn, pWaveHdr1, sizeof(WAVEHDR));
		if (bRecording)
		{
			setupBufferIn();
		}
	}
}

EXPORT BOOL* endRecording()
{
	bRecording = FALSE;
	waveInStop(hWaveIn);
	waveInUnprepareHeader(hWaveIn, pWaveHdr1, sizeof(WAVEHDR));
	Sleep(200);
	waveInReset(hWaveIn);
	waveInClose(hWaveIn);

	return &bDone;
}

EXPORT void setupBufferOut(PBYTE* samples)
{
	//if (pWaveHdr1)
	//	waveOutUnprepareHeader(hWaveOut, (pWaveHdr1), sizeof(WAVEHDR));

	//pBuffer1 = malloc(INP_BUFFER_SIZE);

	pWaveHdr1 = malloc(sizeof(WAVEHDR));
	pWaveHdr1->lpData = *samples; 
	pWaveHdr1->dwBufferLength = dwDataLength;
	pWaveHdr1->dwBytesRecorded = 0;
	pWaveHdr1->dwUser = 0;
	pWaveHdr1->dwFlags = 4 | 8;
	pWaveHdr1->dwLoops = 1;
	pWaveHdr1->lpNext = dwRepetitions;
	pWaveHdr1->reserved = 0;

	waveOutPrepareHeader(hWaveOut, pWaveHdr1, sizeof(WAVEHDR));

	//waveOutAddBuffer(hWaveOut, pWaveHdr1, sizeof(WAVEHDR));

}

EXPORT void setupWaveOut(PBYTE* samples)
{

	waveform.wFormatTag = WAVE_FORMAT_PCM;
	waveform.nChannels = 1;
	waveform.nSamplesPerSec = 11025;
	waveform.nAvgBytesPerSec = 11025;
	waveform.nBlockAlign = 1;
	waveform.wBitsPerSample = 8;
	waveform.cbSize = 0;

	waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveform, (DWORD)&callbackWaveOut, 0, CALLBACK_FUNCTION);

	setupBufferOut(samples);

	//waveOutStart(hWaveOut);
	waveOutWrite(hWaveOut, pWaveHdr1, sizeof(WAVEHDR));

}

EXPORT void callbackWaveOut(HWAVEOUT hWaveOut, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR waveHdr, DWORD_PTR dwParam2)
{
	if (uMsg == 956)
	{//WOM_CLOSE
	 //     i = waveInUnprepareHeader(deviceHandle, ref wavehdr, Convert.ToUInt32(Marshal.SizeOf(wavehdr)));
	 //     if (i != 0)
	 //     {
	 //         this.Text = "Error: waveInUnprepareHeader " + i;
	 //     }
	}
	else if (uMsg == 957)
	{//WOM_OPEN
		return;
	}
	else if (uMsg == 955)
	{//WOM_DONE
		//if (this.InvokeRequired)
		//    this.Invoke(waveIn, deviceHandle, message, instance, wavehdr, reserved2);
		//else
		waveOutUnprepareHeader(hWaveOut, pWaveHdr1, sizeof(WAVEHDR));
		waveOutClose(hWaveOut);
		return;
	}
}

EXPORT void pauseWave()
{
	waveOutPause(hWaveOut);
}

EXPORT void resumeWave()
{
	waveOutRestart(hWaveOut);
}

EXPORT int getLengthOfSamples()
{
	return dwDataLength;
}

EXPORT void setLengthOfSamples(DWORD length)
{
	dwDataLength = length;
}

EXPORT BYTE** initializeSamples(int numBytes)
{
	pSaveBuffer = (PBYTE)malloc((size_t)numBytes);

	return &pSaveBuffer;
}