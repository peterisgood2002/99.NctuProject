%�Ψӿ�Xfourier transformation���Ϥ���
function output (IM2 ,filename) 
    maximum = max ( max (IM2) );
    minimum = min ( min (IM2) );
    interval = maximum - minimum;
    
    [row col] = size ( IM2 );
    OM2 = zeros ( row ,col , 'double' );
    for ( i = 1 : row )
        for ( j = 1 : col ) 
            a = ( IM2 ( i , j ) - minimum ) / interval;
            OM2( i , j ) =  a * 255; 
        end
    end
    
    %�Npixel�����ܷQ�n���϶�
    
    imwrite (OM2 ,filename);
    
   
end