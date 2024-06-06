
#pragma once

class CRemoteDocTemplate : public CMultiDocTemplate
   {
     public:
       CRemoteDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
           CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);

       virtual ~CRemoteDocTemplate();
   };


				