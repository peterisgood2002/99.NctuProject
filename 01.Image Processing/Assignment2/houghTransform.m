%進行Hough Transform計算
%θ界於-90 ~ 90 ρ界於-對角線~正對角線之間
%     ------------->θ
%     |
%     |
%     |
%    \|/ρ 
%parameter coordinate的直線表示法為 ρ = x * cosθ + y * sinθ
%我們必須尋找local maximum才能達到直線不接近的狀況
%而我們也知道最大值級極大值 因此我們每次皆找最大值 
%並且設下threshold將最大值 將最大值附近的點全部mask起來 以免我們找到過於接近
%輸入：一張已經決定好edge而且只有兩種顏色的圖 想找出多少條線
%輸出：相對應的線的(ρrho ,θ theta)值
function [RHO1 ,THETA1] = houghTransform ( iG2 , nol)
    %共有nol條線
    RHO1 = zeros ( 1 ,nol ,'double');
    THETA1 = zeros ( 1 ,nol , 'double' );
    
    %計算Accumulator cell
    [row col] = size( iG2 );
    bright = max( max(iG2) );
    rho = ceil ( sqrt( row^2 + col^2 ) );
    theta = 90;
    AC = calAC(iG2);
    
    %找尋Local maximum
    %在parameter coordinate中 找nol次最大值
    i = 1;
    while ( i <= nol )
        %為了加快速度 我們使用陣列處理來尋找最大值
        %先找每一row當中的最大值
        [M1 R1] = max( AC );
        %再透過拿到的每一row最大值找尋每一col最大值
        [m c] = max( M1 );
        r = R1(1 ,c);
        %將最大值附近的點設為0
        AC = mLocalMax(AC , r ,c );
        
        r = r - rho - 1;
        c = c - theta - 1;

        RHO1(1 ,i) = r;%還原
        THETA1(1 ,i) = c;

        i = i + 1;
             
    end
   
    
%計算Accumulator Cells 因為語言限制 因此我們將其normalized到0 ~2*rho
%輸入：圖
%輸出：Accumulator cells所產生的matric
function AC = calAC (iG2)
    AC = zeros( 2*rho+1 ,2*theta+1 , 'uint16' );
    %計算每一個亮點的可能直線值
    for ( x = 1 : row )
        for ( y = 1 : col )
            if ( iG2(x ,y) == bright )
                for ( t = -theta : theta )%每次增加1個角度
                    tt = pi * t / 180;
                    r = x * cos(tt) + y * sin(tt);
                 
                    if ( r >= -rho && r <= rho )%在界線內的才應該算入array中
                        i = uint16( r +rho+ 1 );%array從1開始算起
                        j = t + theta + 1;
                    
                        AC( i , j ) = AC( i , j )+1;%相對應值++
                    end
                end
            end
        end
    end
    imwrite (uint8(AC) ,'.\result\accumulator cell test.bmp');
end
%calAC function結束


end
%houghTransform function結束

%local maximum附近全部變0
%傳入：accumulator cell 座標
%傳出：被mask後的accumulator cell
function AC = mLocalMax( AC ,  r, c)
    [ row col ] = size( AC );
    ts = 3;
    
    s = 20;
    for ( x = r-s : r+s )
        if ( x >= 1 && x <= row )
            for ( y = c-ts : c+ts )
                if ( y >= 1 && y <= col )
                    AC ( x ,y ) = 0;
                end
            end
        end
    end
end



