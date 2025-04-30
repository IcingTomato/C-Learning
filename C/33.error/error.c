#include "../shit.h"

int main(void) 
{
    // 弹出警告窗口
    // 参数1：父窗口句柄，NULL表示无父窗口
    // 参数2：窗口显示的消息内容
    // 参数3：窗口标题
    // 参数4：窗口类型标志
    //        MB_ICONWARNING - 显示警告图标（黄色感叹号）
    //        MB_OK - 只显示一个"确定"按钮
    MessageBox(
        NULL,
        "请立即关闭计算机！",
        "警告",
        MB_ICONWARNING | MB_OK
    );

    return 0;
}