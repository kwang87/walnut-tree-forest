//////////////////////////////////////////////////////////////////////
// ImageListManager.h:
//////////////////////////////////////////////////////////////////////

#ifndef __RESOURCEMANAGER_H__
#define __RESOURCEMANAGER_H__

#include <map>

using namespace std;

class AFX_CLASS_EXPORT ImageListManager
{
protected:
	static ImageListManager* m_pImageListManager;

	typedef map<unsigned int, CImageList*> mapImageList;
	mapImageList  m_mImageList;
 
	CImageList* m_pCurrentImageList;

	ImageListManager();
	virtual ~ImageListManager();

public:
	inline static ImageListManager* getInstance()
	{
		if(m_pImageListManager == 0)
			m_pImageListManager = new ImageListManager();

		return m_pImageListManager;
	}

	void initialize();
	void destroy();
	
	bool addImageList(unsigned int nResourceID, int nImageWidth, COLORREF colTransparent);
	void drawImage(CDC* pDC, POINT pt, unsigned int nResourceID, int nIndex);
	void drawImageOn(CDC* pDC, POINT pt, int nIndex);
};

#endif // __RESOURCEMANAGER_H__

