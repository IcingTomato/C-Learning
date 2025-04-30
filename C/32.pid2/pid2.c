#include "../shit.h"

// 定义不同的系统模拟类型
typedef enum 
{
    SYSTEM_LINEAR,         // 线性系统
    SYSTEM_NONLINEAR,      // 非线性系统
    SYSTEM_OSCILLATORY,    // 振荡系统
    SYSTEM_DELAYED         // 延迟系统
} SystemType;

// PID控制器结构体
typedef struct 
{
    float kp;           // 比例系数
    float ki;           // 积分系数
    float kd;           // 微分系数
    
    float target;       // 目标值
    
    float error_sum;    // 误差累积（用于积分项）
    float last_error;   // 上一次的误差（用于微分项）
    float last_deriv;   // 上次的微分值（用于微分滤波）
    
    float output_max;   // 输出上限
    float output_min;   // 输出下限
    float integral_max; // 积分项上限
    float integral_min; // 积分项下限
    
    int initialized;    // 是否已初始化（第一次计算时没有last_error）
    float deriv_filter; // 微分项滤波系数 (0-1)
} PID_Controller;

// 随机数生成增强函数 - 组合多种种子源
unsigned int enhanced_seed_generator() 
{
    unsigned int seed1 = (unsigned int)time(NULL);          // 时间种子
    unsigned int seed2 = (unsigned int)clock();             // CPU时钟计数
    unsigned int seed3 = 0;
    
    // 尝试从文件读取随机字节
    FILE *random_source = fopen("/dev/urandom", "rb");
    if (random_source) 
    {
        fread(&seed3, sizeof(unsigned int), 1, random_source);
        fclose(random_source);
    }
    
    // 如果读取失败，使用简单的混合方法
    if (seed3 == 0) 
    {
        seed3 = seed1 ^ (seed2 << 7) ^ (seed2 >> 3);
    }
    
    return seed1 ^ (seed2 << 11) ^ (seed3 << 17);
}

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
    pid->last_deriv = 0.0f;
    
    pid->output_max = output_max;
    pid->output_min = output_min;
    pid->integral_max = integral_max;
    pid->integral_min = integral_min;
    
    pid->initialized = 0;
    pid->deriv_filter = 0.2f; // 默认微分滤波系数，0.2表示当前微分占80%，上次微分占20%
}

// 重置PID控制器
void PID_Reset(PID_Controller *pid) 
{
    pid->error_sum = 0.0f;
    pid->last_error = 0.0f;
    pid->last_deriv = 0.0f;
    pid->initialized = 0;
}

