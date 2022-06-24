#include "TrayNotify.h"

IMPLEMENT_DYNAMIC(TrayNotify, CWnd)

BEGIN_MESSAGE_MAP(TrayNotify, CWnd)
END_MESSAGE_MAP()

TrayNotify::TrayNotify() noexcept : m_pParent(nullptr)
{
}

TrayNotify::~TrayNotify()
{
  Shell_NotifyIcon(NIM_DELETE, &m_Tray);
}

int TrayNotify::Setup(CWnd* pParent, UINT ID)
{
  m_pParent = pParent;

  m_Tray.cbSize = sizeof(m_Tray);
  //Size of this structure, in bytes. 

  m_Tray.hWnd = m_pParent->GetSafeHwnd();
  //Handle to the window that receives notification 
  //messages associated with an icon in the taskbar 
  //status area. The Shell uses hWnd and uID to 
  //identify which icon to operate on when Shell_NotifyIcon is invoked. 

  m_Tray.uID = 1;
  //Application-defined identifier of the taskbar icon.
  //The Shell uses hWnd and uID to identify which icon 
  //to operate on when Shell_NotifyIcon is invoked. You
  // can have multiple icons associated with a single 
  //hWnd by assigning each a different uID. 

  m_Tray.uCallbackMessage = WM_US_TRAY_NOTIFY;
  //Application-defined message identifier. The system 
  //uses this identifier to send notifications to the 
  //window identified in hWnd. These notifications are 
  //sent when a mouse event occurs in the bounding 
  //rectangle of the icon, or when the icon is selected 
  //or activated with the keyboard. The wParam parameter 
  //of the message contains the identifier of the taskbar 
  //icon in which the event occurred. The lParam parameter 
  //holds the mouse or keyboard message associated with the
  // event. For example, when the pointer moves over a 
  //taskbar icon, lParam is set to WM_MOUSEMOVE. 

  #ifndef GCL_HICON
  #define GCL_HICON (-14)
  #endif // GCL_HICON
  HICON hIcon = m_pParent->GetIcon(FALSE);
  hIcon = hIcon == nullptr ? HICON(GetClassLongPtr(m_pParent->GetSafeHwnd(), GCL_HICON)) : hIcon;
  m_Tray.hIcon = hIcon;
  //Handle to the icon to be added, modified, or deleted

  TCHAR trayName[] = _T("System Tray");
  lstrcpyn(m_Tray.szTip, trayName, lstrlen(trayName));
  //Pointer to a null-terminated string with the text 
  //for a standard ToolTip. It can have a maximum of 64 
  //characters including the terminating NULL. 

  m_Tray.uFlags = NIF_ICON | NIF_MESSAGE;
  //Flags that indicate which of the other members contain 
  //valid data.

  BOOL success = m_Menu.LoadMenu(ID);
  if (!success)
  {
    return __LINE__;
  }

  success = Shell_NotifyIcon(NIM_ADD, &m_Tray);
  if (!success)
  {
    return __LINE__;
  }

  return 0;
}

LRESULT TrayNotify::OnNotification(WPARAM wParam, LPARAM lParam)
{
  switch (LOWORD(lParam))
  {
  case WM_LBUTTONDBLCLK:
    {
      m_pParent->ShowWindow(SW_SHOW);
    }
    break;

  case WM_RBUTTONDOWN:
    {
      CPoint point;
      GetCursorPos(&point);
      m_Menu.GetSubMenu(0)->TrackPopupMenu(
        TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, point.x, point.y, m_pParent);
    }
    break;

  default:
    break;
  }

  return TRUE;
}
