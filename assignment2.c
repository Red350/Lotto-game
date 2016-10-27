/* Lotto game where a player can input 6 unique numbers in the range 1-42.
 * The player has the option to print, sort or check their numbers. 
 * They can also input new numbers at any point, or exit the program.
 * 
 * OS: Linux Mint 17.2 Rafaela
 * Compiled using GCC version 4.84
 * Author: Pádraig Redmond C15755659
 * Last updated: 04/03/2016
 */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 6
#define LOTTO_MAX 42

void print_menu();
void flush_buffer();
void input_numbers(int*, int);
int validate_numbers(int*, int);
void print_numbers(int*, int);
void quick_sort(int*, int, int);
void update_frequency(int*, int*, int);
void print_frequency(int*, int);
void check_numbers(int*, int*, int);

int main()
{
	int menu_option;
	int i;
	int nums_entered = 0;	// used to check if user has inputted numbers
	int player_numbers[SIZE];

	// 6 winning numbers, last number is the bonus
	int winning_numbers[SIZE+1] = { 1, 3, 5, 7, 9, 11, 42 };

	// all frequencies initialised to 0
	int freq[42] = {0};

	// Print introduction to the game
	printf("Welcome to the lotto game!\n");

	// Menu loop
	do
	{
		// Print the menu
		print_menu();
		
		// Take input from user and flush the buffer
		scanf("%d", &menu_option);
		flush_buffer();

		// Check if menu option is valid
		// if not, ask user for input again
		while (menu_option < 1 || menu_option > 6)
		{
			printf("ERROR: Please select a valid menu option: ");
			scanf("%d", &menu_option);
			flush_buffer();
		}
		
		printf("\n");

		// perform the action that the user has selected
		switch (menu_option)
		{
			// input lotto numbers into array
			case 1:
				printf("Please enter your 6 unique numbers (must be in the range 1-42)\n");
				input_numbers(player_numbers, SIZE);
				printf("Numbers saved.\n");
				nums_entered = 1;	// Flag to allow the user to use the other menu options
				update_frequency(freq, player_numbers, SIZE);
				break;

			// display lotto numbers stored in array
			case 2:
				if (nums_entered)
				{
					print_numbers(player_numbers, SIZE);
				}
				else
				{
					printf("ERROR: Please enter your numbers first.\n");
				}
				break;

			// sort the lotto numbers in ascending order
			case 3:
				if (nums_entered)
				{
					quick_sort(player_numbers, SIZE, 0);
					printf("Numbers sorted.\n");
				}
				else
				{
					printf("ERROR: Please enter your numbers first.\n");
				}
				break; 
			
			// check inputted lotto numbers against winning numberrs
			case 4:
				if (nums_entered)
				{
					check_numbers(player_numbers, winning_numbers, SIZE);
				}
				else
				{
					printf("ERROR: Please enter your numbers first.\n");
				}
				break;
			
			// display the frequency of the numbers inputted
			case 5:
				if (nums_entered)
				{
					print_frequency(freq, LOTTO_MAX);
				}
				else
				{
					printf("ERROR: Please enter your numbers first.\n");
				}
				break;
		}
	}
	while(menu_option != 6);

	printf("Thank you for playing.\n");

	return 0;
}

void print_menu()
{
	printf("\n");
	printf("\t-----------------------------------\n");
	printf("\t| 1. Input numbers                |\n");
	printf("\t| 2. Display numbers              |\n");
	printf("\t| 3. Sort numbers                 |\n");
	printf("\t| 4. Check numbers                |\n");
	printf("\t| 5. Display frequency of numbers |\n");
	printf("\t| 6. Exit Program                 |\n");
	printf("\t-----------------------------------\n");
	printf("\nPlease select an option: ");
}

// Function to store numbers in an array of given size
// returns 1 if numbers were valid, 0 if invalid
//
void input_numbers(int *player_nums, int size)
{
	int i; 

	// is_int is used to ensure the user inputs numbers, and not characters
	int is_int;
	
	// valid_input is used when an integer array has been successfully input.
	// It is set to 1 when the integers are in the correct range
	// and there are no duplicates
	int valid_input = 0;
	
	// take input from the user until they provide valid numbers
	do
	{
		is_int = 1;
		for (i = 0; i < size; i++)
		{
			if ((scanf("%d", player_nums+i)) == 0)
			{
				// if the user inputs a non integer, unset is_int
				// and stop taking user input
				is_int = 0;
				break;
			}
		}
		flush_buffer();
	
		// If at this point only integers have been input, call the validate_numbers
		// function to ensure the numbers are unique and in the range 1-42
		if (is_int == 1)
		{
			valid_input = validate_numbers(player_nums, size);
		}
		
		// if the numbers were not valid, prompt the user to input again
		if(!valid_input)
		{
			printf("ERROR: Those numbers are invalid, please try again.\n");
		}
	}
	while(valid_input == 0);
}

