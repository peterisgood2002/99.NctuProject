%處理Fourier Transformation
%輸入：image array和mask array
%輸出：fourier image和fourier mask
function [FDA2 ,FDM2] = freTrans(DA2 ,DM2 ,mode)
    [row col] = size( DA2 );
    [r c ] = size( DM2 );
    P = row + r - 1;
    Q = col + c - 1;
    
    TDA2 = zeros ( row ,col ,'double' );
    for (  i = 1 : row )
        for ( j = 1 : col )
            TDA2 ( i , j ) =   double ( DA2 ( i , j ) ) * (-1) .^(i+j) ;
        end
    end
    TM2 = fft2 (TDA2 ,row ,col );
    TM2 =  sqrt ( real(TM2) .* real(TM2) + imag(TM2) .* imag(TM2) ) ;
    
    
    TM2 = uint8( log (1+abs(TM2) ) );
    output ( TM2 ,'.\result\2\fourier image without zero padding.bmp' );
    %imwrite (  TM2 ,'.\result\2\fourier image without zero padding.bmp' );
    
    FTDA2 = zeros (P ,Q ,'double');%fourier transformation的結果
    FTDM2 = zeros (P ,Q ,'double');
    FTDA2 (1 : row , 1 : col) = DA2 ( 1 : row , 1 : col );
    FTDM2 ( 301 : 300 + r ,  301 :  300 + c ) = DM2 (  1 :  r ,  1 :  c );
%     TA2 = uint8 (FTDA2);
%     imwrite ( TA2 ,'.\result\2\image with zero padding.bmp');
    %進行fourier transformation
    if ( mode == 1 )
        for (  i = 1 : P )
            for ( j = 1 : Q )
                a = (-1) .^(i+j);
                FTDA2 ( i , j ) = FTDA2 ( i , j ) * ( a );
                FTDM2 ( i , j ) = FTDM2 ( i , j ) * ( a );
            end
        end
    end
%     TA2 = uint8 (FTDA2);
%     imwrite ( TA2 ,'.\result\2\image with zero padding and multiply.bmp');
    
    FDA2 = fft2 (FTDA2 ,P ,Q);
%     TA2 = uint8( ifft2(FDA2) );
%     imwrite (TA2 ,'.\result\2\test.bmp');
    FDM2 = fft2 (FTDM2 ,P ,Q);
    %get the image part of mask
    FDM2 = imag (FDM2);
    output (FDM2 ,'.\result\2\test.bmp');
    FDM2 = complex( 0 , FDM2);
    for (  i = 1 : P )
        for ( j = 1 : Q )
            a = (-1) .^(i+j);
            FDM2 ( i , j ) = FDM2 ( i , j ) * ( a );
        end
    end
end