#pragma once
#include "wx/textctrl.h"

static const char g_slide[] = {'0','1','2','3','4','5','6','7','8','9','0','-','\0'};
static const char g_assign[] = {'0','1','2','3','4','5','6','7','8','9','0','.','\0'};

class CNumericTextCtrl : public wxTextCtrl
{
public:
	CNumericTextCtrl();
	CNumericTextCtrl(wxWindow *parent,
					wxWindowID id, 
					const wxString &value=wxEmptyString, 
					const wxPoint &pos=wxDefaultPosition, 
					const wxSize &size=wxDefaultSize, 
					long style=0, 
					const wxValidator &validator=wxDefaultValidator, 
					const wxString &name=wxTextCtrlNameStr);
	~CNumericTextCtrl();

	//���û�е��������������������ַ����ϣ�Ĭ�Ͻ���������0~9������
	void SetFilter(const char * pChar)
	{
		m_pCharSet = pChar;
	}

protected:
	void OnChar(wxKeyEvent& event);

private:
	const char * m_pCharSet;

};