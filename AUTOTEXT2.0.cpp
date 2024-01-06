#include <iostream>
#include <windows.h>
#include <commctrl.h> // ��Ӵ�ͷ�ļ�����������ؼ���صĳ����ͺ���
#include <string>
#include <vector>
#include <thread> // ��Ӵ�ͷ�ļ���ʹ���߳�

// ���廰������
std::vector<std::string> phrases;

// ���ڱ���
const std::string windowTitle = "AUTOTEXT MADE BY FireHY";

// ȫ�ֱ�־����ʾ�Ƿ��˳�ѭ��
bool shouldExit = false;

// ���ͻ��ﺯ��
void sendPhrase(const std::string& phrase, HWND hwnd)
{
  // �����︴�Ƶ�������
  OpenClipboard(NULL);
  EmptyClipboard();
  HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, phrase.size() + 1);
  if (hg != NULL)
  {
    memcpy(GlobalLock(hg), phrase.c_str(), phrase.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
  }
  CloseClipboard();

  // �ڶԻ�����ճ�����ģ�ⰴ��������
  keybd_event(VK_CONTROL, 0, 0, 0);  // ����Ctrl��
  keybd_event('V', 0, 0, 0);   // ����V��
  keybd_event('V', 0, KEYEVENTF_KEYUP, 0);  // �ɿ�V��
  keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);   // �ɿ�Ctrl��

  // ������Ϣ��ģ�ⰴ��������
  keybd_event(VK_RETURN, 0, 0, 0);   // ���»س���
  keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // �ɿ��س���
}

// ���ڹ��̺���
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
  {
    // �����༭��
    HWND hwndEdit = CreateWindowEx(
      0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
      10, 10, 380, 300,
      hwnd, (HMENU)100, GetModuleHandle(NULL), NULL
    );

    // ������ť
    HWND hwndStartButton = CreateWindowEx(
      0, "BUTTON", "��ʼ", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      10, 320, 180, 30,
      hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
    );

    HWND hwndStopButton = CreateWindowEx(
      0, "BUTTON", "����", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      210, 320, 180, 30,
      hwnd, (HMENU)2, GetModuleHandle(NULL), NULL
    );

    // ��������ؼ�
    HWND hwndSlider = CreateWindowEx(
      0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ,
      10, 360, 380, 30,
      hwnd, (HMENU)3, GetModuleHandle(NULL), NULL
    );
    SendMessage(hwndSlider, TBM_SETRANGE, TRUE, MAKELONG(0, 100));  // ���û���ؼ��ķ�Χ
    SendMessage(hwndSlider, TBM_SETPOS, TRUE, 50);  // ���û���ؼ��ĳ�ʼλ��

    break;
  }
  case WM_COMMAND:
  {
    if (LOWORD(wParam) == 1) // ��ʼ��ť
    {
      // ���ÿ�ʼ��ť�����ý�����ť
      EnableWindow(GetDlgItem(hwnd, 1), FALSE);
      EnableWindow(GetDlgItem(hwnd, 2), TRUE);

      // ��ȡ�༭������
      int textLength = GetWindowTextLength(GetDlgItem(hwnd, 100));
      std::vector<char> buffer(textLength + 1);
      GetWindowText(GetDlgItem(hwnd, 100), buffer.data(), textLength + 1);
      std::string text(buffer.data());

      // ���༭�����ݷָ�Ϊ�������
      std::string delimiter = "\r\n";
      size_t pos = 0;
      std::string token;
      while ((pos = text.find(delimiter)) != std::string::npos) {
        token = text.substr(0, pos);
        phrases.push_back(token);
        text.erase(0, pos + delimiter.length());
      }
      phrases.push_back(text);

      // ��ȡ����ؼ��ĵ�ǰֵ
      int speed = SendMessage(GetDlgItem(hwnd, 3), TBM_GETPOS, 0, 0);

      // ѭ�����ͻ���
      while (true)
      {
        // ���ѡ����
        int index = rand() % phrases.size();
        std::string phrase = phrases[index];

        // ���ͻ�����QQ�Ի���
        HWND hwndQQ = FindWindow(NULL, "QQ");
        if (hwndQQ != NULL)
        {
          sendPhrase(phrase, hwndQQ);
        }

        // ��ʱһ��ʱ�䣬���ݻ���ؼ���ֵ��������ʱʱ��
        int delay = 100 - speed;
        Sleep(delay);

        // ����Ƿ����˽�����ť
        if (!IsWindowEnabled(GetDlgItem(hwnd, 2))) // ������ť
        {
          shouldExit = true;
          break;
        }

        // ��ѭ����ÿ�ε���֮�����һ��С����ʱ���ͷ�CPU��Դ
        Sleep(10);
      }

      // ��ջ�������
      phrases.clear();

      // ���ÿ�ʼ��ť�����ý�����ť
      EnableWindow(GetDlgItem(hwnd, 1), TRUE);
      EnableWindow(GetDlgItem(hwnd, 2), FALSE);
    }
    else if (LOWORD(wParam) == 2) // ������ť
    {
      // ���ÿ�ʼ��ť�����ý�����ť
      EnableWindow(GetDlgItem(hwnd, 1), TRUE);
      EnableWindow(GetDlgItem(hwnd, 2), FALSE);
      shouldExit = true;
    }
    break;
  }
  case WM_CLOSE:
  {
    DestroyWindow(hwnd);
    break;
  }
  case WM_DESTROY:
  {
    PostQuitMessage(0);
    break;
  }
  default:
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  return 0;
}

// �̺߳��������ڴ�����Ϣ
void MessageThread(HWND hwnd)
{
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

// ������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // ע�ᴰ����
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(icex);
  icex.dwICC = ICC_BAR_CLASSES;
  InitCommonControlsEx(&icex);

  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = "MyWindowClass";
  RegisterClass(&wc);

  // ��������
  HWND hwnd = CreateWindowEx(
    0, "MyWindowClass", windowTitle.c_str(), WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
    NULL, NULL, hInstance, NULL
  );

  // ��ʾ����
  ShowWindow(hwnd, nCmdShow);

  // �����̣߳���������Ϣѭ��
  std::thread thread(MessageThread, hwnd);
  thread.detach(); // �����̣߳�ʹ���ں�̨����

  // ��Ϣѭ��
  MSG msg;
  while (true)
  {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      // �˳�ѭ��������
      if (msg.message == WM_QUIT || shouldExit)
      {
        break;
      }
    }

    // �˳�ѭ��������
    if (msg.message == WM_QUIT || shouldExit)
    {
      break;
    }

    // �ڴ˴����Դ�����������

    // ��ѭ����ÿ�ε���֮�����һ��С����ʱ���ͷ�CPU��Դ
    Sleep(10);
  }

  return 0;
}

