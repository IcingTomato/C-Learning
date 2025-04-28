# Square root calculation using long division method

def sqrt_classic(n):
    # If input is 0, return 0 directly
    if n == 0:
        return 0
    
    root = 0  # Result square root
    rem = 0   # Remainder
    
    # Group the number by two digits (from left to right)
    digits = []  # Store each group of two digits
    
    # Decompose n into groups of two digits
    temp = n
    while temp > 0:
        digits.append(temp % 100)
        temp //= 100
    
    # Process from the highest group (in reverse order)
    for i in range(len(digits) - 1, -1, -1):
        # Add the current group to the remainder
        rem = rem * 100 + digits[i]
        
        # Find a quotient x, such that (20*root + x)*x <= rem
        x = 1
        while (20 * root + x) * x <= rem:
            x += 1
        x -= 1  # Subtract the extra trial
        
        # Update the remainder
        rem -= (20 * root + x) * x
        
        # Update the square root result
        root = root * 10 + x
    
    return root

def main():
    try:
        num = int(input("Enter a number to find its square root: "))
        
        if num < 0:
            print("Cannot compute square root of a negative number!")
        else:
            result = sqrt_classic(num)
            print("Square root of {}".format(num))
            print("is approximately {}".format(result))
    except ValueError:
        print("Please enter a valid integer.")

if __name__ == "__main__":
    main()