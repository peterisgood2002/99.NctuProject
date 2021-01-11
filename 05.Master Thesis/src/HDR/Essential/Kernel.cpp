/******************************************
FILE
    Kernel.cpp
PURPOSE
    用來產生一個Kernel的 class
	請記住 此class為所有需要Kerenl 例如：Gaussain Kernel或是Gabor Kernel的最大集合

NOTE
	

AUTHOR
    9757553 顏志晟
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