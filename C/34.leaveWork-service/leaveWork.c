#include <windows.h>
#include <stdio.h>
#include <time.h>
#include <commctrl.h>
#include <uxtheme.h>

#define SERVICE_NAME "LeaveWorkReminder"
#define DISPLAY_NAME "准时下班提醒服务"
#define DESCRIPTION "在每天18:28警告你准备下班"

#define COUNTDOWN_SECONDS 120 // 倒计时总秒数（2分钟）
#define TIMER_ID 1
#define WM_APP_INIT_COUNTDOWN (WM_APP + 1)

SERVICE_STATUS ServiceStatus;
SERVICE_STATUS_HANDLE hStatus;
HANDLE hServiceStopEvent = NULL;

// 倒计时窗口数据
typedef struct {
    int remainingSeconds;
    HWND hProgressBar;
    HWND hLabel;
} CountdownData;

// 窗口过程函数
LRESULT CALLBACK CountdownWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static CountdownData* pData = NULL;
    static HFONT hFont = NULL;
    static HBRUSH hLabelBrush = NULL; // 添加标签背景画刷

    switch (msg)
    {
        case WM_CREATE:
            // 分配数据结构
            pData = (CountdownData*)malloc(sizeof(CountdownData));
            if (pData)
            {
                pData->remainingSeconds = COUNTDOWN_SECONDS;
                
                // 创建倒计时标签 - 添加 SS_NOTIFY 样式以接收颜色消息
                pData->hLabel = CreateWindow(
                    "STATIC", "", 
                    WS_CHILD | WS_VISIBLE | SS_CENTER | SS_NOTIFY,
                    50, 20, 200, 40, 
                    hwnd, NULL, GetModuleHandle(NULL), NULL
                );

                // 创建进度条 - 使用Windows 7样式
                pData->hProgressBar = CreateWindowEx(
                    0, PROGRESS_CLASS, NULL, 
                    WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                    50, 70, 200, 20, 
                    hwnd, NULL, GetModuleHandle(NULL), NULL
                );
                
                // 设置进度条范围
                SendMessage(pData->hProgressBar, PBM_SETRANGE, 0, MAKELPARAM(0, COUNTDOWN_SECONDS));
                SendMessage(pData->hProgressBar, PBM_SETPOS, COUNTDOWN_SECONDS, 0);
                
                // 设置进度条为Windows 7样式
                SendMessage(pData->hProgressBar, PBM_SETSTATE, PBST_NORMAL, 0);
                
                // 应用Visual Styles到进度条
                SetWindowTheme(pData->hProgressBar, L"", L"");
                
                // 创建大字体
                hFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                                  DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, 
                                  CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
                
                if (hFont) {
                    SendMessage(pData->hLabel, WM_SETFONT, (WPARAM)hFont, TRUE);
                }
                
                // 发送初始化消息
                PostMessage(hwnd, WM_APP_INIT_COUNTDOWN, 0, 0);
                hLabelBrush = CreateSolidBrush(RGB(255, 255, 255));
            }
            break;
            
        case WM_APP_INIT_COUNTDOWN:
            // 开始计时器
            SetTimer(hwnd, TIMER_ID, 1000, NULL);
            
            // 立即更新显示
            if (pData) {
                char text[128];
                sprintf(text, "还有 %d 秒就要润了!", pData->remainingSeconds);
                SetWindowText(pData->hLabel, text);
            }
            break;
            
            case WM_TIMER:
            if (wParam == TIMER_ID && pData)
            {
                // 更新剩余时间
                pData->remainingSeconds--;
                
                // 更新进度条
                SendMessage(pData->hProgressBar, PBM_SETPOS, pData->remainingSeconds, 0);
                
                // 当剩余时间少于1/3时，将进度条设置为黄色警告状态
                if (pData->remainingSeconds <= COUNTDOWN_SECONDS / 3 && 
                    pData->remainingSeconds > COUNTDOWN_SECONDS / 6) {
                    SendMessage(pData->hProgressBar, PBM_SETSTATE, PBST_PAUSED, 0);
                }
                // 当剩余时间少于1/6时，将进度条设置为红色错误状态
                else if (pData->remainingSeconds <= COUNTDOWN_SECONDS / 6) {
                    SendMessage(pData->hProgressBar, PBM_SETSTATE, PBST_ERROR, 0);
                }
                
                // 更新标签文本
                char text[128];
                sprintf(text, "还有 %d 秒就要润了!", pData->remainingSeconds);
                SetWindowText(pData->hLabel, text);
                
                // 检查是否倒计时完成
                if (pData->remainingSeconds <= 0)
                {
                    KillTimer(hwnd, TIMER_ID);
                    MessageBox(hwnd, "开润！\n请立即关闭计算机！", "警告", MB_ICONWARNING | MB_OK);
                    DestroyWindow(hwnd);
                }
            }
            break;
            
        case WM_CTLCOLORSTATIC:
            // 为静态控件(标签)设置自定义颜色
            if ((HWND)lParam == pData->hLabel && hLabelBrush)
            {
                HDC hdcStatic = (HDC)wParam;
                SetTextColor(hdcStatic, RGB(0, 0, 0));    // 黑色文本
                SetBkColor(hdcStatic, RGB(255, 255, 255)); // 白色背景
                return (LRESULT)hLabelBrush;
            }
            break;
            
        case WM_DESTROY:
            if (pData) {
                free(pData);
                pData = NULL;
            }
            if (hFont) {
                DeleteObject(hFont);
                hFont = NULL;
            }
            if (hLabelBrush) {
                DeleteObject(hLabelBrush);
                hLabelBrush = NULL;
            }
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// 显示倒计时窗口的线程函数
DWORD WINAPI ShowReminderThread(LPVOID lpParam)
{
    // 创建一个新的桌面会话以显示GUI
    HDESK hDesk = OpenInputDesktop(0, FALSE, DESKTOP_CREATEWINDOW);
    SetThreadDesktop(hDesk);
    
    // 确保Common Controls DLL已初始化 - 添加更多控件类别
    INITCOMMONCONTROLSEX icc;
    icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
    icc.dwICC = ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES;
    InitCommonControlsEx(&icc);
    
    // 注册窗口类
    WNDCLASSEX wc = {0};
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = CountdownWndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "CountdownWindowClass";
    RegisterClassEx(&wc);
    
    // 创建窗口
    HWND hWnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW,
        "CountdownWindowClass",
        "下班倒计时",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 150,
        NULL, NULL, GetModuleHandle(NULL), NULL
    );
    
    if (hWnd)
    {
        // 将窗口居中显示
        RECT rc;
        GetWindowRect(hWnd, &rc);
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        int windowWidth = rc.right - rc.left;
        int windowHeight = rc.bottom - rc.top;
        SetWindowPos(hWnd, NULL, 
                     (screenWidth - windowWidth) / 2, 
                     (screenHeight - windowHeight) / 2,
                     0, 0, SWP_NOSIZE | SWP_NOZORDER);
        
        // 显示窗口
        ShowWindow(hWnd, SW_SHOW);
        UpdateWindow(hWnd);
        
        // 消息循环
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    
    CloseDesktop(hDesk);
    return 0;
}

// 服务控制处理函数
VOID WINAPI ServiceCtrlHandler(DWORD dwControl)
{
    switch (dwControl)
    {
        case SERVICE_CONTROL_STOP:
        case SERVICE_CONTROL_SHUTDOWN:
            ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            ServiceStatus.dwControlsAccepted = 0;
            SetServiceStatus(hStatus, &ServiceStatus);
            
            // 触发事件以停止主线程
            SetEvent(hServiceStopEvent);
            return;
            
        case SERVICE_CONTROL_INTERROGATE:
            break;
            
        default:
            break;
    }
    
    SetServiceStatus(hStatus, &ServiceStatus);
}

// 检查时间并显示提醒
VOID CheckTimeAndNotify()
{
    static BOOL alreadyNotifiedToday = FALSE;
    static int lastCheckedDay = -1;
    
    time_t now;
    struct tm *local_time;
    
    time(&now);
    local_time = localtime(&now);
    
    // 每天重置通知状态
    if (lastCheckedDay != local_time->tm_mday) {
        lastCheckedDay = local_time->tm_mday;
        alreadyNotifiedToday = FALSE;
    }
    
    // 检查是否是18:28 (或者刚刚过了这个时间)
    if (!alreadyNotifiedToday && 
        local_time->tm_hour == 19 && 
        (local_time->tm_min == 10 || local_time->tm_min == 11)) {
        
        // 设置已通知标志，防止重复提醒
        alreadyNotifiedToday = TRUE;
        
        // 创建线程来显示提醒
        HANDLE hThread = CreateThread(NULL, 0, ShowReminderThread, NULL, 0, NULL);
        if (hThread) {
            CloseHandle(hThread);
        }
        
        // 记录到日志 - 帮助调试
        FILE *logFile = fopen(".\\LeaveWork_log.txt", "a");
        if (logFile) {
            fprintf(logFile, "[%04d-%02d-%02d %02d:%02d:%02d] 已显示下班通知\n", 
                   local_time->tm_year+1900, local_time->tm_mon+1, local_time->tm_mday,
                   local_time->tm_hour, local_time->tm_min, local_time->tm_sec);
            fclose(logFile);
        }
    }
}

// 服务主函数
VOID WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    // 初始化服务状态
    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    ServiceStatus.dwWaitHint = 0;
    
    // 注册服务控制处理函数
    hStatus = RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);
    if (hStatus == 0)
    {
        return;
    }
    
    // 创建服务停止事件
    hServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hServiceStopEvent == NULL)
    {
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode = GetLastError();
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }
    
    // 服务已启动
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(hStatus, &ServiceStatus);
    
    // 主循环 - 每隔30秒检查一次时间
    while (WaitForSingleObject(hServiceStopEvent, 30000) != WAIT_OBJECT_0)
    {
        CheckTimeAndNotify();
    }
    
    // 服务停止
    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    SetServiceStatus(hStatus, &ServiceStatus);
}

