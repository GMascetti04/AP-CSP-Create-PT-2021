#pragma once
#include "xpch.h"

/*
* Key and Mouse button codes come from the values defined in
* https://github.com/glfw/glfw/blob/master/include/GLFW/glfw3.h
*/
namespace codes {

	enum class MouseCode {
		left_button = 0,
		right_button = 1,
		middle_button = 2
	};

	enum class KeyCode {
		unkown = -1,

		space = 32,
		apostrophe = 39,   
		comma = 44,        
		minus = 45,        
		period = 46,       
		forward_slash = 47,

		n0 = 48,         
		n1 = 49,         
		n2 = 50,         
		n3 = 51,         
		n4 = 52,         
		n5 = 53,         
		n6 = 54,         
		n7 = 55,         
		n8 = 56,         
		n9 = 57,         

		semicolon = 59,  
		equal = 61,      

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		left_bracket = 91,  
		back_slash = 92,    
		right_bracket = 93, 
		grave_accent = 96,  

		
		escape = 256,
		enter = 257,
		tab = 258,
		backspace = 259,
		insert = 260,
		del = 261,
		right = 262,
		left = 263,
		down = 264,
		up = 265,
		page_up = 266,
		page_down = 267,
		home = 268,
		end = 269,
		caps_lock = 280,
		scroll_lock = 281,
		num_lock = 282,
		print_screen = 283,
		pause = 284,
		f1 = 290,
		f2 = 291,
		f3 = 292,
		f4 = 293,
		f5 = 294,
		f6 = 295,
		f7 = 296,
		f8 = 297,
		f9 = 298,
		f10 = 299,
		f11 = 300,
		f12 = 301,
		

		//keypad
		kp0 = 320,
		kp1 = 321,
		kp2 = 322,
		kp3 = 323,
		kp4 = 324,
		kp5 = 325,
		kp6 = 326,
		kp7 = 327,
		kp8 = 328,
		kp9 = 329,
		kp_decimal = 330,
		kp_divide = 331,
		kp_multiply = 332,
		kp_subtract = 333,
		kp_add = 334,
		kp_enter = 335,
		kp_equal = 336,

		left_shift = 340,
		left_control = 341,
		left_alt = 342,
		left_super = 343,
		right_shift = 344,
		right_control = 345,
		right_alt = 346,
		right_Super = 347,
		menu = 348
	};
}
