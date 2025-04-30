#include "../shit.h"

int main() {
    // 弹出警告窗口
    // 参数1：父窗口句柄，NULL表示无父窗口
    // 参数2：窗口显示的消息内容
    // 参数3：窗口标题
    // 参数4：窗口类型标志
    //        MB_ICONWARNING - 显示警告图标（黄色感叹号）
    //        MB_OK - 只显示一个"确定"按钮
    MessageBox(
        NULL,
        "这是一个警告消息！\n请注意程序可能存在异常情况。",
        "警告",
        MB_ICONWARNING | MB_OK
    );

    return 0;
}