#pragma once
#include "remSnd.h"
#include "cManualAscBuffer.h"
#include "cManualDescBuffer.h"
#include "cManualHighBuffer.h"
#include <thread>
#include <atomic>

class cManual :
	public remSnd
{
	cManualAscBuffer manual_asc_buffer;
	cManualDescBuffer manual_desc_buffer;
	cManualHighBuffer manual_high_buffer;

	std::thread *pthr;
	std::atomic<bool> is_thr_terminate;
	std::atomic<bool> is_thr_action;

	ALdouble _cvt_asc2desc_ofs(ALdouble ofs) const;
	ALdouble _cvt_desc2asc_ofs(ALdouble ofs) const;

	void _threadFunction();

public:
	cManual();
	~cManual();

	bool init();

	void on_highlight();
	void on_unhighlight();
};

