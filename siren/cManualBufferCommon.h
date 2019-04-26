#pragma once
#include "cSettings.h"

#define FMIN (float)(cSettings::cfg.getRoot()["fx"]["manual"]["fmin"])
#define FMAX (float)(cSettings::cfg.getRoot()["fx"]["manual"]["fmax"])
#define MANUAL_ASC_SPEED SEC2DELTA((float)(cSettings::cfg.getRoot()["fx"]["manual"]["fsec_asc"]), FMIN, FMAX)
#define MANUAL_DESC_SPEED SEC2DELTA((float)(cSettings::cfg.getRoot()["fx"]["manual"]["fsec_desc"]), FMIN, FMAX)
#define MANUAL_HIGH_SEC (float)(cSettings::cfg.getRoot()["fx"]["manual"]["high_tone_sec"])