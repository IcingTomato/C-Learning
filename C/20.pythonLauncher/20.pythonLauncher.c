#include "../shit.h"

void setConsoleWhiteBackground() {
    // 获取控制台句柄
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 设置背景为白色，前景为黑色
    SetConsoleTextAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);

    // 填充整个屏幕为白色
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    DWORD charsWritten;
    COORD topLeft = {0, 0};
    FillConsoleOutputAttribute(hConsole, BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE, consoleSize, topLeft, &charsWritten);
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, topLeft, &charsWritten);

    // 将光标移动到左上角
    SetConsoleCursorPosition(hConsole, topLeft);
}

int main() {
    // 设置控制台为全黑
    setConsoleWhiteBackground();

    // 等待 3 秒
    Sleep(3000);

    // 恢复默认控制台颜色并清屏
    system("cls");
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    // 显示提示信息并进入 Python 控制台
    printf("Connecting Python Console...\n");
    Sleep(3000);
    system("cls");
    system("python");

    return 0;
}