/******************************************
FILE
    Kernel.cpp
PURPOSE
    �ΨӲ��ͤ@��Kernel�� class
	�аO�� ��class���Ҧ��ݭnKerenl �Ҧp�GGaussain Kernel�άOGabor Kernel���̤j���X

NOTE
	

AUTHOR
    9757553 �C����
*******************************************/

#include "Kernel.h"

Kernel::Kernel( int row ,int col ,double data ) : kernel( row ,col ,data) {

}
int 
Kernel::getRowSize() const {
	return kernel.getRowCount();
}
int 
Kernel::getColSize() const {
	return kernel.getColCount();
}
/*Get the kernel it'll replicate the data */
float* * 
Kernel::getKernel() const {
	return kernel.getDatas();
}

ostream& 
operator<< (ostream& out ,const Kernel& v ) {
	out << v.kernel;

	return out;
}