// Function to check if numbers are in the range 1-42
// and are unique. Returns 1 if valid, 0 if invalid.
//
// The reason for seperating this into a seperate function
// was for ease of breaking out of the nested loop upon invalidating
// a set of numbers by using the return statement.
//
int validate_numbers(int *nums, int size)
{
	int i, j;

	// These nested for loops simultaneously check if each number is 
	// in the range 1-42 and that there are no duplicates
	for (i = 0; i < size; i++)
	{
		// check if in range 1-42, if not return 0
		if (*(nums+i) < 1 || *(nums+i) > 42)
		{
			return 0;
		}
		else
		{
			// checks the current number against all the numbers
			// above it in the array for duplicates
			for (j = i + 1; j < size; j++)
			{
				// if a duplicate is found return 0
				if (*(nums+i) == *(nums+j))
				{
					return 0;
				}
			}
		}
	}

	return 1;
}

// Function to print an integer array of given size
//
void print_numbers(int *player_numbers, int size)
{
	int i;
	printf("Your numbers: ");
	for (i = 0; i < size; i++)
	{
		printf("%d ", *(player_numbers+i));
	}
	printf("\n");
}

// Implementation of the quicksort algorithm.
// I chose this because it was a challenge to write, despite it's inefficiency
// for such a small data set
//
// In this implementation, the pivot is always chosen to be the first element of the array or sub-array.
// Check the pivot against opposite end of array until a number smaller than the pivot is found.
// Swap pivot with that number.
// Then check number beside where pivot was until a bigger number is found, and swap again.
// Repeat until pivot == compare, at which point the pivot is sorted.
// Then call the function recursively on the sub-arrays on either side of the pivot.
// 
void quick_sort(int *arr, int size, int start) { 
	int pivot = start;
	int compare = size + start - 1;
	int temp;
	int side = 0;	// determines which side of the pivot to check against
	
	// when compare == pivot the pivot is centred
	while(compare != pivot)
	{
		if (side == 0)
		{
			// checking numbers to the right, until a smaller number is found
			if(*(arr+pivot) > *(arr+compare))
			{
				// if a smaller number is found, swap it with the pivot
				temp = *(arr+pivot);
				*(arr+pivot) = *(arr+compare);
				*(arr+compare) = temp;

				temp = pivot;
				pivot = compare;
				compare = temp + 1;
				
				// start checking numbers to the left on next loop
				side = 1;
			}
			else
			{

				compare--;
			}
		}
		else
		{
			// checking numbers to the left, until a bigger number is found
			if(*(arr+pivot) < *(arr+compare))
			{
				// if a bigger number is found, swap it with the pivot
				temp = *(arr+pivot);
				*(arr+pivot) = *(arr+compare);
				*(arr+compare) = temp;

				temp = pivot;
				pivot = compare;
				compare = temp - 1;

				// start checking numbers to the right on the next loop
				side = 0;
			}
			else
			{
				compare++;
			}
		}
	}
	
	// At this point the pivot is centred. Now the arrays
	// on either side of the pivot have to be sorted

	// sort the left sub-array if it has more than 1 element
	if (pivot - start > 1)
	{
		quick_sort(arr, pivot - start, start);
	}	

	// sort the right sub-array if it has more than 1 element
	if (size + start - pivot - 1 > 1)
	{
		quick_sort(arr, size + start - pivot - 1, pivot + 1);
	}
}

// Function to update the frequency of each number that
// the player has used
//
void update_frequency(int *freq, int *nums, int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		(*(freq + *(nums+i) - 1))++;
	}
}

// Function to print the frequencies of all numbers whose
// frequency is > 0
//
void print_frequency(int *freq, int size)
{
	int i;

	for (i = 0; i < size; i++)
	{
		if (*(freq + i) != 0)
		{
			printf("The number %d has been chosen %d time(s)\n", i+1, *(freq + i));
		}
	}
}

// Function to check a players array of numbers
// against the winning numbers, determining the prize if any
//
void check_numbers(int *player_nums, int *winning_nums, int size)
{
	int i, j;
	int matches = 0;
	int bonus = 0;

	// Check each number in players numbers against every winning number
	for (i = 0; i < size; i++)
	{
		for (j = 0; j < size; j++)
		{
			if (*(player_nums+i) == *(winning_nums+j))
			{
				// If the number is found, increment the matches
				// variable and stop checking that player number
				matches++;
				break;
			}

			// Check bonus.
			// Since size refers to the player nums size
			// and the winning nums array is one bigger,
			// size does not need to be decremented 
			// to get the last element
			if(*(player_nums+i) == *(winning_nums+size))
			{
				bonus = 1;
			}
		}
	}

	// Numbers have been checked and the results stored.
	// Now need to check what prize, if any, has been won
	switch (matches)
	{
		case 6:
			printf("Match 6\n");
			printf("You've won the Jackpot!!!!\n");
			break;

		case 5:
			printf("Match 5");
			if (bonus)
			{
				printf(" + Bonus");
				printf("\nYou've won a new car!\n");
			}
			else
				printf("\nYou've won a holiday!\n");
			break;

		case 4:
			printf("Match 4");
			if (bonus)
			{
				printf(" + Bonus");
				printf("\nYou've won a weekend away!\n");
			}
			else
				printf("\nYou've won a night out!\n");
			break;

		case 3:
			if (bonus)
			{
				printf("Match 3 + Bonus");
				printf("\nYou've won a cinema ticket!\n");
			}
			else
				printf("You've won nothing!\n");
			break;

		default:
			printf("You've won nothing!\n");
			break;
	}

}

// Function to clear the input buffer.
// Prevents '\n' being read as input.
//
void flush_buffer()
{
	char c;
	while((c = getchar()) != '\n')
	{
		// Discarding input
	}
}