// 计算PID输出值（增强版，带微分滤波和抗积分饱和）
float PID_Calculate(PID_Controller *pid, float current_value, float dt) 
{
    // 计算误差
    float error = pid->target - current_value;
    
    // 比例项
    float p_term = pid->kp * error;
    
    // 积分项（带抗积分饱和）
    // 只有当输出未饱和时才进行积分
    float output_test = p_term + pid->error_sum * pid->ki;
    if (!((output_test > pid->output_max && error > 0) || 
          (output_test < pid->output_min && error < 0))) 
    {
        pid->error_sum += error * dt;
    }
    
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
    
    // 微分项（带低通滤波）
    float d_term = 0.0f;
    if (pid->initialized) 
    {
        float deriv = (error - pid->last_error) / dt;
        // 应用低通滤波
        float filtered_deriv = (1.0f - pid->deriv_filter) * deriv + pid->deriv_filter * pid->last_deriv;
        d_term = pid->kd * filtered_deriv;
        pid->last_deriv = filtered_deriv;
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

// 模拟不同类型的系统响应
float simulate_system_response(SystemType type, float current_value, float control_input, float dt) 
{
    float new_value = current_value;
    
    switch (type) 
    {
        case SYSTEM_LINEAR:
            // 简单线性系统: dx/dt = k*u
            new_value += control_input * 0.01f;
            break;
            
        case SYSTEM_NONLINEAR:
            // 非线性系统: dx/dt = k*u/(1+x^2)
            new_value += control_input * 0.01f / (1 + current_value * current_value * 0.01f);
            break;
            
        case SYSTEM_OSCILLATORY:
            // 振荡系统: dx/dt = k*u - a*sin(x)
            new_value += control_input * 0.01f - 0.05f * sin(current_value * 0.1f);
            break;
            
        case SYSTEM_DELAYED:
            // 延迟系统 (简化模拟): dx/dt = k*u，但反应较慢
            new_value += control_input * 0.005f;
            break;
            
        default:
            new_value += control_input * 0.01f;
    }
    
    return new_value;
}

// 生成目标值轨迹
float generate_target(int mode, int step, float prev_target) 
{
    static int period_counter = 0;
    float new_target = prev_target;
    
    switch (mode % 5) 
    {
        case 0: // 随机目标
            if (step % 50 == 0 && step > 0) 
            {
                new_target = (float)(rand() % 200);
            }
            break;
            
        case 1: // 阶跃信号
            if (step % 100 == 0) 
            {
                period_counter = (period_counter + 1) % 4;
                switch (period_counter) {
                    case 0: new_target = 50.0f; break;
                    case 1: new_target = 100.0f; break;
                    case 2: new_target = 150.0f; break;
                    case 3: new_target = 75.0f; break;
                }
            }
            break;
            
        case 2: // 正弦波
            new_target = 100.0f + 50.0f * sin(step * 0.01f);
            break;
            
        case 3: // 三角波
            {
                float tri_period = 200.0f;
                float phase = fmod(step, tri_period) / tri_period;
                if (phase < 0.5f) 
                {
                    new_target = 50.0f + 100.0f * (phase * 2.0f);
                } 
                else 
                {
                    new_target = 150.0f - 100.0f * ((phase - 0.5f) * 2.0f);
                }
            }
            break;
            
        case 4: // 方波
            if ((step / 100) % 2 == 0) 
            {
                new_target = 50.0f;
            } 
            else 
            {
                new_target = 150.0f;
            }
            break;
    }
    
    return new_target;
}

// 示例/测试函数
int main() 
{
    // 使用增强的随机数种子生成器
    unsigned int seed = enhanced_seed_generator();
    srand(seed);
    printf("Using random seed: %u\n", seed);
    
    // 创建并初始化PID控制器
    PID_Controller pid;
    
    // 参数：kp, ki, kd, target, output_min, output_max, integral_min, integral_max
    PID_Init(&pid, 2.0f, 0.5f, 0.25f, 100.0f, -1000.0f, 1000.0f, -100.0f, 100.0f);
    
    // 模拟系统变量
    float current_value = 0.0f;
    float control_output = 0.0f;
    float dt = 0.1f; // 时间步长100ms
    int iteration = 0;
    
    // 系统类型和目标模式（随机选择）
    SystemType system_type = rand() % 4;
    int target_mode = rand() % 5;
    
    const char* system_names[] = {"Linear", "Nonlinear", "Oscillatory", "Delayed"};
    const char* target_names[] = {"Random", "Step", "Sine", "Triangle", "Square"};
    
    printf("System type: %s\n", system_names[system_type]);
    printf("Target mode: %s\n", target_names[target_mode]);
    printf("\nTime\tTarget\tCurrent\tOutput\tP-Term\tI-Term\tD-Term\n");
    
    // 主循环
    while (1) 
    {
        // 每1000次迭代更换新的随机种子和系统/目标模式
        if (iteration % 1000 == 0 && iteration > 0) {
            seed = enhanced_seed_generator();
            srand(seed);
            system_type = rand() % 4;
            target_mode = rand() % 5;
            printf("\n--- New simulation ---\n");
            printf("Seed: %u, System: %s, Target: %s\n", 
                  seed, system_names[system_type], target_names[target_mode]);
            printf("Time\tTarget\tCurrent\tOutput\tP-Term\tI-Term\tD-Term\n");
        }
        
        // 根据当前模式生成目标值
        float new_target = generate_target(target_mode, iteration, pid.target);
        if (new_target != pid.target) 
        {
            PID_SetTarget(&pid, new_target);
            printf("--- Target changed to %.1f ---\n", new_target);
        }
        
        // 计算控制量
        control_output = PID_Calculate(&pid, current_value, dt);
        
        // 模拟系统响应
        current_value = simulate_system_response(system_type, current_value, control_output, dt);
        
        // 计算各分项用于打印
        float error = pid.target - current_value;
        float p_term = pid.kp * error;
        float i_term = pid.ki * pid.error_sum;
        float d_term = 0;
        if (pid.initialized) 
        {
            d_term = pid.kd * pid.last_deriv;
        }
        
        // 输出当前状态
        printf("%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\t%.1f\n", 
               iteration * dt, pid.target, current_value, control_output, p_term, i_term, d_term);
        
        iteration++;
        
        // 添加延迟
        #ifdef _WIN32
            Sleep(100); // Windows
        #else
            usleep(100000); // Linux/Unix (100毫秒)
        #endif
    }
    
    return 0;
}