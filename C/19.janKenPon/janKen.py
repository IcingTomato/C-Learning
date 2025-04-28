# Rock-Paper-Scissors game with two machines playing against each other
import random

def generate_choice():
    # Generate a random number between 1-3, representing scissors(1), stone(2), paper(3)
    return random.randint(1, 3)

def display_choice(choice, machine_num):
    print("Machine {} chooses: ".format(machine_num), end="")
    if choice == 1:
        print("scissor.")
    elif choice == 2:
        print("stone.")
    elif choice == 3:
        print("paper.")

def determine_winner(machine1, machine2):
    result = machine1 - machine2
    
    if result == 0:
        print("Tie.")
    elif result == -1 or result == 2:
        print("Machine 2 wins!")
    else:
        print("Machine 1 wins!")
    print()

def main():
    round_count = 1
    
    while True:
        print("Round {}".format(round_count))
        
        # Generate choices for both machines
        machine1 = generate_choice()
        machine2 = generate_choice()
        
        # Display choices
        display_choice(machine1, 1)
        display_choice(machine2, 2)
        
        # Determine the winner
        determine_winner(machine1, machine2)
        
        round_count += 1
        
        # Optional: press a key to continue or set a round limit
        # To stop after a specific number of rounds, add the condition: if round_count > 10: break

if __name__ == "__main__":
    main()