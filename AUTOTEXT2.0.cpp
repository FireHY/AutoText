#include <iostream>
#include <windows.h>
#include <commctrl.h> // 添加此头文件以声明滑块控件相关的常量和函数
#include <string>
#include <vector>
#include <thread> // 添加此头文件以使用线程

// 定义话语数组
std::vector<std::string> phrases;

// 窗口标题
const std::string windowTitle = "AUTOTEXT MADE BY FireHY";

// 全局标志，表示是否退出循环
bool shouldExit = false;

// 发送话语函数
void sendPhrase(const std::string& phrase, HWND hwnd)
{
  // 将话语复制到剪贴板
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

  // 在对话框中粘贴话语（模拟按键操作）
  keybd_event(VK_CONTROL, 0, 0, 0);  // 按下Ctrl键
  keybd_event('V', 0, 0, 0);   // 按下V键
  keybd_event('V', 0, KEYEVENTF_KEYUP, 0);  // 松开V键
  keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);   // 松开Ctrl键

  // 发送消息（模拟按键操作）
  keybd_event(VK_RETURN, 0, 0, 0);   // 按下回车键
  keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // 松开回车键
}

// 窗口过程函数
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case WM_CREATE:
  {
    // 创建编辑框
    HWND hwndEdit = CreateWindowEx(
      0, "EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
      10, 10, 380, 300,
      hwnd, (HMENU)100, GetModuleHandle(NULL), NULL
    );

    // 创建按钮
    HWND hwndStartButton = CreateWindowEx(
      0, "BUTTON", "开始", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      10, 320, 180, 30,
      hwnd, (HMENU)1, GetModuleHandle(NULL), NULL
    );

    HWND hwndStopButton = CreateWindowEx(
      0, "BUTTON", "结束", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
      210, 320, 180, 30,
      hwnd, (HMENU)2, GetModuleHandle(NULL), NULL
    );

    // 创建滑块控件
    HWND hwndSlider = CreateWindowEx(
      0, TRACKBAR_CLASS, NULL, WS_CHILD | WS_VISIBLE | TBS_HORZ,
      10, 360, 380, 30,
      hwnd, (HMENU)3, GetModuleHandle(NULL), NULL
    );
    SendMessage(hwndSlider, TBM_SETRANGE, TRUE, MAKELONG(0, 100));  // 设置滑块控件的范围
    SendMessage(hwndSlider, TBM_SETPOS, TRUE, 50);  // 设置滑块控件的初始位置

    break;
  }
  case WM_COMMAND:
  {
    if (LOWORD(wParam) == 1) // 开始按钮
    {
      // 禁用开始按钮，启用结束按钮
      EnableWindow(GetDlgItem(hwnd, 1), FALSE);
      EnableWindow(GetDlgItem(hwnd, 2), TRUE);

      // 获取编辑框内容
      int textLength = GetWindowTextLength(GetDlgItem(hwnd, 100));
      std::vector<char> buffer(textLength + 1);
      GetWindowText(GetDlgItem(hwnd, 100), buffer.data(), textLength + 1);
      std::string text(buffer.data());

      // 将编辑框内容分割为多个话语
      std::string delimiter = "\r\n";
      size_t pos = 0;
      std::string token;
      while ((pos = text.find(delimiter)) != std::string::npos) {
        token = text.substr(0, pos);
        phrases.push_back(token);
        text.erase(0, pos + delimiter.length());
      }
      phrases.push_back(text);

      // 获取滑块控件的当前值
      int speed = SendMessage(GetDlgItem(hwnd, 3), TBM_GETPOS, 0, 0);

      // 循环发送话语
      while (true)
      {
        // 随机选择话语
        int index = rand() % phrases.size();
        std::string phrase = phrases[index];

        // 发送话语至QQ对话框
        HWND hwndQQ = FindWindow(NULL, "QQ");
        if (hwndQQ != NULL)
        {
          sendPhrase(phrase, hwndQQ);
        }

        // 延时一段时间，根据滑块控件的值来决定延时时长
        int delay = 100 - speed;
        Sleep(delay);

        // 检查是否点击了结束按钮
        if (!IsWindowEnabled(GetDlgItem(hwnd, 2))) // 结束按钮
        {
          shouldExit = true;
          break;
        }

        // 在循环的每次迭代之后，添加一个小的延时，释放CPU资源
        Sleep(10);
      }

      // 清空话语数组
      phrases.clear();

      // 启用开始按钮，禁用结束按钮
      EnableWindow(GetDlgItem(hwnd, 1), TRUE);
      EnableWindow(GetDlgItem(hwnd, 2), FALSE);
    }
    else if (LOWORD(wParam) == 2) // 结束按钮
    {
      // 启用开始按钮，禁用结束按钮
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

// 线程函数，用于处理消息
void MessageThread(HWND hwnd)
{
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  // 注册窗口类
  INITCOMMONCONTROLSEX icex;
  icex.dwSize = sizeof(icex);
  icex.dwICC = ICC_BAR_CLASSES;
  InitCommonControlsEx(&icex);

  WNDCLASS wc = {};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = "MyWindowClass";
  RegisterClass(&wc);

  // 创建窗口
  HWND hwnd = CreateWindowEx(
    0, "MyWindowClass", windowTitle.c_str(), WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
    NULL, NULL, hInstance, NULL
  );

  // 显示窗口
  ShowWindow(hwnd, nCmdShow);

  // 创建线程，并启动消息循环
  std::thread thread(MessageThread, hwnd);
  thread.detach(); // 分离线程，使其在后台运行

  // 消息循环
  MSG msg;
  while (true)
  {
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      // 退出循环的条件
      if (msg.message == WM_QUIT || shouldExit)
      {
        break;
      }
    }

    // 退出循环的条件
    if (msg.message == WM_QUIT || shouldExit)
    {
      break;
    }

    // 在此处可以处理其他任务

    // 在循环的每次迭代之后，添加一个小的延时，释放CPU资源
    Sleep(10);
  }

  return 0;
}

