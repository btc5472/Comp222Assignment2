/*************************************
Brandon Cobb
Assignment 2
October 12, 2019
Comp 222 Tues & Thurs 9:30am - 10:45pm
*************************************/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void decimalToFloat();
void floatToDecimal();

int main()
{
	int choice = 0;

	do {
		printf("Floating-point conversion:\n");
		printf("1) Decimal to IEEE-754 conversion\n2) IEEE-754 to decimal representation\n3) Exit\n");
		printf("Enter Selection:");
		scanf("%d", &choice);

		if (choice == 1)
			decimalToFloat();

		if (choice == 2)
			floatToDecimal();

		if (choice > 3 || choice < 1)
			printf("Please enter a valid choice 1-3\n");

	} while (choice != 3);

	// Choice == 3 so quit program
	printf("\nProgram Terminated Normally");
	return(0);
}

// Decimal to IEEE-754 conversion
void decimalToFloat()
{
	double dec_num = 0, frac_of_dec = 0, whole_num_of_dec = 0;
	int decimal_point_shifts = 0;
	int exponent_of_dec = 0;

	// Prompt for decimal number
	printf("Enter the decimal representation: ");
	if (!scanf("%lf", &dec_num)) {
		printf("Decimal number was not entered correctly. Exiting program.\n");
		exit(116);
	}

	// Check for 0--if so, print result
	if (dec_num == 0) {
		printf("Sign: 0\nBiased Exponent : 00000000\nMantissa: 00000000000000000000000\nThe IEEE-754 representation is: 0.000000\n");
		return;
	}

	const double dec_num_const = dec_num;

	if (dec_num_const >= 1 || dec_num_const <= -1) {	// If there is a number before the decimal point then save that num
		whole_num_of_dec = dec_num_const;
		long long int num = whole_num_of_dec;
		whole_num_of_dec = num;
	}
	const double whole_num_of_dec_const = whole_num_of_dec;
	frac_of_dec = fabs(dec_num) - fabs(whole_num_of_dec);
	const double frac_of_dec_const = frac_of_dec;


	// Print sign: if number > 0, sign is 0, else 1
	if (dec_num_const >= 0)
		printf("Sign:0\n");
	else
		printf("Sign:1\n");


	// Normalize number:
	for (dec_num = fabs(dec_num_const); dec_num >= 2; decimal_point_shifts++) {
		dec_num /= 2;
	}

	for (dec_num = fabs(dec_num_const); dec_num < 1; decimal_point_shifts--) {
		dec_num *= 2;
	}
	const long long int normalized_dec_num = dec_num;

	// Decimal point shifts
	const int decimal_point_shifts_const = decimal_point_shifts;
	exponent_of_dec = decimal_point_shifts_const;

	// Bias exponent by 127 and print result in binary
	if (fabs(whole_num_of_dec) < 0)
		exponent_of_dec -= 127;
	else
		exponent_of_dec += 127;

	printf("Biased Exponent:");
	for (int i = 7; i >= 0; i--) {
		if (exponent_of_dec >= pow(2, i)) {
			printf("%d", 1);
			exponent_of_dec = (exponent_of_dec - pow(2, i));
		}
		else
			printf("%d", 0);
	}
	printf("\n");

	// Print Mantissa
	printf("Mantissa:");
	// Print 1st part of mantissa (Whole number part of decimal number without first leading 1)
	whole_num_of_dec = fabs(whole_num_of_dec);
	int i = 23, dps;
	for (dps = decimal_point_shifts_const; i >= 0; i--) {
		if (dps < 0)
			break;
		if (i == 23) {
			whole_num_of_dec -= pow(2, decimal_point_shifts_const);
			dps--;
			continue; // Dont print first bit of the whole number
		}

		if (whole_num_of_dec >= pow(2, dps)) {
			printf("%d", 1);
			whole_num_of_dec -= pow(2, dps);
		}
		else
			printf("%d", 0);
		dps--;
	}
	// Print the 2nd (fraction) part of the mantissa
	if (decimal_point_shifts_const < 0) {
		decimal_point_shifts = -1;
	} else if (decimal_point_shifts > 0)
		decimal_point_shifts = decimal_point_shifts_const;

	for (int i = (23 - decimal_point_shifts); i > 0; i--) {
		frac_of_dec *= 2;
		if (frac_of_dec < 1) {
			if (whole_num_of_dec_const == 0 && i == 24) {
				// Dont print bit
			}
			else
				printf("%d", 0);
		}
		else if (frac_of_dec > 1) {
			if (whole_num_of_dec_const == 0 && i == 24) {
				frac_of_dec -= 1; // Dont print bit
			}
			else {
				printf("%d", 1);
				frac_of_dec -= 1;
			}
		}
		else if (frac_of_dec == 1) {
			printf("%d", 1);
			frac_of_dec = 0;
		}
	}
	printf("\n");

	// Print IEEE-754 representation


	return;
}

