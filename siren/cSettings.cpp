#include "stdafx.h"
#include "cSettings.h"
#include "common.h"
#include <io.h>
#include <sstream>

#ifdef _DEBUG
	#pragma comment (lib,  ".\\libconfig-1.7.2\\Debug\\libconfig++_d.lib")
#else
	#pragma comment (lib,  ".\\libconfig-1.7.2\\Release\\libconfig++.lib")
#endif
#pragma comment (lib,  "Shlwapi.lib")

Config cSettings::cfg;
cSettings settings;


cSettings::cSettings()
{
	if (_access(SETTINGS_FILE, 4)) // Read-only
	{
		Setting &root = cfg.getRoot();

		// Add some settings to the configuration.
		Setting &audio = root.add("audio", Setting::TypeGroup);
		audio.add("frequency", Setting::TypeInt) = 22050;
		audio.add("max_sample_val", Setting::TypeInt) = 0xff;

		Setting &fx = root.add("fx", Setting::TypeGroup);
		Setting &horn = fx.add("horn", Setting::TypeGroup);
		Setting &table = horn.add("table", Setting::TypeArray);
		const short a_horn[] = { 1300, 1070, 900, 750, 620, 520, 450, 370, 310, 250, 220, 180, 150, 120, 100, 90, 100, 120, 150, 180, 220, 310, 370, 450, 520, 620, 750, 900, 1070, 1300 };
		const int ci_cnt = sizeof(a_horn) / sizeof(short);

		for (int i = 0; i < ci_cnt; i++)
			table.add(Setting::TypeInt) = a_horn[i];

		Setting &wail = fx.add("wail", Setting::TypeGroup);
		wail.add("fmin", Setting::TypeFloat) = 400.0f;
		wail.add("fmax", Setting::TypeFloat) = 2000.0f;
		wail.add("fsec", Setting::TypeFloat) = 5.0f;

		Setting &yelp1 = fx.add("yelp1", Setting::TypeGroup);
		yelp1.add("fmin", Setting::TypeFloat) = 400.0f;
		yelp1.add("fmax", Setting::TypeFloat) = 2000.0f;
		yelp1.add("fsec", Setting::TypeFloat) = 0.6f;

		Setting &yelp2 = fx.add("yelp2", Setting::TypeGroup);
		yelp2.add("fmin", Setting::TypeFloat) = 400.0f;
		yelp2.add("fmax", Setting::TypeFloat) = 2000.0f;
		yelp2.add("fsec", Setting::TypeFloat) = 0.35;

		Setting &fast = fx.add("fast", Setting::TypeGroup);
		fast.add("fmin", Setting::TypeFloat) = 400.0f;
		fast.add("fmax", Setting::TypeFloat) = 2000.0f;
		fast.add("fsec", Setting::TypeFloat) = 0.175;

		Setting &short_end = fx.add("short_end", Setting::TypeGroup);
		short_end.add("fmin", Setting::TypeFloat) = 250.0f;
		short_end.add("fmax", Setting::TypeFloat) = 800.0f;
		short_end.add("fsec", Setting::TypeFloat) = 5.0f;

		Setting &hilo = fx.add("hilo", Setting::TypeGroup);
		hilo.add("fmin", Setting::TypeFloat) = 975.0f;
		hilo.add("fmax", Setting::TypeFloat) = 1275.0f;
		hilo.add("fsec", Setting::TypeFloat) = 0.5285f;

		Setting &manual = fx.add("manual", Setting::TypeGroup);
		manual.add("fmin", Setting::TypeFloat) = 200.0f;
		manual.add("fmax", Setting::TypeFloat) = 1350.0f;
		manual.add("fsec_asc", Setting::TypeFloat) = 2.5f;
		manual.add("fsec_desc", Setting::TypeFloat) = 2.5f * 2;
		manual.add("high_tone_sec", Setting::TypeFloat) = 0.2f;

		// Write out the new configuration.
		try
		{
			cfg.writeFile(SETTINGS_FILE);
		}
		catch (const FileIOException &/*fioex*/)
		{
			std::stringstream ss_err;
			ss_err << "I/O error while writing file: " << SETTINGS_FILE;
			std::string s_err = ss_err.str();
			std::wstring ws_err(s_err.begin(), s_err.end());
			ERRMSG(ws_err.c_str());
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		try
		{
			cfg.readFile(SETTINGS_FILE);
		}
		catch (const FileIOException &fioex)
		{
			ERRMSG(L"I/O error while reading file.");
			exit(EXIT_FAILURE);
		}
		catch (const ParseException &pex)
		{
			std::stringstream ss_err;
			ss_err << "Parse error at " << pex.getFile() << ":" << pex.getLine() << " - " << pex.getError();
			std::string s_err = ss_err.str();
			std::wstring ws_err(s_err.begin(), s_err.end());
			ERRMSG(ws_err.c_str());
			exit(EXIT_FAILURE);
		}

		//int f = 0;
		//cfg.lookupValue("audio.frequency", f);
		//f = cfg.getRoot()["audio"]["frequency"];
		//Sleep(0);
	}
}


cSettings::~cSettings()
{
}
