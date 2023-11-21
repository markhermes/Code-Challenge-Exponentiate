/*
 * ExponentObj_test.cpp
 *
 *  Created on: Sep 29, 2023
 *      Author: mark
 */

#include "ExponentObj.h"
#include <iostream>

// test the zero base case
void test_min()
{
	u32 test_val = 0;
	cout << "Test Number is " << test_val << " : ";
	ExponentObj inst(test_val);
	inst.perform_exponentiation();
}

// test the one base case
void test_value_one()
{
	u32 test_val = 1;
	cout << "Test Number is " << test_val << " : ";
	ExponentObj inst(test_val);
	inst.perform_exponentiation();
}

// test the example provided
void test_provided_demo()
{
	u32 test_val = 123;
	cout << "Test Number is " << test_val << " : ";
	ExponentObj inst(test_val);
	inst.perform_exponentiation();
}

// test the largest number possible
// -- takes a long time for using a larger value
void test_max()
{
	u32 test_val = 9999;
	cout << "Test Number is " << test_val << " : ";
	ExponentObj inst(test_val);
	inst.perform_exponentiation();
}

int main()
{
	double user_input;
	cout << "Please enter an integer from 0-99999 to exponentiate, e.g. 123 -> 123^321 \n";

	// validate the input here to ensure the user input is between 0 - 99999
	// and that the value is an integer, not a decimal.
	while(1)
	{
		cin >> user_input;

		double integer_diff = user_input - (double)((u32)user_input);
		if(user_input >= 0 && user_input <= 99999 && integer_diff == 0)
		{
			break;
		}
		else
		{
			cout << "Input is not valid, try again \n";
		}
	}

	ExponentObj inst((u32)user_input);
	inst.perform_exponentiation();

	return 0;
}
