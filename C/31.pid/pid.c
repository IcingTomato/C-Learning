#include "../shit.h"

// PID控制器结构体
typedef struct 
{
    float kp;           // 比例系数
    float ki;           // 积分系数
    float kd;           // 微分系数
    
    float target;       // 目标值
    
    float error_sum;    // 误差累积（用于积分项）
    float last_error;   // 上一次的误差（用于微分项）
    
    float output_max;   // 输出上限
    float output_min;   // 输出下限
    float integral_max; // 积分项上限
    float integral_min; // 积分项下限
    
    int initialized;    // 是否已初始化（第一次计算时没有last_error）
} PID_Controller;

// 初始化PID控制器
void PID_Init(PID_Controller *pid, float kp, float ki, float kd, 
              float target, float output_min, float output_max,
              float integral_min, float integral_max) 
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->target = target;
    
    pid->error_sum = 0.0f;
    pid->last_error = 0.0f;
    
    pid->output_max = output_max;
    pid->output_min = output_min;
    pid->integral_max = integral_max;
    pid->integral_min = integral_min;
    
    pid->initialized = 0;
}

// 重置PID控制器
void PID_Reset(PID_Controller *pid) 
{
    pid->error_sum = 0.0f;
    pid->last_error = 0.0f;
    pid->initialized = 0;
}

// 计算PID输出值
float PID_Calculate(PID_Controller *pid, float current_value, float dt) 
{
    // 计算误差
    float error = pid->target - current_value;
    
    // 比例项
    float p_term = pid->kp * error;
    
    // 积分项
    pid->error_sum += error * dt;
    
    // 积分限幅
    if (pid->error_sum > pid->integral_max) 
    {
        pid->error_sum = pid->integral_max;
    } 
    else if (pid->error_sum < pid->integral_min) 
    {
        pid->error_sum = pid->integral_min;
    }
    
    float i_term = pid->ki * pid->error_sum;
    
    // 微分项
    float d_term = 0.0f;
    if (pid->initialized) {
        d_term = pid->kd * ((error - pid->last_error) / dt);
    }
    
    // 更新last_error和initialized
    pid->last_error = error;
    pid->initialized = 1;
    
    // 计算总输出
    float output = p_term + i_term + d_term;
    
    // 输出限幅
    if (output > pid->output_max) 
    {
        output = pid->output_max;
    } 
    else if (output < pid->output_min) 
    {
        output = pid->output_min;
    }
    
    return output;
}

// 设置新的目标值
void PID_SetTarget(PID_Controller *pid, float new_target) 
{
    pid->target = new_target;
}

// 示例/测试函数
int main() 
{
    // 使用当前时间作为随机数种子
    srand((unsigned int)time(NULL));
    
    // 创建并初始化PID控制器
    PID_Controller pid;
    
    // 参数：kp, ki, kd, target, output_min, output_max, integral_min, integral_max
    PID_Init(&pid, 2.0f, 0.5f, 0.25f, 100.0f, -1000.0f, 1000.0f, -100.0f, 100.0f);
    
    // 模拟系统变量
    float current_value = 0.0f;
    float control_output = 0.0f;
    float dt = 0.1f; // 时间步长100ms
    int iteration = 0;
    
    // 简单模拟 - 修改为无限循环
    printf("Time\tTarget\tCurrent\tOutput\tP-Term\tI-Term\tD-Term\n");
    
    while (1) 
    { // 无限循环
        // 计算控制量
        control_output = PID_Calculate(&pid, current_value, dt);
        
        // 模拟系统响应（简化模型）
        current_value += control_output * 0.01f;
        
        // 计算各分项用于打印（非必要，仅展示）
        float error = pid.target - current_value;
        float p_term = pid.kp * error;
        float i_term = pid.ki * pid.error_sum;
        float d_term = pid.kd * ((error - pid.last_error) / dt);
        
        // 输出当前状态
        printf("%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\n", 
               iteration * dt, pid.target, current_value, control_output, p_term, i_term, d_term);
        
        // 每50次迭代随机更改目标值
        if (iteration % 50 == 0 && iteration > 0) 
        {
            float new_target = (float)(rand() % 200); // 0-199范围内的随机目标值
            PID_SetTarget(&pid, new_target);
            printf("--- Target changed to %.1f ---\n", new_target);
        }
        
        iteration++;
        
        // 添加一点延迟，防止输出太快
        #ifdef _WIN32
            Sleep(100); // Windows
        #else
            usleep(100000); // Linux/Unix (100毫秒)
        #endif
    }
    
    return 0;
}