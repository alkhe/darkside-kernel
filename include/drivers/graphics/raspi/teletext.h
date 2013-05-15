#ifndef __TELETEXT_H
#define __TELETEXT_H

// Character definitions from SAA5050 datasheet
// Each character is a 5x9 bit matrix
// 9 rows of 5-bit numbers
static unsigned char teletext[][9] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0},			// space
	{4, 4, 4, 4, 4, 0, 4, 0, 0},			// !
	{10, 10, 10, 0, 0, 0, 0, 0, 0},			// "
	{10, 31, 10, 10, 10, 31, 10, 0, 0},		// #
	{14, 21, 20, 14, 5, 21, 14, 0, 0},		// $
	{24, 25, 2, 4, 8, 19, 3, 0, 0},			// %
	{8, 20, 20, 8, 21, 18, 13, 0, 0},		// &
	{4, 4, 4, 0, 0, 0, 0, 0, 0},			// '
	{2, 4, 8, 8, 8, 4, 2, 0, 0},			// (
	{8, 4, 2, 2, 2, 4, 8, 0, 0},			// )
	{4, 21, 14, 4, 14, 21, 4, 0, 0},		// *
	{0, 4, 4, 31, 4, 4, 0, 0, 0},			// +
	{0, 0, 0, 0, 0, 4, 4, 8, 0},			// ,
	{0, 0, 0, 14, 0, 0, 0, 0, 0},			// -
	{0, 0, 0, 0, 0, 0, 4, 0, 0},			// .
	{0, 1, 2, 4, 8, 16, 0, 0, 0},			// /
	{4, 10, 17, 17, 17, 10, 4, 0, 0},		// 0
	{4, 12, 4, 4, 4, 4, 14, 0, 0},			// 1
	{14, 17, 1, 6, 8, 16, 31, 0, 0},		// 2
	{31, 1, 2, 6, 1, 17, 14, 0, 0},			// 3
	{2, 6, 10, 18, 31, 2, 2, 0, 0 },		// 4
	{31, 16, 30, 1, 1, 17, 14, 0, 0},		// 5
	{6, 8, 16, 30, 17, 17, 14, 0, 0},		// 6
	{31, 1, 2, 4, 8, 8, 8, 0, 0},			// 7
	{14, 17, 17, 14, 17, 17, 14, 0, 0},		// 8
	{14, 17, 17, 15, 1, 2, 12, 0, 0},		// 9
	{0, 0, 4, 0, 0, 0, 4, 0, 0},			// :
	{0, 0, 4, 0, 0, 4, 4, 8, 0},			// ;
	{2, 4, 8, 16, 8, 4, 2, 0, 0},			// <
	{0, 0, 31, 0, 31, 0, 0, 0, 0},			// =
	{8, 4, 2, 1, 2, 4, 8, 0, 0},			// >
	{14, 17, 2, 4, 4, 0, 4, 0, 0},			// ?
	{14, 17, 23, 21, 23, 16, 14, 0, 0},		// @
	{4, 10, 17, 17, 31, 17, 17, 0, 0},		// A
	{30, 17, 17, 30, 17, 17, 30, 0, 0},		// B
	{14, 17, 16, 16, 16, 17, 14, 0, 0},		// C
	{30, 17, 17, 17, 17, 17, 30, 0, 0},		// D
	{31, 16, 16, 30, 16, 16, 31, 0, 0},		// E
	{31, 16, 16, 30, 16, 16, 16, 0, 0},		// F
	{14, 17, 16, 16, 19, 17, 15, 0, 0},		// G
	{17, 17, 17, 31, 17, 17, 17, 0, 0},		// H
	{14, 4, 4, 4, 4, 4, 14, 0, 0},			// I
	{1, 1, 1, 1, 1, 17, 14, 0, 0},			// J
	{17, 18, 20, 24, 20, 18, 17, 0, 0},		// K
	{16, 16, 16, 16, 16, 16, 31, 0, 0},		// L
	{17, 27, 21, 21, 17, 17, 17, 0, 0},		// M
	{17, 17, 25, 21, 19, 17, 17, 0, 0},		// N
	{14, 17, 17, 17, 17, 17, 14, 0, 0},		// O
	{30, 17, 17, 30, 16, 16, 16, 0, 0},		// P
	{14, 17, 17, 17, 21, 18, 13, 0, 0},		// Q
	{30, 17, 17, 30, 20, 18, 17, 0, 0},		// R
	{14, 17, 16, 14, 1, 17, 14, 0, 0},		// S
	{31, 4, 4, 4, 4, 4, 4, 0, 0},			// T
	{17, 17, 17, 17, 17, 17, 14, 0, 0},		// U
	{17, 17, 17, 10, 10, 4, 4, 0, 0},		// V
	{17, 17, 17, 21, 21, 21, 10, 0, 0},		// W
	{17, 17, 10, 4, 10, 17, 17, 0, 0},		// X
	{17, 17, 10, 4, 4, 4, 4, 0, 0},			// Y
	{31, 1, 2, 4, 8, 16, 31, 0, 0},			// Z
	{24, 16, 16, 16, 16, 16, 24, 0, 0},		// [
	{16, 8, 4, 2, 1, 0, 0, 0},				// (backslash)
	{3, 1, 1, 1, 1, 1, 3, 0, 0},			// ]
	{4, 10, 17, 0, 0, 0, 0, 0, 0},			// ^
	{0, 0, 0, 0, 0, 0, 31, 0, 0},			// _
	{16, 8, 4, 0, 0, 0, 0, 0, 0},			// `
	{0, 0, 14, 1, 15, 17, 15, 0, 0},		// a
	{16, 16, 30, 17, 17, 17, 30, 0, 0},		// b
	{0, 0, 15, 16, 16, 16, 15, 0, 0},		// c
	{1, 1, 15, 17, 17, 17, 15, 0, 0},		// d
	{0, 0, 14, 17, 31, 16, 14, 0, 0},		// e
	{2, 4, 4, 14, 4, 4, 4, 0, 0},			// f
	{0, 0, 15, 17, 17, 17, 15, 1, 14},		// g
	{16, 16, 30, 17, 17, 17, 17, 0, 0},		// h
	{4, 0, 12, 4, 4, 4, 14, 0, 0},			// i
	{4, 0, 4, 4, 4, 4, 4, 4, 8},			// j
	{8, 8, 9, 10, 12, 10, 9, 0, 0},			// k
	{12, 4, 4, 4, 4, 4, 14, 0, 0},			// l
	{0, 0, 26, 21, 21, 21, 21, 0, 0},		// m
	{0, 0, 30, 17, 17, 17, 17, 0, 0},		// n
	{0, 0, 14, 17, 17, 17, 14, 0, 0},		// o
	{0, 0, 30, 17, 17, 17, 30, 16, 16},		// p
	{0, 0, 15, 17, 17, 17, 15, 1, 1},		// q
	{0, 0, 11, 12, 8, 8, 8, 0, 0},			// r
	{0, 0, 15, 16, 14, 1, 30, 0, 0},		// s
	{4, 4, 14, 4, 4, 4, 2, 0, 0},			// t
	{0, 0, 17, 17, 17, 17, 15, 0, 0},		// u
	{0, 0, 17, 17, 10, 10, 4, 0, 0},		// v
	{0, 0, 17, 17, 21, 21, 10, 0, 0},		// w
	{0, 0, 17, 10, 4, 10, 17, 0, 0},		// x
	{0, 0, 17, 17, 17, 17, 15, 1, 14},		// y
	{0, 0, 31, 2, 4, 8, 31, 0, 0},			// z
	{12, 8, 8, 16, 8, 8, 12, 0, 0},			// {
	{4, 4, 4, 4, 4, 4, 4, 0, 0},			// |
	{6, 2, 2, 1, 2, 2, 6},					// }
	{0, 0, 8, 21, 2, 0, 0, 0, 0},			// ~
	{31, 31, 31, 31, 31, 31, 31, 31, 31}	// (full sized block)
};

#endif