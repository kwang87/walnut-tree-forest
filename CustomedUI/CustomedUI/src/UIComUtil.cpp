#include "stdafx.h"
#include "UIComUtil.h"

UIComUtil::UIComUtil()
{
}

UIComUtil::~UIComUtil()
{
}

int UIComUtil::SaveToClipboard(LPSTR lpszBuffer)
{
	HGLOBAL hGlobal;
	LPSTR lpszData;
	unsigned long nSize;

 	OpenClipboard(0);
	EmptyClipboard();

	nSize = lstrlenA(lpszBuffer);

	hGlobal = GlobalAlloc(GMEM_ZEROINIT, nSize+1);

	if(hGlobal == NULL)
		return FALSE;

	lpszData = (LPSTR)GlobalLock(hGlobal);

	for (UINT i = 0; i < nSize + 1; ++i)
		*(lpszData + i) = *(lpszBuffer + i);

	GlobalUnlock(hGlobal);
	SetClipboardData(CF_TEXT, hGlobal);
	CloseClipboard();

	return TRUE;
}
