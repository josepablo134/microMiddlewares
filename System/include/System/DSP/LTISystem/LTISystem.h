/*!
*	@file		LTISystem.h
*	@version	0.0
*	@author		Josepablo Cruz Baas
*	@date		09/07/2020
*	@brief		LTISystem declarations.
*	
*	\f[
*		\frac{Y(z)}{X(z)} = \frac{ b0 + (b1) z^{-1} + ... + (bm) z^{-m} }{ a0 + (a1) z^{-1} + ... + (an) z^{-n} }
*	\f]
*
*	with m <= n
*	Where m is the numerator order and n is the denominator order.
*	
*	Example of use:
*	@code{.c}
*		/// Snippet code for a sensor filtering system
*		#define filterOrder N
*		const var		num[ filterOrder+1 ] = { ... };
*		const var		den[ filterOrder+1 ] = { ... };
*		static var		xdelay[ filterOrder ];
*		static var		ydelay[ filterOrder ];
*		LTITransfer_t	filter;
*		var				dataIn,dataOut;
*		LTITransferInit( &filter , num , den , filterOder , filterOder , xdelay , ydelay );
*		while( true ){
*			dataIn = sensorRead(  );
*			dataOut = LTITransferCompute( &filter , dataIn  );
*			delay( SAMPLE_TIME );
*		}
*	@endcode
**/
#ifndef LTISYSTEM_H_
#define LTISYSTEM_H_

#ifdef __cplusplus
	extern "C" {
#endif

    #include <stdint.h>

	typedef float	var;
	
	/*!
	*	@struct LTITransfer_t
	*	@brief Representation of a transfer function through numerator and denominator coefficients.
	*	@param numOrder numerator order (number of elements)
	*	@param denOrder denominator order (number of elements)
	*	@param *num numerator coefficients of the transfer function with length equal to numOrder+1
	*	@param *den denominator coefficients of the transfer function with length equal to denOrder+1
	*	@param *xdelay delay vector of the input signal with length equal to numOrder
	*	@param *ydelay delay vector of the output signal with length equal to denOrder
	**/
	typedef struct LTITransfer_t{
		uint32_t	numOrder;
		uint32_t	denOrder;
		const var	*num;
		const var   *den;
		var         *xdelay;
		var			*ydelay;
	}LTITransfer_t;

	/*!
	*	@brief	Initialize the transfer function object.
	*
	*	@param	*system pointer to LTITransfer_t structure.
	*	@param	*num numerator coefficients of the transfer function.
	*	@param	*den denominator coefficients of the transfer function.
	*	@param	numOrder order of the numerator transfer function.
	*	@param	denOrder order of the denominator transfer function.
	*	@param	*xdelay delay vector of the input signal.
	*	@param	*ydelay delay vector of the output signal.
	*	@return nothing.
	**/
	extern void LTITransferInit( LTITransfer_t* system,
										const var* num,
										const var* den,
										uint32_t numOrder,
										uint32_t denOrder,
										var* xdelay,
										var* ydelay);
	/*!
	*	@brief	Compute a value in the transfer function.
	*
	*	@param	*system pointer to already initialized LTITransfer_t structure.
	*	@param	input value to compute through the filter.
	*	@return var result of the filter computation.
	**/
    extern var  LTITransferCompute( LTITransfer_t* system , var input);

#ifdef __cplusplus
	}
#endif

#endif
