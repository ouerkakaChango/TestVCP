#include "XLine.h"
#include "KCommon.h"
void XLine::Print() {
	if (isA) {
		cout << "\nXLine: y=" << A;
	}
	else {
		cout << "\nXline: x=" << k << "y+(" << t<<")";
	}
}
