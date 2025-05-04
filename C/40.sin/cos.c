#include <SDL2/SDL.h>
#include <math.h>
#include <stdbool.h>

/*
gcc cos.c -o cos -lmingw32 -lSDL2main -lSDL2 -mwindows
*/

int main(int argc, char* argv[]) 
{
    // 初始化SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
    {
        printf("SDL初始化失败: %s\n", SDL_GetError());
        return 1;
    }
    
    int width = 800;
    int height = 600;
    
    // 创建窗口和渲染器
    SDL_Window* window = SDL_CreateWindow("余弦函数图像", 
                                         SDL_WINDOWPOS_UNDEFINED, 
                                         SDL_WINDOWPOS_UNDEFINED, 
                                         width, height, 
                                         SDL_WINDOW_SHOWN);
    if (!window) 
    {
        printf("窗口创建失败: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) 
    {
        printf("渲染器创建失败: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    int originX = width / 2;   // X轴原点
    int originY = height / 2;  // Y轴原点
    double scale = 50.0;       // 缩放系数，表示每个数学单位占多少像素
    
    bool quit = false;
    SDL_Event event;
    
    while (!quit) 
    {
        // 处理事件
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                quit = true;
            } 
            else if (event.type == SDL_KEYDOWN) 
            {
                // 按+键放大，按-键缩小
                if (event.key.keysym.sym == SDLK_PLUS || event.key.keysym.sym == SDLK_KP_PLUS) 
                {
                    scale *= 1.1;
                } 
                else if (event.key.keysym.sym == SDLK_MINUS || event.key.keysym.sym == SDLK_KP_MINUS) 
                {
                    scale /= 1.1;
                }
            }
        }
        
        // 清屏（白色背景）
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        
        // 绘制网格线
        SDL_SetRenderDrawColor(renderer, 220, 220, 220, 255);
        
        // 水平网格线 (每π/2单位)
        double gridStep = M_PI / 2.0;
        for (double grid = 0; grid < 10; grid += gridStep) 
        {
            int gridPix = (int)(grid * scale);
            SDL_RenderDrawLine(renderer, 0, originY - gridPix, width, originY - gridPix);
            SDL_RenderDrawLine(renderer, 0, originY + gridPix, width, originY + gridPix);
        }
        
        // 垂直网格线
        for (double grid = 0; grid < 20; grid += gridStep) 
        {
            int gridPix = (int)(grid * scale);
            SDL_RenderDrawLine(renderer, originX - gridPix, 0, originX - gridPix, height);
            SDL_RenderDrawLine(renderer, originX + gridPix, 0, originX + gridPix, height);
        }
        
        // 绘制坐标轴
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 0, originY, width, originY);  // X轴
        SDL_RenderDrawLine(renderer, originX, 0, originX, height); // Y轴
        
        // 绘制余弦曲线
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        
        int prevY = 0;
        bool firstPoint = true;
        
        for (int x = 0; x < width; x++) 
        {
            // 将屏幕坐标转换为数学坐标
            double angle = (x - originX) / scale;
            
            // 计算余弦值
            double cosValue = cos(angle);
            
            // 将余弦值转换为屏幕坐标
            int y = originY - (int)(cosValue * scale);
            
            // 绘制连续曲线
            if (firstPoint) {
                firstPoint = false;
                prevY = y;
            } else if (y >= 0 && y < height) {
                SDL_RenderDrawLine(renderer, x-1, prevY, x, y);
                prevY = y;
            }
        }
        
        // 绘制坐标刻度
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        
        // X轴刻度（每π/2单位）
        for (double tick = -10.0; tick <= 10.0; tick += gridStep) 
        {
            int xPos = originX + (int)(tick * scale);
            if (xPos >= 0 && xPos < width) 
            {
                SDL_RenderDrawLine(renderer, xPos, originY - 5, xPos, originY + 5);
            }
        }
        
        // Y轴刻度（每1单位）
        for (int tick = -5; tick <= 5; tick++) 
        {
            if (tick == 0) continue; // 跳过原点
            int yPos = originY - (int)(tick * scale);
            if (yPos >= 0 && yPos < height) 
            {
                SDL_RenderDrawLine(renderer, originX - 5, yPos, originX + 5, yPos);
            }
        }
        
        // 更新屏幕
        SDL_RenderPresent(renderer);
        
        // 帧率控制
        SDL_Delay(16);  // 约60FPS
    }
    
    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}