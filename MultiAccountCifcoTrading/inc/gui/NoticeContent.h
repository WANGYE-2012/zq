class UserNoticeDlg;
class NoticeContent
{
public:
    NoticeContent();
    ~NoticeContent();
    void SetField(CThostFtdcNoticeField& field);
    void SetFinished(){m_bUserNoticeFinished=true;}
    bool GetFinished(){return m_bUserNoticeFinished;}
    wxString GetContent();   
private:
    wxArrayString m_UserNoticeContent;//���һλ����100��
    bool m_bUserNoticeFinished; 
};