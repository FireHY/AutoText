//AUTOTEXT自动文本v1.0 by FireHY 
#include <iostream>
#include <windows.h>

// 定义话语数组
const std::string phrases[] = {
  "还钱!",
  "还钱!",
  "还钱!",
  // 其他话语...
};

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
  keybd_event('V', 0, 0, 0);     // 按下V键
  keybd_event('V', 0, KEYEVENTF_KEYUP, 0);  // 松开V键
  keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);   // 松开Ctrl键

  // 发送消息（模拟按键操作）
  keybd_event(VK_RETURN, 0, 0, 0);   // 按下回车键
  keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // 松开回车键
}

int main()
{
  // 获取QQ对话框句柄（假设已经打开）
  HWND hwndQQ = FindWindow(NULL, "QQ");

  // 检查禁言限制并绕过
  // ...

  // 选择发送方式（重复发送或随机发送）
  bool repeatSend = true;
  bool randomSend = false;

  // 循环发送话语
  while (true)
  {
    // 判断是否重复发送
    if (!repeatSend)
      break;

    // 随机选择话语
    std::string phrase;
    if (randomSend)
    {
      int index = rand() % (sizeof(phrases) / sizeof(phrases[0]));
      phrase = phrases[index];
    }
    else
    {
      phrase = phrases[0];
    }

    // 发送话语至QQ对话框
    sendPhrase(phrase, hwndQQ);

    // 延时一段时间，避免发送太快
    Sleep(100);
  }

  // 关闭QQ对话框或释放句柄
  // ...

  return 0;
}
