// ParameterTree.cpp : implementation file
//

#include "stdafx.h"
#include "ParameterTree.h"


// CParameterTree

IMPLEMENT_DYNAMIC(CParameterTree, CTreeCtrl)

CParameterTree::CParameterTree()
{
	m_uFlags=0;
	m_eType = PERMISSION_BKG;
	m_mapModuleData.clear();
}

CParameterTree::~CParameterTree()
{
}


BEGIN_MESSAGE_MAP(CParameterTree, CTreeCtrl)
    ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnStateIconClick)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

void CParameterTree::OnStateIconClick( NMHDR* pNMHDR, LRESULT* pResult )
{
	if(m_uFlags&TVHT_ONITEMSTATEICON) *pResult=1;
	else *pResult = 0;
}

void CParameterTree::OnLButtonDown( UINT nFlags, CPoint point )
{
	HTREEITEM hItem =HitTest(point, &m_uFlags);
	if ( (m_uFlags&TVHT_ONITEMSTATEICON ))
	{
		//nState: 0->��ѡ��ť 1->û��ѡ�� 2->����ѡ�� 3->ȫ��ѡ��
		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		nState=(nState==3)?1:3;
		(this->GetParent())->PostMessage(WM_USER + 12,0,(LPARAM)hItem);
		SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );

	}

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CParameterTree::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags )
{
	//����ո��
	if(nChar==0x20)
	{
		HTREEITEM hItem =GetSelectedItem();
		UINT nState = GetItemState( hItem, TVIS_STATEIMAGEMASK ) >> 12;
		if(nState!=0)
		{
			nState=(nState==3)?1:3;
			SetItemState( hItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		}
	}
	else CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CParameterTree::OnKeydown( NMHDR* pNMHDR, LRESULT* pResult )
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;

	*pResult = 0;
}

BOOL CParameterTree::SetItemState( HTREEITEM hItem, UINT nState, UINT nStateMask, BOOL bSearch/*=TRUE*/ )
{
	BOOL bReturn=CTreeCtrl::SetItemState( hItem, nState, nStateMask );

	UINT iState = nState >> 12;
	if(iState!=0)
	{
		if(bSearch) TravelChild(hItem, iState);
		TravelSiblingAndParent(hItem,iState);
	}
	return bReturn;
}
void CParameterTree::TravelChild( HTREEITEM hItem,int nState )
{
	HTREEITEM hChildItem,hBrotherItem; // �� ��

	//�����ӽڵ㣬û�оͽ���
	hChildItem=GetChildItem(hItem);
	if(hChildItem!=NULL)
	{
		//this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hChildItem);

		//�����ӽڵ��״̬�뵱ǰ�ڵ��״̬һ��
		CTreeCtrl::SetItemState( hChildItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
		//�ٵݹ鴦���ӽڵ���ӽڵ���ֵܽڵ�
		TravelChild(hChildItem, nState);

		//�����ӽڵ���ֵܽڵ�����ӽڵ�
		hBrotherItem=GetNextSiblingItem(hChildItem);
		while (hBrotherItem)
		{
			//this->GetParent()->PostMessage(WM_USER+12,0,(LPARAM)hBrotherItem);
			//�����ӽڵ���ֵܽڵ�״̬�뵱ǰ�ڵ��״̬һ��
			int nState1 = GetItemState( hBrotherItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				CTreeCtrl::SetItemState( hBrotherItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//�ٵݹ鴦���ӽڵ���ֵܽڵ���ӽڵ���ֵܽڵ�
			TravelChild(hBrotherItem, nState);
			hBrotherItem=GetNextSiblingItem(hBrotherItem);
		}
	}
}


void CParameterTree::TravelSiblingAndParent( HTREEITEM hItem, int nState )
{
	HTREEITEM hNextSiblingItem,hPrevSiblingItem,hParentItem;  //��һ�������ֵ� ��һ�������ֵ�  ���׽ڵ�

	//���Ҹ��ڵ㣬û�оͽ���
	hParentItem=GetParentItem(hItem);
	if(hParentItem!=NULL)
	{
		int nState1=nState;//���ʼֵ����ֹû���ֵܽڵ�ʱ����

		//���ҵ�ǰ�ڵ�������ֵܽڵ��״̬
		hNextSiblingItem=GetNextSiblingItem(hItem);
		while(hNextSiblingItem!=NULL)
		{
			nState1 = GetItemState( hNextSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=nState && nState1!=0) break;
			else hNextSiblingItem=GetNextSiblingItem(hNextSiblingItem);
		}

		if(nState1==nState)
		{
			//���ҵ�ǰ�ڵ�������ֵܽڵ��״̬
			hPrevSiblingItem=GetPrevSiblingItem(hItem);
			while(hPrevSiblingItem!=NULL)
			{
				nState1 = GetItemState( hPrevSiblingItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=nState && nState1!=0) break;
				else hPrevSiblingItem=GetPrevSiblingItem(hPrevSiblingItem);
			}
		}

		if(nState1==nState || nState1==0)
		{
			nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
			if(nState1!=0)
			{
				//���״̬һ�£��򸸽ڵ��״̬�뵱ǰ�ڵ��״̬һ��
				CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK );
			}
			//�ٵݹ鴦���ڵ���ֵܽڵ���丸�ڵ�
			TravelSiblingAndParent(hParentItem,nState);
		}
		else
		{
			//״̬��һ�£���ǰ�ڵ�ĸ��ڵ㡢���ڵ�ĸ��ڵ㡭��״̬��Ϊ����̬
			hParentItem=GetParentItem(hItem);
			while(hParentItem!=NULL)
			{
				nState1 = GetItemState( hParentItem, TVIS_STATEIMAGEMASK ) >> 12;
				if(nState1!=0)
				{
					CTreeCtrl::SetItemState( hParentItem, INDEXTOSTATEIMAGEMASK(2), TVIS_STATEIMAGEMASK );
				}
				hParentItem=GetParentItem(hParentItem);
			}
		}
	}	
}

void CParameterTree::ClearItems()
{
	DeleteAllItems();

	m_uFlags=0;
	m_eType = PERMISSION_BKG;
	m_mapModuleData.clear();
}

void CParameterTree::SetPermissionModule( std::map<int, PermissionModule> mapModuleData )
{
	m_mapModuleData = mapModuleData;
}

void CParameterTree::ShowPermissionModule(PermissionType eType)
{
	DeleteAllItems();

	InsertItem( _T( "ȫѡ" ), TVI_ROOT );
	m_eType = eType;
	std::map<int, PermissionModule>::iterator it = m_mapModuleData.begin();
	for( ; it != m_mapModuleData.end(); it++ )
	{
		PermissionModule module = it->second;
		if ( module.nModuleType != m_eType )
		{
			continue;
		}

		HTREEITEM hRootItem = GetRootItem();
		HTREEITEM hModuleItem = InsertItem( module.szModuleName, hRootItem, TVI_LAST);
		SetItemData(hModuleItem, module.nModuleID);

		HTREEITEM hPermissionItem = NULL;
		if ( m_eType == PERMISSION_BKG )
		{
			if ( module.nCommonOperators & PERMISSION_BKG_COMMON_ADD )
			{
				HTREEITEM hPermissionItem = InsertItem( PERMISSION_STRING_ADD, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, PERMISSION_BKG_COMMON_ADD);
			}

			if ( module.nCommonOperators & PERMISSION_BKG_COMMON_MODIFY )
			{
				HTREEITEM hPermissionItem = InsertItem( PERMISSION_STRING_MODIFY, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, PERMISSION_BKG_COMMON_MODIFY);
			}

			if ( module.nCommonOperators & PERMISSION_BKG_COMMON_DEL )
			{
				HTREEITEM hPermissionItem = InsertItem( PERMISSION_STRING_DEL, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, PERMISSION_BKG_COMMON_DEL);
			}

			if ( module.nCommonOperators & PERMISSION_BKG_COMMON_QUERY )
			{
				HTREEITEM hPermissionItem = InsertItem( PERMISSION_STRING_QUERY, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, PERMISSION_BKG_COMMON_QUERY);
			}

			if ( module.nCommonOperators & PERMISSION_BKG_COMMON_CHANGE_STATUS )
			{
				HTREEITEM hPermissionItem = InsertItem( PERMISSION_STRING_CHANGE_STATUS, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, PERMISSION_BKG_COMMON_CHANGE_STATUS);
			}

			if ( module.nCommonOperators & PERMISSION_BKG_COMMON_ADD_MODIFY )
			{
				HTREEITEM hPermissionItem = InsertItem( PERMISSION_STRING_ADD_MODIFY, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, PERMISSION_BKG_COMMON_ADD_MODIFY);
			}
		}
		else
		{
			if ( module.nCommonOperators & PERMISSION_FORE_LOGON )
			{
				HTREEITEM hPermissionItem = InsertItem( PERMISSION_STRING_LOGON, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, PERMISSION_FORE_LOGON);
			}

			if ( module.nCommonOperators & PERMISSION_FORE_ADD_TRADE_ACCOUNT )
			{
				HTREEITEM hPermissionItem = InsertItem( PERMISSION_STRING_ADD_TRADE_ACCOUNT, hModuleItem, TVI_LAST);
				SetItemData(hPermissionItem, PERMISSION_FORE_ADD_TRADE_ACCOUNT);
			}
		}
	}
}

void CParameterTree::SetPermissionData( std::map<int, PermissionData> mapData )
{
	if ( mapData.empty())
	{
		return;
	}

	HTREEITEM hRootItem = GetRootItem();
	HTREEITEM hChildItem = GetChildItem( hRootItem );
	while ( NULL != hChildItem )
	{
		//��ȡȨ��ģ��ID���������ӽڵ��ѡ��״̬
		int nModuleID = GetItemData(hChildItem);
		std::map<int, PermissionData>::iterator it = mapData.find(nModuleID);
		if ( it != mapData.end() )
		{
			HTREEITEM hSubItem = GetChildItem( hChildItem );
			while( NULL != hSubItem )
			{
				DWORD dwData = GetItemData(hSubItem);
				if ( dwData&it->second.nCommonOperators )
				{
					int nState = 3;
					SetItemState(hSubItem, INDEXTOSTATEIMAGEMASK(nState), TVIS_STATEIMAGEMASK);
				}

				hSubItem = GetNextSiblingItem(hSubItem);
			}
		}

		hChildItem = GetNextSiblingItem(hChildItem);
	}
}

void CParameterTree::GetPermissionData( std::vector<PermissionData>& vData )
{
	vData.clear();

	HTREEITEM hRootItem = GetRootItem();
	HTREEITEM hChildItem = GetChildItem( hRootItem );
	while ( NULL != hChildItem )
	{
		//��ȡ�ڵ�ѡ��״̬
		PermissionData data;
		memset(&data, 0, sizeof(data));
		data.nModuleID = GetItemData(hChildItem);
		int nState = GetItemState( hChildItem, TVIS_STATEIMAGEMASK ) >> 12;
		if ( 2 == nState || 3 == nState )
		{
			HTREEITEM hSubItem = GetChildItem( hChildItem );
			while( NULL != hSubItem )
			{
				int nSubState = GetItemState( hSubItem, TVIS_STATEIMAGEMASK ) >> 12;
				if ( 2 == nSubState || 3 == nSubState)
				{
					data.nCommonOperators |= GetItemData(hSubItem);
				}

				hSubItem = GetNextSiblingItem(hSubItem);
			}
		}

		if ( data.nCommonOperators != 0 )
		{
			vData.push_back(data);
		}

		hChildItem = GetNextSiblingItem(hChildItem);
	}
}

PermissionType CParameterTree::GetCurShowType()
{
	return m_eType;
}
