#pragma once
#include "cSettings.h"

//#define RESTRICTED_VERSION
#ifdef RESTRICTED_VERSION
	#define MAX_BUTTON_PRESSES 33
#endif

#define SETTINGS_FILE "siren.cfg"
#define FREQUENCY (int)(cSettings::cfg.getRoot()["audio"]["frequency"])
#define MAX_SAMPLE_VAL (int)(cSettings::cfg.getRoot()["audio"]["max_sample_val"])
#define US2SIZE(us) (((us) * FREQUENCY) / 1000000)
#define SEC2DELTA(sec, fmin, fmax) (((fmax) - (fmin)) / (FREQUENCY * (sec)))
// Macro to release a COM object
#define ReleaseCOM(x) if(x) { x->Release(); x = NULL; }
#define ERRMSG(Msg) MessageBox(NULL, Msg, _T("Error"), MB_OK | MB_ICONEXCLAMATION)