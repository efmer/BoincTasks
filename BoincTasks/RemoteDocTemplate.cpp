#include "stdafx.h"
#include "BoincTasks.h"
#include "RemoteDocTemplate.h"


CRemoteDocTemplate::CRemoteDocTemplate(UINT nIDResource,
     CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass,
     CRuntimeClass* pViewClass) : CMultiDocTemplate(nIDResource, pDocClass,
                                    pFrameClass, pViewClass)
   {
   }

   CRemoteDocTemplate::~CRemoteDocTemplate()
   {
     // This prevents the base class virtual destructor from
     // destroying the menu resource (i.e. its checks to make
     // sure the handle isn't NULL)

     m_hMenuShared = NULL;
   }