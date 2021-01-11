%1.	Give an algorithm and its implementation for the above image processing
%   in the spatial domain.
%2.	Give an algorithm and its implementation for the above image convolution operation 
%   using the Fourier transform approach with zero padding.
%3. Give an algorithm and its implementation for the above image
%   convolution operation using the Fourier transform approach with zero padding, but without centering

clear;

SM2 = [-1 0 1;
       -2 0 2;
       -1 0 1;];
IM2 = imread('.\data\data.tif');

% %correlation
% CM2 = correlationOP(IM2 ,SM2);
% CM2 = uint8(CM2);
% imwrite(CM2 ,'.\result\1\correlate.bmp');
% 
% %Convolution
% SMC2 = [1 0 -1;
%         2 0 -2;
%         1 0 -1;];
% CM2 = correlationOP(IM2 ,SMC2);
% CM2 = uint8(CM2);
% imwrite(CM2 ,'.\result\1\covolution.bmp');

%2.Fourier Transformation with zero padding
% mode = input('請問您想看哪一種Fourier Transformation(0)with zero padding (1)with zero padding but without centering');%需要centering
mode = 0;
[FDA2 ,FDM2] = freTrans (IM2 ,SM2 ,mode);
%取spectrum
TM2 = sqrt ( real(FDA2) .* real(FDA2) + imag(FDA2) .* imag(FDA2) );
TM2 = uint8( log (1+abs(TM2) ) );
output(TM2 ,'.\result\2\image spectrum.bmp');
TM2 = sqrt ( real(FDM2) .* real(FDM2) + imag(FDM2) .* imag(FDM2) );
TM2 = uint8( log (1+abs(TM2) ) );
output(TM2 ,'.\result\2\mask spectrum.bmp');

TA2 = uint8( ifft2(FDA2) );
imwrite (TA2 ,'.\result\2\test.bmp');

FIM2 = FDA2 .* FDM2 ;
output (FIM2 ,'.\result\2\convolution.bmp');

[row col] = size ( FIM2 );
iFIM2 = ifft2 ( FIM2 ,row ,col );
if ( mode == 1 )
    for (  i = 1 : row )
       for ( j = 1 : col )
           a = (-1) .^(i+j);
           iFIM2 ( i , j ) =  real(iFIM2 ( i , j ))  * ( a );
       end
    end
end
[row col] = size ( IM2);
% iFIM2 = iFIM2(1 : row ,1 : col);
iFIM2 = uint8 (iFIM2);
imwrite (iFIM2 ,'.\result\2\result image.bmp');



