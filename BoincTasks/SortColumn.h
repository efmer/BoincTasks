#pragma once

#define MAX_SORTING_COLUMN		20

#define SORT_STRING_UP_PRIMARY		"^ "
#define SORT_STRING_UP_SECONDARY	"^2 "
#define SORT_STRING_UP_TERTIARY		"^3 "
#define SORT_STRING_DOWN_PRIMARY	"v "
#define SORT_STRING_DOWN_SECONDARY	"v2 "
#define SORT_STRING_DOWN_TERTIARY	"v3 "
#define SORT_STRING_NONE_SECONDARY	""
#define SORT_STRING_NONE_TERTIARY	""

#define SORT_PRIMARY				0
#define SORT_SECONDARY				1
#define SORT_TERTIARY				2

class CSortColumn
{
public:
	CSortColumn(void);
	~CSortColumn(void);

	void SetColumnOrder(int iOrderCount, int *pOrder);
	void OrderChanged(int iOrderCount, int *pOrder);
	void Setup(CListCtrl *pListCtrl, HWND hWnd, UINT iNrColums, char *pRegistrySortTasksPrimary, char *pRegistrySortTasksSecondary,  char *pRegistrySortTasksTertiary, int iDefaultPrimary, int iDefaultSecondary, int iDefaultTertiary);
	void RemoveDoubles(UINT *piColumnPrimary, UINT *piColumnSecondary, UINT *piColumnTertiary, UINT *piSortingSecondary, UINT *piSortingTertiary);
	void GetColumnOption(UINT *piSortingColumnPrimary, UINT *piSortingOptionPrimary, UINT *piSortingColumnSecondary, UINT *piSortingOptionSecondary, UINT *piSortingColumnTertiary, UINT *piSortingOptionTertiary);
	void ClickItem(NMHDR* pNMHDR, LRESULT* pResult, int iColumn = -1);
	void WriteRegistry();
	void SetSortingIndicator(int iColumnPrimary, int iSortingPrimary, int iColumnSecondary, int iSortingSecondary,  int iColumnTertiary, int iSortingTertiary);

	void WebSorting(char *pcSort);
	void SetSorting(int iColumn, bool bLControlKey, bool bRControlKey);
	void ToggleSorting(int iColumn);

private:
	UINT	m_iSortingColumnPrimary;
	UINT	m_iSortingOptionPrimary;
	UINT	m_iSortingColumnSecondary;
	UINT	m_iSortingOptionSecondary;
	UINT	m_iSortingOptionTertiary;
	UINT	m_iSortingColumnTertiary;

	char	*m_pRegistrySortPrimary;
	char	*m_pRegistrySortSecondary;
	char	*m_pRegistrySortTertiary;

	UINT	m_iColumnOrder[MAX_SORTING_COLUMN+1];



	CListCtrl *m_pListCtrl;
	HWND	m_hWnd;

	int		m_iNrColomns;

};
