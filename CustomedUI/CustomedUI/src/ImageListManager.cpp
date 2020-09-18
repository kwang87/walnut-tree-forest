//////////////////////////////////////////////////////////////////////
// ImageListManager.cpp
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "ImageListManager.h"

ImageListManager* ImageListManager::m_pImageListManager = 0;

ImageListManager::ImageListManager() : m_pCurrentImageList(0)
{
	m_mImageList.clear();
}

ImageListManager::~ImageListManager()
{
	m_pCurrentImageList = 0;

	mapImageList::iterator iter;
	for(iter = m_mImageList.begin(); iter != m_mImageList.end(); iter++){
		(iter->second)->DeleteImageList();
		delete (iter->second);
	}

	m_mImageList.clear();
	
	m_pImageListManager = 0;
}

void ImageListManager::initialize()
{
	m_mImageList.clear();
}

void ImageListManager::destroy()
{
	delete this;
}

bool ImageListManager::addImageList(unsigned int nResourceID, int nImageWidth, COLORREF colTransparent)
{
	CBitmap bmpTemp;
 
	if(bmpTemp.LoadBitmap(nResourceID)){
		BITMAP bm;
		bmpTemp.GetBitmap(&bm);

		CImageList* pImageList = new CImageList;

		pImageList->Create(nImageWidth, bm.bmHeight,ILC_MASK | ILC_COLORDDB,0,0);
		pImageList->Add(&bmpTemp, colTransparent);

		m_mImageList.insert(mapImageList::value_type(nResourceID, pImageList));
		
		bmpTemp.DeleteObject();

		return true;
	}

	return false;
}

void ImageListManager::drawImage(CDC* pDC, POINT pt, unsigned int nResourceID, int nIndex)
{
	mapImageList::iterator iter = m_mImageList.find(nResourceID);

	if(iter != m_mImageList.end()){
 		(iter->second)->Draw(pDC, nIndex, pt, ILD_TRANSPARENT);
		m_pCurrentImageList = (iter->second);
	}
} 

void ImageListManager::drawImageOn(CDC* pDC, POINT pt, int nIndex)
{
	if(m_pCurrentImageList){
 		m_pCurrentImageList->Draw(pDC, nIndex, pt, ILD_TRANSPARENT);
	}
} 
