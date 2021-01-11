%運用image gradient進行Edge Detection動作
%透過Sobel operator進行垂直 水平 以及45度角 和-45度角的edge
%傳入：一張圖
%傳出：偵測Edge過後的圖 而且其內不值不是0就是255
function iG2 =edgeDetction ( A3 ) 
    HM = [-1 -2 -1;
           0  0  0;
           1  2  1];%判斷水平線
    VM = [-1  0  1;
          -2  0  2;
          -1  0  1 ];
    [row col color] = size( A3);
    dTGM2 = zeros(row ,col ,'double');%gradient magnitude = |dR| + |dG| +|dB|
    for ( i = 1 : 3 )
        dHM2 = correlationOP( A3(: ,: ,i) ,HM );
        dVM2 = correlationOP( A3(: ,: ,i) ,VM );
        %str = strcat ( '.\result\90\gx' , int2str(i) ,'.bmp');
        %imwrite( uint8(dHM2) , str );
        %str = strcat ( '.\result\90\gy' , int2str(i) ,'.bmp');
        %imwrite( uint8(dVM2) , str );
        dGM2 = abs(dHM2) + abs(dVM2);
        %str = strcat ( '.\result\90\gradienthv' , int2str(i) ,'.bmp');
        %imwrite( uint8(dGM2) , str );
        dTGM2 = dTGM2 + abs( dGM2 );
    end
    %imwrite (dTGM2 ,'.\result\90\gradient.bmp');
    t = findThreshold(dTGM2);
    iG2 = addThreshold (dTGM2 , 620);
    imwrite (iG2 ,'.\result\90\gwiththeshold.bmp');
    
%edgeDetction function結束

%將Gradiant加上threshold
%傳入：Gradiant map threshold value
%傳出：只有黑白的圖
function iOG2 = addThreshold ( dIG2 , threshold )
    [row col] = size( dIG2 );
    iOG2 = zeros(row ,col ,'uint8');
    for ( i = 1 : row )
        for ( j = 1 : col )
            if ( dIG2( i ,j) > threshold ) 
                iOG2 (i ,j) = 255;
            end
        end
    end
end
    
%addThreshold function結束        
end