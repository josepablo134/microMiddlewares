#include <System/DSP/LTISystem/LTISystem.h>

void LTITransferInit( LTITransfer_t* system,
								const var* num,
								const var* den,
								uint32_t numOrder,
								uint32_t denOrder,
								var* xdelay,
								var* ydelay){
	system->den = den;
	system->num = num;
	system->denOrder = denOrder;
	system->numOrder = numOrder;
	system->xdelay = xdelay;
	system->ydelay = ydelay;

	/// Set initial conditions
	// numOrder <= denOrder
	uint32_t k;
	for( k=0; k<numOrder ; k++ ){
		system->xdelay[ k ] = \
		system->ydelay[ k ] = 0.0;
	}
	for(; k<denOrder ; k++ ){
		system->ydelay[ k ] = 0.0;
	}
}

var  LTITransferCompute( LTITransfer_t* system , var input){
	uint32_t k;
	var tempOut = input*system->num[0];
	/// Compute new output
	// numOrder <= denOrder
	for(k=0; k<system->numOrder; k++){
		tempOut += system->xdelay[ k ] * system->num[ k+1 ];
		tempOut -= system->ydelay[ k ] * system->den[ k+1 ];
	}
	for(; k<system->denOrder; k++){
		tempOut -= system->ydelay[ k ] * system->den[ k+1 ];
	}
	tempOut /= system->den[0];

	/// Shift delay registers
	for( k=system->denOrder-1; k > system->numOrder; k-- ){
		system->ydelay[ k ] = system->ydelay[ k-1 ];
	}
	for( ; k > 0 ; k-- ){
		system->xdelay[ k ] = system->xdelay[ k-1 ];
		system->ydelay[ k ] = system->ydelay[ k-1 ];
	}
	if( system->xdelay ){
		system->xdelay[0] = input;
	}
	system->ydelay[0] = tempOut;
	return tempOut;
}
