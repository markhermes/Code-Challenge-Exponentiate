/*
 * 	ExponentObj.cpp
 *
 *	Defines an object for exponentiating integers.
 *
 *	Because we are interested in exponentiating very large numbers, (the example
 *	provided results in ~ 10^670 and the maximum double value is ~ 10^308, we
 *	have to resort to fundamental long multiplication rules with arrays
 *	that contain representations of the base and intermediate numbers.
 *
 *	Similarly, we expand the exponent to consist of a sequence of
 *	multiplications A^3 = A*A*A, where A2 = A*A is performed, then A2*A is
 *	performed. In this code, we refer to A2 as the state and A as a
 *
 *	The operation can be achieved more efficiently by noticing that
 *	A^4 = (A*A)*(A*A), since we have already calcultated A2 = A*A, this is
 *	simply A2*A2 (saves a calculation). However, since efficiency is not
 *	paramount, we choose this simpler option described in the previous
 *	paragraph.
 *
 */

#include <iostream>
#include <cmath>
#include "ExponentObj.h"


ExponentObj::ExponentObj(u32 base)
{

	state.as_vect.reserve(1000000);
	intermediate_mult_vals.reserve(100000);

	// if the exponent is zero, then the result is 1
	if(base == 0) b.input_val = 0;
	else
	{
		a.input_val = base;
		vectorize_into_digits(&a);
		reverse_the_digits(&b,&a);

		//find the greatest multiplication factor that we need
		u32 exp = b.input_val;

		greatest_base_two_exponent(&largest_mult_factor, &exp);
	}
}


void ExponentObj::perform_exponentiation()
{
	int i;

	// initialize the state for the first operation
	state = a;

	if(b.input_val > 0)
	{

		//initialize
		intermediate_mult_vals.push_back(state.as_vect);
		// populate the vector containing the multiplication products
		for(i = 1; i <= largest_mult_factor; i++)
		{
			input_params new_state, intermediate_state;
			intermediate_state.as_vect  = intermediate_mult_vals[(i-1)];
			multiplication_operation(&new_state,&intermediate_state,
					&intermediate_state);

			// add the new state to the buffer
			intermediate_mult_vals.push_back(new_state.as_vect);
		}

		//begin the process of combining the intermediate values
		u32 num_multiplications_left;

		// first, multiply by the highest level
		u32 prev_exp_val;
		u32 exp_val = b.input_val;
		u8 exp_level = largest_mult_factor;

		// init iter_state to 1
		iter_state.as_vect.push_back(1);
		if(exp_level > 0)
		{
		while(exp_val > 0)
		{

			prev_exp_val = exp_val;
			greatest_base_two_exponent(&exp_level,&exp_val);

			u32 num_intermed_mults = prev_exp_val / pow(2,exp_level);

			// perform exp_val / exp_level number of operations
			for(i = 0; i < num_intermed_mults; i++)
			{
				input_params mult_arg;
				mult_arg.as_vect = intermediate_mult_vals[exp_level];
				multiplication_operation(&iter_state, &iter_state, &mult_arg);
			}
			//cout << "exponent val : " << exp_val;
			//cout << "exponent level : " << exp_level << "\n";

			exp_level --;
		}
		state = iter_state;
		}
//		// perform the sequence of multiplication operations
//		for(i = 1; i < b.input_val; i++)
//		{
//			input_params in1 = a;
//			input_params in2 = state;
//
//			multiplication_operation(&state,&in1,&in2);
//		}

		// if the base value is nonzero
		if(state.as_vect.size()>0)
		{
			// print the digits
			cout << "value is: ";
			for(i = (state.as_vect.size() - 1); i >= 0;  --i)
			{ cout << (int)state.as_vect[i];}
			cout << "\n";
		}

		// handle the zero base case
		else{cout << "value is: 0 \n";}
	}
	else{ cout << "value is: 1 \n";}

}

void ExponentObj::print_digits(input_params * inst)
{
	int i;
	for(i = (inst->as_vect.size() - 1); i >= 0;  --i)
	{ cout << (int)inst->as_vect[i];}
	cout << "\n";
}

void ExponentObj::vectorize_into_digits(input_params * inst)
{
	u32 input = inst->input_val;

	//decompose input into an array
	while(input > 0)
	{
		u8 digit = fmod(input,10);
		input = input/10;
		inst->as_vect.push_back(digit);
	}
}

