// CreateHtmlWebServerFilter.cpp : implementation file
//

#include "stdafx.h"
#include "BoincTasks.h"
#include "CreateHtmlWebServerFilter.h"


// CreateHtmlWebServerFilter

CreateHtmlWebServerFilter::CreateHtmlWebServerFilter()
{
}

CreateHtmlWebServerFilter::~CreateHtmlWebServerFilter()
{
}

// CreateHtmlWebServerFilter member functions


CString CreateHtmlWebServerFilter::Create(CString sFilter, int iFilterId, int iFilter)
{
	CString sFilterTemplate, sFormat;
	
	sFormat.Format("%d,%d", iFilterId, iFilter);

	sFilterTemplate = "<a href='#' OnClick='filter(__nr__)' data-role='button' id='button-filter__nr__' data-theme='b' data-mini='true' data-icon='__icon__'>__filter_name__</a>";

	sFilterTemplate.Replace("__filter_name__", sFilter);

	sFilterTemplate.Replace("__nr__", sFormat);

	if (iFilter) sFilterTemplate.Replace("__icon__", "check");
	else		 sFilterTemplate.Replace("__icon__", "");
	
	return sFilterTemplate;

}