// IEEE-754 to Decimal conversion
void floatToDecimal()
{
	unsigned long int fp_num = 0;

	// Prompt for IEEE-754 representation
	printf("Enter the IEEE-754 representation:");
	if (!scanf("%lx", &fp_num)) {
		printf("A hex number was not entered, exiting program.\n");
		exit(115);
	}

	const unsigned long int fp_num_const = fp_num;


	// Save sign bit, exponent, and mantissa into separate variables
	unsigned int sign;
	if (fp_num_const >= pow(2, 31)) {
		sign = 1;
		fp_num -= pow(2, 31);
	}
	else
		sign = 0;

	int exponent = 0;
	for (int i = 8; i > 0; i--) {
		if (fp_num >= pow(2, i+22)) {
			exponent += pow(2, i-1);
			fp_num -= pow(2, i+22);
		}
	}
	const unsigned int biased_exponent_const = exponent;
	const int unbiased_exponent = exponent - 127;

	unsigned int mantissa = 0;
	for (int i = 22; i >= 0; i--) {
		if (fp_num >= pow(2, i)) {
			mantissa += pow(2, i);
			fp_num -= pow(2, i);
		}
	}
	const unsigned int mantissa_const = mantissa;


	// Special case check for fp_num
	// Check for NaN
	if (mantissa_const > 0 && biased_exponent_const == 255) {
		printf("NaN\n");
		return;
	}
	// Check for +-Infinity
	if (biased_exponent_const == 255) {
		if (sign == 0)
			printf("+Infinity\n");
		else
			printf("-Infinity\n");
		return;
	}
	// Check for +-0
	if (fp_num_const == 0) {
		printf("Sign: +\nUnbiased Exponent: 0.0\nNormalized Decimal: 0.0\nDecimal: 0.0\n");
		return;
	}
	else if (fp_num_const == pow(2, 31)) {
		printf("Sign: -\nnUnbiased Exponent: 0.0\nNormalized Decimal: 0.0\nDecimal: 0.0\n");
		return;
	}


	// Print results
	// Print Sign
	if (sign == 0) {
		printf("Sign: +\n");
	}
	else
		printf("Sign: -\n");

	// Calculate fraction of mantissa
	double fraction = 0;
	mantissa = mantissa_const;
	for (int i = 1; mantissa != 0; i++) {
		if (mantissa >= pow(2, 23 - i)) {
			fraction = fraction + (1 / pow(2, i));
			mantissa -= pow(2, 23 - i);
		}
	}

	// If unbiased exponent=0, number is denormalized with unbiased exponent of -126,
	// print denormalized number as fraction * 2^(-126), return
	if (biased_exponent_const == 0 && mantissa_const > 0)
		printf("Denormalized number: %.10lf * 2^(-126)\n", fraction);
	else {
		printf("Unbiased Exponent: %d\n", unbiased_exponent);
		printf("Normalized Decimal: %lf\n", ((double)1 + fraction)); // 1 is added because it is a required implied num in IEEE 754
		printf("Decimal: %.40lf\n", ((double)1 + fraction) * pow(2, unbiased_exponent)); // Decimal represented by the hex entered
	}

	return;
}

// TODO: Test dec_num == 1
// Decimals in option selector
// Save sign, exp, and mantissa numbers to a string and then print the single string
// Make sure negatives work
// exp of dec has to be positive I think
// 0.0000000000000000000000000000000000001 doesnt work

/////////	 Make sure error handing of scanf works (Line 50). Make sure Normalizing the dec_num works (Line 80)   ////////////////

// EXTRA CODE //////////////////////////////////

	/* // Decimal to binary conversion
	for (int i = 31; i >= 0; i--) {
		int j = dec_num >> i;
		if (j & 1)
			printf("1");
		else
			printf("0");
	}  */
