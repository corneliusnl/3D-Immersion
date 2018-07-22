#ifndef MULTIMEDIA_MULTIMEDIA_H
#define MULTIMEDIA_MULTIMEDIA_H
// ------------------------------------------------------------
// Midi Music
// Copyright 1997 (c) Leprechaun Software.  All Rights Reserved.
// ------------------------------------------------------------
#include <Windows.h>
#include <MMSystem.h>
#include <DigitalV.h>

//
// Defines
//
#define TYPE_MIDI 	"sequencer"
#define TYPE_CD		"cdaudio"
#define TYPE_WAVE		"waveaudio"
#define TYPE_AVI		"avivideo"

//
// Function Prototypes
//
void ShowError(DWORD dwError);

// ------------------------------------------------------------
// Midi Music
// ------------------------------------------------------------
class MCISEQ
{
private:
	MCI_OPEN_PARMS		MciOpen;
	MCI_PLAY_PARMS		MciPlay;
	MCI_GENERIC_PARMS	MciGeneric;

public:
	MCIDEVICEID			wDeviceID;
   HWND					hParent;

	// Construction/Destruction
	MCISEQ(HWND hWnd, char *Filename);
	~MCISEQ();

	// Methods
	int Play();
	int Stop();
	int Pause();
	int Resume();
};

// ------------------------------------------------------------
// AVI Video
// ------------------------------------------------------------
class AVIVIDEO
{
private:
	MCI_DGV_OPEN_PARMS      mciOpen;
	MCI_DGV_WINDOW_PARMS    mciWindow;
	MCI_DGV_PUT_PARMS			mciPut;
	MCI_DGV_PLAY_PARMS      mciPlay;
	MCI_STATUS_PARMS        mciStatus;
	MCI_GENERIC_PARMS  		mciGeneric;

public:
	HWND		hParent;
	WORD		DeviceID;

	AVIVIDEO(HWND hWnd, char *Filename);
	~AVIVIDEO();

	int	SetWindow();
	int	Put(int X, int Y, int Width, int Height);
	int	Play();
	int	Stop();
	int	Pause();
	int	Resume();
};

#endif
