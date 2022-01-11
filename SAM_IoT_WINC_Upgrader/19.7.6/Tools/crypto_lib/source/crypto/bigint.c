/*!
@file       		
	bigint.c

@brief	
	Implementation of the bigint Library.

@sa				
	The Handbook of Applied Cryptography , chapter14
*/

/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
INCLUDES
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#include "crypto.h"
#include "nmi_m2m.h"



/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*
MACROS
*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

#define MULT(x,y,i,r)		if(y & BIT##i) \
								r+= (x << i);
#ifdef WIN32
#define Multiply32_(u32A, u32B,P0,P1)\
{\
	uint64	u64P = 0;\
	u64P = ((uint64)(u32A) * (uint64)(u32B));\
	*(P0) = (uint32)u64P;\
	*(P1) = (uint32)(u64P >> 32);\
}

#else
#define Multiply32_(u32A, u32B,P0,P1)\
{\
	uint64	u64A,u64P = 0;\
	u64A = (uint64)u32A;\
	MULT(u64A,u32B,0,u64P);\
	MULT(u64A,u32B,1,u64P);\
	MULT(u64A,u32B,2,u64P);\
	MULT(u64A,u32B,3,u64P);\
	MULT(u64A,u32B,4,u64P);\
	MULT(u64A,u32B,5,u64P);\
	MULT(u64A,u32B,6,u64P);\
	MULT(u64A,u32B,7,u64P);\
	MULT(u64A,u32B,8,u64P);\
	MULT(u64A,u32B,9,u64P);\
	MULT(u64A,u32B,10,u64P);\
	MULT(u64A,u32B,11,u64P);\
	MULT(u64A,u32B,12,u64P);\
	MULT(u64A,u32B,13,u64P);\
	MULT(u64A,u32B,14,u64P);\
	MULT(u64A,u32B,15,u64P);\
	MULT(u64A,u32B,16,u64P);\
	MULT(u64A,u32B,17,u64P);\
	MULT(u64A,u32B,18,u64P);\
	MULT(u64A,u32B,19,u64P);\
	MULT(u64A,u32B,20,u64P);\
	MULT(u64A,u32B,21,u64P);\
	MULT(u64A,u32B,22,u64P);\
	MULT(u64A,u32B,23,u64P);\
	MULT(u64A,u32B,24,u64P);\
	MULT(u64A,u32B,25,u64P);\
	MULT(u64A,u32B,26,u64P);\
	MULT(u64A,u32B,27,u64P);\
	MULT(u64A,u32B,28,u64P);\
	MULT(u64A,u32B,29,u64P);\
	MULT(u64A,u32B,30,u64P);\
	MULT(u64A,u32B,31,u64P);\
	*(P0) = (uint32)u64P;\
	*(P1) = (uint32)(u64P >> 32);\
}
#endif

