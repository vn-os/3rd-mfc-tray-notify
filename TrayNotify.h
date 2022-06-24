#pragma once

#include <afxwin.h>

#define WM_US_TRAY_NOTIFY (WM_USER + 1)

class TrayNotify : public CWnd
{
  DECLARE_DYNAMIC(TrayNotify)
public:
  TrayNotify() noexcept;
  virtual ~TrayNotify();

  int Setup(CWnd* pParent, UINT ID);

public:
  afx_msg LRESULT OnNotification(WPARAM wParam, LPARAM lParam);
  DECLARE_MESSAGE_MAP()

protected:
  CMenu m_Menu;
  CWnd* m_pParent;
  NOTIFYICONDATA m_Tray;
};

#define SYSTEM_TRAY_DECL_MFC()\
private:\
  TrayNotify m_Tray;\
protected:\
  afx_msg LRESULT OnTrayNotification(WPARAM wParam, LPARAM lParam)\
  {\
    return m_Tray.OnNotification(wParam, lParam);\
  }

#define SYSTEM_TRAY_IMPL_MFC(parent, menu)\
  m_Tray.Setup(parent, menu);

#define ON_WM_SYSTEM_TRAY_NOTIFY()\
  ON_MESSAGE(WM_US_TRAY_NOTIFY, OnTrayNotification)