// 安装服务
BOOL InstallService()
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if (!hSCManager)
    {
        printf("无法打开服务控制管理器: %d\n", GetLastError());
        printf("八成是没用管理员模式运行\n");
        return FALSE;
    }
    
    char path[MAX_PATH];
    if (!GetModuleFileName(NULL, path, MAX_PATH))
    {
        printf("无法获取程序路径: %d\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return FALSE;
    }
    
    SC_HANDLE hService = CreateService(
        hSCManager,
        SERVICE_NAME,
        DISPLAY_NAME,
        SERVICE_ALL_ACCESS,
        SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // 添加交互式标志
        SERVICE_AUTO_START,
        SERVICE_ERROR_NORMAL,
        path,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    );
    
    if (!hService)
    {
        printf("无法创建服务: %d\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return FALSE;
    }
    
    // 设置服务描述信息
    SERVICE_DESCRIPTION sd;
    sd.lpDescription = DESCRIPTION;
    ChangeServiceConfig2(hService, SERVICE_CONFIG_DESCRIPTION, &sd);
    
    printf("服务安装成功!\n");
    
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return TRUE;
}

// 卸载服务
BOOL UninstallService()
{
    SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (!hSCManager)
    {
        printf("无法打开服务控制管理器: %d\n", GetLastError());
        return FALSE;
    }
    
    SC_HANDLE hService = OpenService(hSCManager, SERVICE_NAME, SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
    if (!hService)
    {
        printf("无法打开服务: %d\n", GetLastError());
        CloseServiceHandle(hSCManager);
        return FALSE;
    }
    
    // 尝试停止服务
    SERVICE_STATUS status;
    ControlService(hService, SERVICE_CONTROL_STOP, &status);
    
    // 删除服务
    if (!DeleteService(hService))
    {
        printf("无法删除服务: %d\n", GetLastError());
        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return FALSE;
    }
    
    printf("服务卸载成功!\n");
    
    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    return TRUE;
}

// 主函数 - 控制台应用程序入口点
int main(int argc, char* argv[])
{
    // 解析命令行参数
    if (argc > 1)
    {
        if (strcmp(argv[1], "-install") == 0)
        {
            return InstallService() ? 0 : 1;
        }
        else if (strcmp(argv[1], "-uninstall") == 0)
        {
            return UninstallService() ? 0 : 1;
        }
        else if (strcmp(argv[1], "-test") == 0)
        {
            // 测试模式 - 直接启动倒计时UI
            HANDLE hThread = CreateThread(NULL, 0, ShowReminderThread, NULL, 0, NULL);
            if (hThread)
            {
                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);
            }
            return 0;
        }
        else if (strcmp(argv[1], "-status") == 0)
        {
            SC_HANDLE hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
            if (hSCManager)
            {
                SC_HANDLE hService = OpenService(hSCManager, SERVICE_NAME, SERVICE_QUERY_STATUS);
                if (hService)
                {
                    SERVICE_STATUS status;
                    if (QueryServiceStatus(hService, &status))
                    {
                        printf("服务状态: ");
                        switch (status.dwCurrentState)
                        {
                            case SERVICE_STOPPED: printf("已停止\n"); break;
                            case SERVICE_START_PENDING: printf("正在启动\n"); break;
                            case SERVICE_STOP_PENDING: printf("正在停止\n"); break;
                            case SERVICE_RUNNING: printf("正在运行\n"); break;
                            case SERVICE_CONTINUE_PENDING: printf("正在继续\n"); break;
                            case SERVICE_PAUSE_PENDING: printf("正在暂停\n"); break;
                            case SERVICE_PAUSED: printf("已暂停\n"); break;
                            default: printf("未知状态(%d)\n", status.dwCurrentState); break;
                        }
                    }
                    else
                    {
                        printf("无法查询服务状态: %d\n", GetLastError());
                    }
                    CloseServiceHandle(hService);
                }
                else
                {
                    printf("无法打开服务: %d\n", GetLastError());
                }
                CloseServiceHandle(hSCManager);
            }
            else
            {
                printf("无法打开服务控制管理器: %d\n", GetLastError());
            }
            return 0;
        }
    }

    // 注册服务入口函数
    SERVICE_TABLE_ENTRY ServiceTable[] = 
    {
        {SERVICE_NAME, (LPSERVICE_MAIN_FUNCTION)ServiceMain},
        {NULL, NULL}
    };
    
    // 启动服务控制分派器
    if (!StartServiceCtrlDispatcher(ServiceTable))
    {
        // 如果不是作为服务运行，则显示使用帮助
        printf("下班提醒服务\n");
        printf("用法:\n");
        printf("  %s -install   : 安装服务\n", argv[0]);
        printf("  %s -uninstall : 卸载服务\n", argv[0]);
        printf("  %s -test      : 测试模式(直接显示倒计时)\n", argv[0]);
        printf("  %s -status    : 查看服务状态\n", argv[0]);
        printf("\n安装后请手动启动服务: sc start %s\n", SERVICE_NAME);
        return 1;
    }
    
    return 0;
}