/*********************************************************************
Function
	Multiply32

Description
	Multiplies 2 32-bit integer values. 

Return
	None. 
*********************************************************************/
void Multiply32
(
uint32 	u32A,
uint32 	u32B,
uint32 	*pu32P0,
uint32 	*pu32P1
)
{
	Multiply32_(u32A,u32B,pu32P0,pu32P1);
}
/*********************************************************************
Function
	BI_Sizeof

Description
	Gets the actual size of a digit after excluding unnecessary zeros.

Return
	The actual size of the digit or zero.
*********************************************************************/
uint32 BI_Sizeof
(	
uint32 	*pu32Digit, 
uint32 	u32Size
)
{
	uint32 u32ActualSize = 0;

	if(pu32Digit != NULL)
	{
		sint32 	s32Idx;
		u32ActualSize = u32Size;
		for(s32Idx = u32ActualSize - 1;s32Idx >= 0 ; s32Idx --)
		{
			if(pu32Digit[s32Idx] != 0)
			{
				break;
			}
		    	u32ActualSize --;
		}
	}
	return u32ActualSize;
}
/*********************************************************************
Function
	BI_Complement

Description
	Computes the two's complement of a big integer

Return
	None
*********************************************************************/
static void BI_Complement
(
	uint32 	*pu32W, 
	uint32 	*pu32WComplement, 
	uint32 	u32Wsize
)
{
	if((pu32W != NULL) && (pu32WComplement != NULL) && (u32Wsize > 0))
	{
		uint32 u32Counter,u32Carry;
		u32Carry = 1;
		for(u32Counter = 0;u32Counter <u32Wsize ;u32Counter++)
		{
			pu32WComplement[u32Counter] = (~pu32W[u32Counter]) + u32Carry;
		}
	}
}
/*********************************************************************
Function
	BI_ShortDivide

Description
	Computes the division : (X1 * b + X0) / Y0

Return
	The division result Q
*********************************************************************/
static uint32 BI_ShortDivide
(
	uint32 	u32X1,
	uint32 	u32X0,
	uint32 	u32Y0
)
{
/* 
ALGORITHM 
		Unsigned Integer Division with Remainder
REFERENCE
		http://en.wikipedia.org/wiki/Division_algorithm#Integer_division_.28unsigned.29_with_remainder
****************************************************************************************
if D == 0 then throw DivisionByZeroException end
Q := 0                    initialize quotient and remainder to zero
R := 0                     
for i = n-1...0 do   " where n is number of bits "
  R := R << 1               left-shift R by 1 bit     
  R(0) := N(i)             set the least-significant bit of R equal to bit i of the numerator
  if R >= D then
    R = R - D               
    Q(i) := 1
  end
end  
****************************************************************************************/

	uint32 	u32Q = 0;

	if((u32Y0 != 0) && ((u32X0 != 0) || (u32X1 != 0)))
	{
		uint32 	u32R0,u32R1;
		uint32 	u32BitMask = BIT31;
		sint32 		i;

		/* Remainder of the Most Significant digit. */
		u32R0 = u32X1 % u32Y0;
		u32R1 = 0;

        /* Perform division of the Least significant digit. */
		for(i = 31; i >= 0; i --)
		{
			/* Shift the remainder with one bit to left
			R <== R << 1
			*/
			u32R1 <<= 1;
			if(u32R0 & BIT31)
				u32R1 |= BIT0; 
			u32R0 <<= 1;

			/* Set bit0 of R to the current bit of X
			R(0) = X(i)
			*/
			if(u32BitMask & u32X0)
				u32R0 |= BIT0;

			/* If R > Y then 
			- R <== R-Y 
			- Q(i) = 1
			*/
			if((u32R0 > u32Y0) || (u32R1 > 0))
			{
				u32R1 -= ((u32R0 > u32Y0) ? 0 : 1);
				u32R0 -= u32Y0;
				u32Q |= u32BitMask;
			}
			u32BitMask >>= 1;
		}
	}
	return u32Q;
}
/*********************************************************************
Function
	BI_Add

Description
	Adding two arrays of integers.

Return
	Addition Carry.
*********************************************************************/
static sint32 BI_Add
(
	uint32 * pu32X, 
	uint32 * pu32Y, 
	uint32 * pu32Sum, 
	uint32 u32Size)
{
	sint32 		s32Ret = 0;
	
	if((pu32X != NULL) && (pu32Y != NULL) && (pu32Sum != NULL) && (u32Size != 0))
	{
		uint32 u32Counter;
		uint32 u32Carry = 0;
		uint32 u32Sum;

		/* Perform the Addition */
		for(u32Counter = 0 ;u32Counter < u32Size ; u32Counter ++)
		{
			u32Sum = u32Carry;
			u32Carry = 0;
			u32Sum += pu32X[u32Counter];
			if(u32Sum < pu32X[u32Counter])
				u32Carry ++;
			u32Sum += pu32Y[u32Counter];
			if(u32Sum < pu32Y[u32Counter])
				u32Carry ++;
			pu32Sum[u32Counter] = u32Sum;
		}

		/* Return the carry */
		s32Ret = (sint32)u32Carry;
	}
	return s32Ret;
}
/*********************************************************************
Function
	BI_Subtract

Description
	Subtracts two arrays of integers

Return
	subtraction carry.
*********************************************************************/
static sint32 BI_Subtract
(
	uint32 	*pu32X, 
	uint32 	*pu32Y, 
	uint32 	*pu32Diff, 
	uint32 	u32Size
)
{
	sint32 		s32Ret = 0;
	
	if((pu32X != NULL) && (pu32Y != NULL) && (pu32Diff != NULL) && (u32Size != 0))
	{
		uint32 u32Counter;
		uint32 u32Carry = 0;
		uint32 u32Temp;

		/* Perform the Addition */
		for(u32Counter = 0 ;u32Counter < u32Size ; u32Counter ++)
		{
			u32Temp = pu32Y[u32Counter] + u32Carry;

			/* Get the Carry */
			u32Carry = ((u32Temp > pu32X[u32Counter])?1 : 0);

			/* Get the Sum */
			pu32Diff[u32Counter] = pu32X[u32Counter] - u32Temp;
		}
		
		/* Overflow happens (X < Y) Get the complement of the result */
		if(u32Carry != 0)
		{
			/* Obtain the Complement of the Second operand */
			BI_Complement(pu32Diff,pu32Diff,u32Size);
		}
		/* Return the carry */
		s32Ret = (sint32)(u32Carry);
	}
	return s32Ret;
}
/*********************************************************************
Function
	BI_MultSub

Description
	Computes the expression : X = X - q * Y;

Return
	The last carry of the subtraction. 
*********************************************************************/
static uint32 BI_MultSub
(
uint32 	*pu32X,
uint32 	*pu32Y,
uint32 	u32q,
uint32 	u32Ysize
)
{
	uint32 	u32SubtractionCarry = 0;

	if(u32q != 0)
	{
		uint32 	u32Counter;
		uint32 	u32ProductCarry = 0;
		uint32 	u32P0,u32P1;
		uint32	u32Tmp, u32SubBase;

		/* The multiplication and the subtraction are done in the same loop for fast operation */
		for(u32Counter = 0;u32Counter < u32Ysize ;u32Counter ++)
		{
			u32P0 = u32P1 = 0;
			u32SubBase = 0;

			/* Multiply ==> Calculates q * Y[i] */
			Multiply32_(u32q,pu32Y[u32Counter],&u32P0,&u32P1);
			u32P0 += u32ProductCarry;
			if(u32P0 < u32ProductCarry)
				u32P1 ++;
			u32ProductCarry = u32P1;
			
			/* Subtract ==> Calculates X[i] = X[i] - q * Y[i] */
			u32SubBase += u32SubtractionCarry;
			if(u32SubBase < u32SubtractionCarry)
				u32SubtractionCarry = 1;
			else
				u32SubtractionCarry = 0;

			u32SubBase += u32P0;
			if(u32SubBase < u32P0)
				u32SubtractionCarry ++;		

			if(u32SubBase > pu32X[u32Counter])
			{
				u32SubtractionCarry++;
			}
			pu32X[u32Counter] -= u32SubBase;
		}
		u32SubtractionCarry += u32ProductCarry;
		u32Tmp = u32SubtractionCarry;
		u32SubtractionCarry = ((u32SubtractionCarry > pu32X[u32Ysize])?1 : 0);
		pu32X[u32Ysize] -= u32Tmp;
	}
	return u32SubtractionCarry;
}
/*********************************************************************
Function
	BI_Compare

Description
	Compares two arrays of integers

Return 
	The function returns one of three values
	- 0 If the two arrays are equal.
	- positive integer if X > Y.
	- negative value if X < Y.
*********************************************************************/
static sint32 BI_Compare
(	
	uint32 	*pu32X,
	uint32 	*pu32Y,
	uint32 	u32Size
)
{
	sint32 		s32Ret = 0;

	if((pu32X != NULL) && (pu32Y != NULL) && (u32Size > 0))
	{
		sint32 s32Counter;

		for(s32Counter = u32Size - 1; s32Counter >= 0; s32Counter --)
		{
			if(pu32X[s32Counter] != pu32Y[s32Counter])
			{
				s32Ret = ((pu32X[s32Counter] > pu32Y[s32Counter])? 1 : -1);
				break;
			}
		}
	}
	else
	{
		s32Ret = -1;
	}	
	return s32Ret;
}
/*********************************************************************
Function
	BI_Divide

Description
	Divide two big integers. Calculate (X/Y) to get Q and R where X = Q * Y + R.
	Implements Algorithm 14.20 in the handbook of applied cryptography.

Return
	None.
*********************************************************************/
void BI_Divide
(
uint32 	*pu32X, 
uint32 	*pu32Y, 
uint32 	*pu32Q, 
uint32 	*pu32R,
uint32 	u32Xsize, 
uint32 	u32Ysize
)
{
	if((pu32X != NULL) && (pu32Y != NULL) && (pu32Q != NULL) && (pu32R != 0))
	{
		if((u32Ysize > 0) && (u32Xsize > 0))
		{
			uint32 n,t;

			/* Obtain the sizes of the division operands */
			n = u32Xsize - 1;
			t = u32Ysize - 1;

			if((n < t) || ((n == t) && (BI_Compare(pu32X, pu32Y, n + 1) < 0)))
			{
				/* X < Y, Then the remainder will be equal to X and quotient is zero.
				* Q = 0, R = X
				*/
				M2M_MEMCPY(pu32R,pu32X,u32Xsize * sizeof(uint32));
			}
			else
			{
				uint32 au32Xtemp[3], au32Ytemp[3];
				uint32 i;
				uint32 u32P0,u32P1;

				/*======*======*======*
					STEP 1 
						Initialization		
				*======*======*======*/
				M2M_MEMSET(pu32Q,0,(n-t+1) * sizeof(uint32));
				M2M_MEMSET(pu32R,0,(t+1) * sizeof(uint32));

				/*======*======*======*======*
					STEP 2 
						While X >= Y * b ^ (n-t)		
							- Q[n-t] <== Q[n-t] +1; 	
							- X <== X - Y * b ^ (n-t);	
				*======*======*======*======*/
				do
				{
					if(BI_Compare(&pu32X[n-t], pu32Y, t+1) >= 0)
					{
						pu32Q[n-t] ++;
						BI_Subtract(&pu32X[n-t], pu32Y, &pu32X[n-t],t+1);
					}
					else
					{
						break;
					}
				}while(1);
				/*======*======*======*======*
					STEP 3	
						Loop from i = n downto (t+1)	
				*======*======*======*======*/
				for(i = n; i > t ; i--)
				{
					/*======*======*======*======*======*======*
						STEP 3.1								
							If X[i] == Y[t]	Then Q[i-t-1] <== (b-1)	
							Else  Q[i-t-1] <== (X[i] * b + X[i-1])/Y[t]
					*======*======*======*======*======*======*/
					if(pu32X[i] == pu32Y[t])
					{
						pu32Q[i-t-1] = (uint32)(-1);
					}
					else
					{
						/* Calculate the expression Q[i-t-1] = (X[i] * b + X[i-1])/Y[t] */
						pu32Q[i-t-1] = BI_ShortDivide(pu32X[i], pu32X[i - 1], pu32Y[t]);
					}

					/*======*======*======*======*======*======*
						STEP 3.2														
							While  Q[i-t-1] * (Y[t] * b + Y[t-1]) > (X[i] * b2 + X[i-1] * b + X[i-2])	
								- Q[i-t-1] <== Q[i-t-1] - 1;			
					*======*======*======*======*======*======*/

					/* Calculate (X[i] * b2 + X[i-1] * b + X[i-2]) */
					au32Xtemp[2] = pu32X[i];
					au32Xtemp[1] = pu32X[i - 1];
					au32Xtemp[0] = pu32X[i - 2];

					do
					{
						u32P0 = u32P1 = 0;

						/* Calculate Q[i-t-1] * (Y[t] * b + Y[t-1]) */
						Multiply32(pu32Y[t - 1], pu32Q[i-t-1] ,&au32Ytemp[0], &au32Ytemp[1]);
						Multiply32(pu32Y[t], pu32Q[i-t-1] ,&u32P0, &u32P1);
						au32Ytemp[1] += u32P0;
						if(au32Ytemp[1] < u32P0)
							u32P1 ++;
						au32Ytemp[2] = u32P1;

						/* Compare */
						if(BI_Compare(au32Ytemp,au32Xtemp,3) > 0)
						{
							pu32Q[i-t-1] --;
						}
						else
						{
							break;
						}
					}while(1);

					/*======*======*======*======*======*
						STEP 3.3							
							X <== X - Q[i-t-1] * Y * b ^ (i-t-1)		
					*======*======*======*======*======*/
					if(BI_MultSub(&pu32X[i-t-1], pu32Y,pu32Q[i-t-1] ,t+1) > 0)
					{	
                        // TODO: This path is not tested at all. In all testing and actual scenarios it is not entered at all.
						/*=======================	*
						* STEP 3.4						*
						*	If X < 0 Then					*
						*		- Q[i-t-1] <== Q[i-t-1] - 1;	*
						*		- X <== X + Y * b ^ (i-t-1);*	
						*========================	*/
						uint32	u32Carry = 0;
						pu32Q[i-t-1] --;
						u32Carry = BI_Add(&pu32X[i-t-1], pu32Y,&pu32X[i-t-1],t);
						pu32X[i] += u32Carry;
					}
				}

				/********************************
				* STEP 4							*
				*	R <== X						*
				********************************/
				M2M_MEMCPY(pu32R,pu32X,(t+1) * 4);
			}
		}
	}
}
/*********************************************************************
Function
	BI_Square

Description
	Multiplies two big integers

Return
	NONE
*********************************************************************/
static void BI_Square(uint32 *x,uint32 *w, uint32 ndigits)
{
	/*	Computes square w = x * x
		where x is a multiprecision integer of ndigits
		and w is a multiprecision integer of 2*ndigits

		Ref: Menezes p596 Algorithm 14.16 with errata.
	*/

	uint32 k, p[2], u[2], cbit, carry;
	uint32 i, j, t, i2, cpos;

	t = ndigits;

	/* 1. For i from 0 to (2t-1) do: w_i = 0 */
	i2 = t << 1;
	for (i = 0; i < i2; i++)
		w[i] = 0;

	carry = 0;
	cpos = i2-1;
	/* 2. For i from 0 to (t-1) do: */
	for (i = 0; i < t; i++)
	{
		/* 2.1 (uv) = w_2i + x_i * x_i, w_2i = v, c = u 
		   Careful, w_2i may be double-prec
		*/
		i2 = i << 1; /* 2*i */
		Multiply32(x[i], x[i], &p[0],&p[1]);
		p[0] += w[i2];
		if (p[0] < w[i2])
			p[1]++;
		k = 0;	/* p[1] < b, so no overflow here */

		if (i2 == cpos && carry)
		{
			p[1] += carry;
			if (p[1] < carry)
				k++;
			carry = 0;
		}
		w[i2] = p[0];
		u[0] = p[1];
		u[1] = k;

		/* 2.2 for j from (i+1) to (t-1) do:
		   (uv) = w_{i+j} + 2x_j * x_i + c,
		   w_{i+j} = v, c = u,
		   u is double-prec 
		   w_{i+j} is dbl if [i+j] == cpos
		*/
		k = 0;
		for (j = i+1; j < t; j++)
		{
			/* p = x_j * x_i */
			Multiply32(x[i], x[j], &p[0],&p[1]);
			
			/* p = 2p <=> p <<= 1 */
			cbit = (p[0] & BIT31) != 0;
			k =  (p[1] & BIT31) != 0;
			p[0] <<= 1;
			p[1] <<= 1;
			p[1] |= cbit;
			
			/* p = p + c */
			p[0] += u[0];
			if (p[0] < u[0])
			{
				p[1]++;
				if (p[1] == 0)
					k++;
			}
			p[1] += u[1];
			if (p[1] < u[1])
				k++;
			
			/* p = p + w_{i+j} */
			p[0] += w[i+j];
			if (p[0] < w[i+j])
			{
				p[1]++;
				if (p[1] == 0)
					k++;
			}
			if ((i+j) == cpos && carry)
			{	/* catch overflow from last round */
				p[1] += carry;
				if (p[1] < carry)
					k++;
				carry = 0;
			}
			/* w_{i+j} = v, c = u */
			w[i+j] = p[0];
			u[0] = p[1];
			u[1] = k;
		}
		/* 2.3 w_{i+t} = u */
		w[i+t] = u[0];
		/* remember overflow in w_{i+t} */
		carry = u[1];	
		cpos = i+t;
	}
}
/*********************************************************************
Function
	BI_Multiply

Description
	Multiplies two big integers.
	It implements Algorithm 14.12 in the handbook of applied cryptography.

Return
	None.

*********************************************************************/
static void BI_Multiply
(
uint32 	*pu32X,
uint32 	*pu32Y,
uint32 	*pu32Product,
uint32 	u32XSize,
uint32	u32YSize
)
{
	if((pu32X != NULL) && (pu32Y != NULL) && (pu32Product != NULL))
	{
		if((pu32X != pu32Product) && (pu32Y != pu32Product))
		{
			uint32 	u32i,u32k;
			uint32 	u32Carry;
			uint32 	u32P0 = 0,u32P1 = 0;

			/* Initialize */
			M2M_MEMSET(pu32Product, 0, ((u32XSize + u32YSize) * sizeof(uint32)));
			for(u32i = 0; u32i < u32YSize ; u32i ++)
			{
				u32Carry = 0;
				for(u32k = 0; u32k < u32XSize; u32k ++)
				{
					u32P0 = 0;
					u32P1 = 0;
					Multiply32(pu32X[u32k] , pu32Y[u32i] , &u32P0, &u32P1);
					u32P0 +=  pu32Product[u32i + u32k];
					if(u32P0 < pu32Product[u32i + u32k])
						u32P1++;
					u32P0 += u32Carry;
					if(u32P0 < u32Carry)
						u32P1 ++;
					u32Carry =  u32P1;
					pu32Product[u32i + u32k] = u32P0;
				}
				pu32Product[u32i + u32XSize] = u32Carry;
			}
		}
	}
}
/*********************************************************************
Function
	BI_ExpMod

Description
	Computes the expression  <<<<<    Y = (X ^ E) mod N    >>>>>
	using the left to right binary method.

Return
	None.
*********************************************************************/
static void BI_ExpMod
(	
uint32	*pu32X,
uint32	u32XSize,
uint32	*pu32E,
uint32	u32ESize,
uint32	*pu32N,
uint32	u32NSize,
uint32	*pu32Y,
uint32	*pu32Q,
uint32	*pu32Intm
)
{
	/****************************************************************
	* This function computes the Modular Exponentiation for   Y = (X ^ e) % n 	*
	* where Y,X,e and n are Large Integers . 								*
	* The Function uses a method called "LEFT-TO-RIGHT BINARY" Method. 		*
	* It is derived from Algorithm 14.79 (Left-to-right binary exponentiation)		*												
	* The exponent e is represented in binary format as asequence of (t+1) bits.	* 
	* The Algorithm is as follows:										*
	*																*
	*	1- Y = 1;														*	
	*	2- For i from t downto 0 do the following:							*	
	*		2.1- Y <== Y * Y mod n										*
	*		2.2- if (ei == 1) Then  Y <== Y * X mod n.						*	
	*	3- Return Y.													*
	*****************************************************************/
	
	if((pu32X != NULL) && (pu32E != NULL) && (pu32N != NULL) && (pu32Y != NULL) && (pu32Intm != NULL) && (pu32Q != NULL))
	{
		uint32 	u32Exponent;
		uint32 	u32CurrentBit;
		uint32 	u32Counter;
		uint32 	u32IntmSize;
		uint32 	u32TmpSize;
		uint32 	u32BitIdx;
		uint32 	u32BitsCount;
		
		u32IntmSize 	= 2 * MAX(u32XSize, u32NSize);

		/*======*======*======*
			STEP 1 
				Result <== 1		
		*======*======*======*/
		M2M_MEMSET(pu32Y, 0, u32NSize * 4);
		pu32Y[0] = 1;

		/*======*======*======*======*======*======*======*======*
			STEP 2 
				Loop on the exponent bit size and start from the Most Significant bit	
		*======*======*======*======*======*======*======*======*/
		for(u32Counter = u32ESize; u32Counter > 0; u32Counter --)
		{
			u32Exponent = pu32E[u32Counter - 1];
			u32BitsCount	= BITS_PER_DIGIT;
			if(u32Counter == u32ESize)
			{
				while((u32Exponent & BIT31)== 0)
				{
					u32BitsCount --;
					u32Exponent <<= 1;
				}
			}
			for(u32BitIdx = 0 ; u32BitIdx < u32BitsCount ; u32BitIdx ++)
			{
				u32CurrentBit = ((u32Exponent << u32BitIdx) & BIT31);

				/*======*======*======*======*======*
					STEP 2.1
						Result <== Result * Result % Modulus	
				*======*======*======*======*======*/
				u32TmpSize =  BI_Sizeof(pu32Y, u32NSize);
				BI_Square(pu32Y, pu32Intm, u32TmpSize);
				u32TmpSize = BI_Sizeof(pu32Intm, u32IntmSize);
				BI_DIVIDE(pu32Intm, pu32N, pu32Q, pu32Y, u32TmpSize, u32NSize);

				/*======*======*======*======*======*======*======*
					STEP 2.2 
						If current bit is 1 Then  Result <== Result * Base % Modulus		
				*======*======*======*======*======*======*======*/
				if(u32CurrentBit != 0)
				{
					BI_Multiply(pu32Y, pu32X, pu32Intm, BI_Sizeof(pu32Y, u32NSize), BI_Sizeof(pu32X, u32XSize));
					u32TmpSize = BI_Sizeof(pu32Intm, u32IntmSize);
					BI_DIVIDE(pu32Intm, pu32N, pu32Q, pu32Y, u32TmpSize, u32NSize);
					u32TmpSize = 0;
				}
			}
		}
	}
}
/*********************************************************************
Function
	BI_ToBytes

Description 
	Converts a big int to an array of bytes putting the most significant byte 
	of the digit as the first byte in the array.

Return
	None.
*********************************************************************/
void BI_ToBytes
(
uint32 	*pu32BigInt,
uint32 	u32BigIntSize, 
uint8 	*pu8Bytes,
uint32	u32BytesCount
)
{
	uint32		u32ByteIdx;
	sint32		s32WordIdx;

	u32ByteIdx	= 0;
	s32WordIdx	= u32BigIntSize - 1;
	
	M2M_MEMSET(pu8Bytes, 0, u32BytesCount);
	if(u32BytesCount > (u32BigIntSize * 4))
	{
		u32ByteIdx = u32BytesCount - (u32BigIntSize * 4);
	}
	else if((u32BytesCount % 4) != 0)
	{
		uint32	u32Idx;
		uint32	u32ExtraBytes = u32BytesCount % 4;

		for(u32Idx = 0; u32Idx < u32ExtraBytes; u32Idx ++)
		{
            pu8Bytes[u32ByteIdx ++] = (uint8)(pu32BigInt[s32WordIdx] >> ((u32ExtraBytes - u32Idx - 1) * 8));
		}
		s32WordIdx --;
	}
	for(; ((s32WordIdx >= 0) && u32ByteIdx < (u32BytesCount - 3)); s32WordIdx --)
	{
		pu8Bytes[u32ByteIdx ++] = BYTE_3(pu32BigInt[s32WordIdx]);
		pu8Bytes[u32ByteIdx ++] = BYTE_2(pu32BigInt[s32WordIdx]);
		pu8Bytes[u32ByteIdx ++] = BYTE_1(pu32BigInt[s32WordIdx]);
		pu8Bytes[u32ByteIdx ++] = BYTE_0(pu32BigInt[s32WordIdx]);
	}
}
/*********************************************************************
Function
	BI_FromBytes

Description 
Make a big int from an array of bytes assuming the Most significant byte of the
resultant digit is the first byte in the input array (BIG INDIAN).

Return
	None.
*********************************************************************/
void BI_FromBytes
(
uint8 	*pu8Bytes,
uint32 	u32BytesCount, 
uint32 	*pu32BigInt
)
{
	uint32	u32WordIdx;
	uint32	u32ByteIdx = 0;

	u32WordIdx	= ((u32BytesCount + 3) / 4) - 1;
	u32ByteIdx	= 0;

	if((u32BytesCount % 4) != 0)
	{
		uint32	u32Idx,u32ExtraBytes;

		u32ExtraBytes = u32BytesCount % 4;
		pu32BigInt[u32WordIdx] = 0;
		for(u32Idx = 0; u32Idx < u32ExtraBytes; u32Idx ++)
			pu32BigInt[u32WordIdx] |= (uint32)(pu8Bytes[u32ByteIdx ++] << ((u32ExtraBytes - u32Idx - 1) * 8));
		u32WordIdx --;
	}
	for(; u32ByteIdx < u32BytesCount ; u32ByteIdx += 4 , u32WordIdx --)
	{
		pu32BigInt[u32WordIdx] = GETU32(pu8Bytes,u32ByteIdx);
	}
}
/*********************************************************************
Function
	BI_ModExp

Description
	Computes the expression  <<<<<    Y = (X ^ E) mod N    >>>>>
	using the left to right binary method.

Return
	-	BI_SUCCESS 
	-	BI_FAIL

*********************************************************************/
sint32 BI_ModExpSW
(	
uint8	*pu8X, 	uint32	u32XSize,
uint8	*pu8E,	uint32	u32ESize,
uint8	*pu8N,	uint32	u32NSize,
uint8	*pu8Y,	uint32	u32YSize
)
{
	sint32 		s32Ret = BI_FAIL;

	if((pu8X != NULL) && (pu8E != NULL) && (pu8N != NULL) && (pu8Y != NULL))
	{
		uint32	u32ScratchMemIdx;
		uint32	u32XWords, u32NWords, u32EWords;
		uint32	*pu32X, *pu32N, *pu32E, *pu32Y;
		uint32	*pu32Intm,*pu32Q;
		uint32 	u32IntmSize;

		u32XWords	= ((u32XSize + 3) / 4);
		u32EWords	= ((u32ESize + 3) / 4);
		u32NWords	= ((u32NSize + 3) / 4);
		u32IntmSize 	= 2 * MAX(u32XWords , u32NWords);
		
		u32ScratchMemIdx = GET_SCRATCH_MEM_IDX();
		pu32X		= (uint32*)SCRATCH_MEM_ALLOC(u32XWords * 4);
		pu32E		= (uint32*)SCRATCH_MEM_ALLOC(u32EWords * 4);
		pu32N		= (uint32*)SCRATCH_MEM_ALLOC(u32NWords * 4);
		pu32Y		= (uint32*)SCRATCH_MEM_ALLOC(u32NWords * 4);
		pu32Intm	= (uint32*)SCRATCH_MEM_ALLOC(u32IntmSize * 4);
		pu32Q		= (uint32*)SCRATCH_MEM_ALLOC(u32IntmSize * 4);

		/* Convert to words. */
		if((pu32Q != NULL) && (pu32Intm != NULL))
		{
			BI_FromBytes(pu8X, u32XSize, pu32X);
			BI_FromBytes(pu8E, u32ESize, pu32E);
			BI_FromBytes(pu8N, u32NSize, pu32N);

			/* Initialize to zero. */
			M2M_MEMSET(pu32Q, 0, u32IntmSize * 4);
			M2M_MEMSET(pu32Intm, 0, u32IntmSize * 4);

			/* Perform the Modular Exponentiation algorithm. */
			BI_ExpMod(pu32X, u32XWords, pu32E, u32EWords, pu32N, u32NWords, pu32Y, pu32Q, pu32Intm);

			/* Convert the result. */
			BI_ToBytes(pu32Y, u32NWords, pu8Y, u32YSize);
			s32Ret = BI_SUCCESS;
		}
		RESTORE_SCRATCH_MEM_IDX(u32ScratchMemIdx);
	}

	return s32Ret;
}
/*********************************************************************
Function
	BI_ModExp

Description
	Computes the expression  <<<<<    Y = (X ^ E) mod N    >>>>>
	The function selects the core to execute (HW or SW).

Return
	-	BI_SUCCESS 
	-	BI_FAIL
*********************************************************************/
sint32 BI_ModExp
(	
uint8	*pu8X,	uint32	u32XSize,
uint8	*pu8E,	uint32	u32ESize,
uint8	*pu8N,	uint32	u32NSize,
uint8	*pu8Y,	uint32	u32YSize
)
{
	sint32	s32Ret = BI_FAIL;

	s32Ret = BI_MOD_EXP_SW(pu8X, u32XSize, pu8E, u32ESize, pu8N, u32NSize, pu8Y, u32YSize);
	return s32Ret;
}
