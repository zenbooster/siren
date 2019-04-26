#include "stdafx.h"
#include "cManual.h"

void cManual::_threadFunction()
{
	while (!is_thr_terminate)
	{
		int processed = GetProcessed();

		if (processed)
		{
			UnqueueBuffer(processed);
			QueueBuffer("*manual_high");
			is_thr_action = true;
		}
		Sleep(100);
	}
	is_thr_terminate = false;
};

cManual::cManual():
	pthr(NULL)
{
}


cManual::~cManual()
{
	Close();
}

bool cManual::init()
{
	mLooped = false;
	DefaultSource();

	/*
	if (!manual_asc.init())
	ERRMSG(_T("manual_asc.init() failed!"));
	if (!manual_desc.init())
	ERRMSG(_T("manual_desc.init() failed!"));
	if (!manual_high.init())
	ERRMSG(_T("manual_high.init() failed!"));
	*/
	bool res = manual_asc_buffer.init();
	res &= manual_desc_buffer.init();
	res &= manual_high_buffer.init();

	QueueBuffer("*manual_asc");

	return res;
}

ALdouble cManual::_cvt_asc2desc_ofs(ALdouble ofs) const
{
	return ((ofs * manual_desc_buffer.get_size()) / manual_asc_buffer.get_size());
}

ALdouble cManual::_cvt_desc2asc_ofs(ALdouble ofs) const
{
	return ((ofs * manual_asc_buffer.get_size()) / manual_desc_buffer.get_size());
}

void cManual::on_highlight()
{
	bool is_running = IsPlaying();

	if (is_running)
	{
		int processed = GetProcessed();
		UnqueueBuffer(processed);

		ALint ofs = GetSampleOffset();
		QueueBuffer("*manual_asc");
		QueueBuffer("*manual_high");

		SetSampleOffset(manual_desc_buffer.get_size() + manual_asc_buffer.get_size() - _cvt_desc2asc_ofs(ofs) - 1);
		UnqueueBuffer();
	}
	else
	{
		int processed = GetProcessed();
		if (processed)
		{
			// ≈сли сирена стартовала и остановилась, убираем последние проигранные буфера:
			UnqueueBuffer(processed); // если поток успел подгрузить очередной manual_high, надо будет удалить 2 буфера.
																			 // делаем как при первом запуске:
			QueueBuffer("*manual_asc");
		}

		QueueBuffer("*manual_high");
		Play();
	}
	is_thr_terminate = false;
	is_thr_action = false;
	pthr = new std::thread(std::mem_fn(&cManual::_threadFunction), this);
}

void cManual::on_unhighlight()
{
	is_thr_terminate = true;
	pthr->join();
	delete pthr;
	pthr = NULL;

	QueueBuffer("*manual_desc");
	int processed = GetProcessed();

	if (is_thr_action)
	{
		SetSampleOffset(manual_high_buffer.get_size() * 2);
	}
	else
	{
		ALint ofs = GetSampleOffset();
		SetSampleOffset(manual_asc_buffer.get_size() + manual_high_buffer.get_size() + manual_desc_buffer.get_size() - _cvt_asc2desc_ofs(ofs));
	}
}
