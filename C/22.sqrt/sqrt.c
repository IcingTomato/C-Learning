#include "../shit.h"

// 长除法求平方根

long long sqrt_classic(long long n) 
{
    // 如果输入是0，直接返回0
    if (n == 0) return 0;
    
    long long root = 0;  // 结果平方根
    long long rem = 0;   // 余数
    
    // 将数字按每两位分组（从左到右）
    int digits[40];      // 存储每组两位数字
    int groups = 0;      // 组数
    
    // 将n分解为两位数字组
    long long temp = n;
    while (temp > 0) {
        digits[groups++] = temp % 100;
        temp /= 100;
    }
    
    // 从高位组开始处理（逆序）
    for (int i = groups - 1; i >= 0; i--) {
        // 将当前组添加到余数
        rem = rem * 100 + digits[i];
        
        // 查找一个商x，使得 (20*root + x)*x <= rem
        long long x = 1;
        while ((20 * root + x) * x <= rem) {
            x++;
        }
        x--; // 减去多试的一次
        
        // 更新余数
        rem -= (20 * root + x) * x;
        
        // 更新平方根结果
        root = root * 10 + x;
    }
    
    return root;
}

int main(void) 
{
    long long num;
    printf("Enter a number to find its square root: ");
    scanf("%lld", &num);

    if (num < 0) 
    {
        printf("Cannot compute square root of a negative number!\n");
    } 
    else 
    {
        long long result = sqrt_classic(num);
        printf("Square root of %lld is approximately %lld\n", num, result);
    }

    return 0;
}