void ExponentObj::reverse_the_digits(input_params * exp , input_params * base)
{
	int i;
	u32 sum = 0;

	// calculate the reversed value as a vector
	u8 max_i = (base->as_vect.size() - 1);
	for(i = max_i; i >= 0; i--)
	{
		exp->as_vect.push_back(base->as_vect[i]);
	}

	// calculate the reversed value as a number
	u32 base_val = base->input_val;
	for(i = 0; i <= max_i; i++)
	{
		sum *= 10;
		sum += base_val % 10;
		base_val /= 10;
	}

	exp->input_val = sum;
}

void ExponentObj::greatest_base_two_exponent(u8 * greatest_exp,
											 u32 * compare_value)
{
	u32 i;

	// only search up to 17 because the input is restricted :
	// 2^16 < 99999 < 2^17
	for(i=0;i<17;i++)
	{
		u32 two_exp = pow(2,i);

		if(*compare_value < two_exp)
		{
			*greatest_exp = (i-1);

			//update the new compare value
			*compare_value =  *compare_value - pow(2,(i-1));

			return;
		}
	}
}

void ExponentObj::multiplication_operation(input_params * out,
		input_params * mult_arg1, input_params * mult_arg2)
{


	// structure the multiplication so that in1 has a smaller size
	input_params in1,in2;
	if (mult_arg2->as_vect.size() > mult_arg1->as_vect.size())
	{ in1 = *mult_arg1; in2 = *mult_arg2; }
	else
	{  in1 = *mult_arg2; in2 = *mult_arg1; }

	u32 i;
	u32 j;
	vector<u32> each_row_length;
	vector<vector <u8>> row_digits;
	vector<u8> temp_vector;
	u32 carry_over_val;

	// each multiplied digit generates a row to add
	for(i=0; i<in1.as_vect.size(); i++)
	{
		carry_over_val = 0;

		// reset the vars
		temp_vector.clear();

		// add zeros to the base of the row
		for(u32 add_zero = 0;add_zero < i; add_zero++)
		{
			temp_vector.push_back(0);
		}

		// iterate through the large argument
		for(j=0; j<in2.as_vect.size(); j++)
		{
			u8 intermediate_val = in1.as_vect[i] * in2.as_vect[j]
															   + carry_over_val;

			// scenario: no carry-over
			if(intermediate_val <= 9)
			{
				temp_vector.push_back(intermediate_val);
				carry_over_val = 0;
			}

			// scenario: the multiplication requires 2 digits, a.k.a. carry-over
			else
			{
				input_params row_val; row_val.input_val = intermediate_val;
				vectorize_into_digits(&row_val);

				// if we are not at the last digit
				u32 test = in2.as_vect.size();
				if(j < (test-1))
				{
					carry_over_val = row_val.as_vect[1];
					temp_vector.push_back(row_val.as_vect[0]);
				}

				// if we are at the last digit then use both carry over val and
				// row_val
				else
				{
					temp_vector.push_back(row_val.as_vect[0]);
					temp_vector.push_back(row_val.as_vect[1]);
				}
			}
		}
		row_digits.push_back(temp_vector);

		// need to make sure i do not try to access value outside of vector mem
		// bounds
		each_row_length.push_back(temp_vector.size());
	}


	//input_params print_dig;
	//print_dig.as_vect = row_digits;
	//print_digits(&print_dig);

	sum_rows(out, row_digits, in1.as_vect.size(), each_row_length,
			temp_vector.size());
}


void ExponentObj::sum_rows(input_params * output, vector <vector<u8>>  rows,
		u32 num_rows, vector<u32> each_row_length, u32 max_row_length)
{
	//clear state since we already used it in the multiplication process.
	output->as_vect.clear();

	u32 i; u32 j; u32 col_sum; u32 carry_over = 0;
	for(j = 0; j<max_row_length; j++)
	{
		col_sum = 0;

		// sum the rows --- only add values that were pushed into the vectors
		for(i = 0; i < num_rows; i++){
			if(j < each_row_length[i])
			{
				col_sum += rows[i][j];
			}
		}

		// add the carry over
		col_sum += carry_over;

		// no carry-over
		if(col_sum <= 9)
		{
			output->as_vect.push_back(col_sum);
			carry_over = 0;
		}
		else
		{
			input_params col_val; col_val.input_val = col_sum;
			vectorize_into_digits(&col_val);

			// if we are not at the last digit
			if(j<(max_row_length-1))
			{
				carry_over = col_val.as_vect[1];
				output->as_vect.push_back(col_val.as_vect[0]);
			}

			// if we are at the last digit then use both carry over val and
			// row_val
			else
			{
				output->as_vect.push_back(col_val.as_vect[0]);
				output->as_vect.push_back(col_val.as_vect[1]);
			}
		}
	}
}
