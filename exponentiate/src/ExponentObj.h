/*
 * ExponentObj.h
 *
 *  Created on: Sep 29, 2023
 *      Author: marka
 */

#ifndef EXPONENTOBJ_H_
#define EXPONENTOBJ_H_

#include <vector>
#include <stdint.h>

using namespace std;

typedef uint8_t u8;
typedef uint32_t u32;

/*
 * Create a struct that holds a value both as a number and as a vector. Use
 * as the default input for funtion calls.
 */
typedef struct{

	vector<u8> as_vect; 		// store the number as a vector
	u32 	input_val;			// store the number as a number

}input_params;

class ExponentObj {

private:

	input_params a;				// base value
	input_params b;				// exponent value
	input_params state;			// final returned value
	input_params iter_state;	// iterative value that creates state
								//		through recursion
	u8 largest_mult_factor; 	//
	vector<vector<u8>> intermediate_mult_vals;
								// vector that stores the a^2, a^4, a^8, ...
								// 		building blocks

public:

	/*
	 * parameters: 		base (IN)
	 *
	 * The constructor takes the base input and generates the reversed exponent
	 */
	ExponentObj(u32 base);

	/*
	 * parameters: 		none
	 *
	 * Performs the high - level logic for choose the sequence of multiplication
	 * operations
	 */
	void perform_exponentiation();

private:

	/*
	 * parameters: 		compare_values (IN/OUT),
	 * 					greatest_exp (OUT)
	 *
	 * Finds the greatest base 2 exponentiated value that is less than the
	 * compare value. It returns the greatest exponent and the compare value
	 * is updated to be the initial compare value minus the exponent value for
	 * the recursive algorithm.
	 */
	void greatest_base_two_exponent(u8 * greatest_exp, u32 * compare_value);

	/*
	 * parameters		inst (IN)
	 *
	 * Print the digits that are stored as vectors in the input_params struct
	 */
	void print_digits(input_params * inst);

	/*
	 * parameters		inst (IN/OUT)
	 *
	 * Generates a vector of digits that represent a number. In this case, it is
	 * used to generate a vector for the base number and some intermediate
	 * values
	 */
	void vectorize_into_digits(input_params * inst);

	/*
	 * parameters		imput_num (IN)
	 * 					reversed_num (OUT)
	 *
	 * Reverses the digits of an input_params struct. This function is used
	 * to reverse the base input genreated by the user.
	 */
	void reverse_the_digits(input_params * reversed_num,
			input_params * input_num);

	/*
	 * parameters		in1 (IN)
	 * 					in2 (IN)
	 * 					out (OUT)
	 *
	 * This function does the heavy lifting by using long-multiplication rules
	 * to operate on vector representations of numbers.
	 */
	void multiplication_operation(input_params * out, input_params * in1,
			input_params * in2);

	/*
	 * parameters		max_row_length (IN)
	 * 					each_row_length (IN)
	 * 					num_rows (IN)
	 * 					row (IN)
	 * 					output (OUT)
	 *
	 * Called by multiplication operation to sum the intermediate multiplication
	 * rows. Breaks up the multiplication function into smaller chanks.
	 */
	void sum_rows(input_params * output, vector <vector<u8>>  rows,
			u32 num_rows, vector<u32> each_row_length, u32 max_row_length);

};

#endif /* EXPONENTOBJ_H_ */
