//AUTOTEXT�Զ��ı�v1.0 by FireHY 
#include <iostream>
#include <windows.h>

// ���廰������
const std::string phrases[] = {
  "��Ǯ!",
  "��Ǯ!",
  "��Ǯ!",
  // ��������...
};

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
  keybd_event('V', 0, 0, 0);     // ����V��
  keybd_event('V', 0, KEYEVENTF_KEYUP, 0);  // �ɿ�V��
  keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);   // �ɿ�Ctrl��

  // ������Ϣ��ģ�ⰴ��������
  keybd_event(VK_RETURN, 0, 0, 0);   // ���»س���
  keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // �ɿ��س���
}

int main()
{
  // ��ȡQQ�Ի������������Ѿ��򿪣�
  HWND hwndQQ = FindWindow(NULL, "QQ");

  // ���������Ʋ��ƹ�
  // ...

  // ѡ���ͷ�ʽ���ظ����ͻ�������ͣ�
  bool repeatSend = true;
  bool randomSend = false;

  // ѭ�����ͻ���
  while (true)
  {
    // �ж��Ƿ��ظ�����
    if (!repeatSend)
      break;

    // ���ѡ����
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

    // ���ͻ�����QQ�Ի���
    sendPhrase(phrase, hwndQQ);

    // ��ʱһ��ʱ�䣬���ⷢ��̫��
    Sleep(100);
  }

  // �ر�QQ�Ի�����ͷž��
  // ...

  return 0;
}
