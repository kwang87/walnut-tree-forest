#ifndef __DSUICOMUTIL_H__
#define __DSUICOMUTIL_H__

#include "stdafx.h"

class AFX_CLASS_EXPORT UIComUtil
{
public:
	UIComUtil();
	virtual ~UIComUtil();

	static int  SaveToClipboard(LPSTR lpszBuffer);
};

#endif // __DSUICOMUTIL_H__
