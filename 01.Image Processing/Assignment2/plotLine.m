%將33條直線畫在圖上 
%輸入：33條直線的 (ρ ,θ ) 圖
%ρ = x * cosθ + y * sinθ
%輸出：該圖
function O2 = plotLine( RHO1 ,THETA1 ,iG2 )
    [a line] = size(RHO1);
    [ row col ] = size(iG2);
    O2 = zeros(row ,col ,'uint8');
    light = 255;
    for ( i = 1 : line )
        r = RHO1( 1 ,i );
        t = THETA1(1 ,i);
        tt = pi * t / 180;
        
        %對整張圖 如果有在線段上 則設成白色
        for ( x = 1 : row )
                if ( t == 0 )%處理垂直線的部分
                    %透過4-connected將線填滿
                    O2 = ConnectedLine(O2 ,x ,r+1);                     
                else
                    y = r / sin(tt) - x * cos(tt) / sin (tt);
                    y = uint16(y);
                    y = y + 1;
                    O2 = ConnectedLine(O2 ,x ,y);
                end
        end
        %str = strcat('.\result\line\',int2str(i) ,'.bmp');
        %imwrite (O2 ,str);
    end
    str = strcat('.\result\result0.bmp');
    imwrite (O2 ,str);
    O2 = bitand(O2 ,iG2); 
    str = strcat('.\result\result1.bmp');
    imwrite (O2 ,str);

%連結線段
%傳入：圖 要連結的點為何
%傳出：圖
function O2 =ConnectedLine ( O2 ,x ,y )
    if ( y <= col )
        %透過4-connected將線填滿
        if ( x-1 >= 1 )%上面
            O2( x-1 , y ) = light;
        end
        if ( x+1 <= row )%下面
            O2( x+1 ,y ) = light;
        end
        if ( y-1 >= 1)%左邊
            O2( x , y-1 ) = light;
        end
        if ( y+1 <=col )%右邊
            O2( x , y+1 ) = light;
        end
        
        O2( x , y ) = light;
        
    end
end
%ConnectedLine function結束

end

