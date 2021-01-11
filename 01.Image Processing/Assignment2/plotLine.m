%�N33�����u�e�b�ϤW 
%��J�G33�����u�� (�l ,�c ) ��
%�l = x * cos�c + y * sin�c
%��X�G�ӹ�
function O2 = plotLine( RHO1 ,THETA1 ,iG2 )
    [a line] = size(RHO1);
    [ row col ] = size(iG2);
    O2 = zeros(row ,col ,'uint8');
    light = 255;
    for ( i = 1 : line )
        r = RHO1( 1 ,i );
        t = THETA1(1 ,i);
        tt = pi * t / 180;
        
        %���i�� �p�G���b�u�q�W �h�]���զ�
        for ( x = 1 : row )
                if ( t == 0 )%�B�z�����u������
                    %�z�L4-connected�N�u��
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

%�s���u�q
%�ǤJ�G�� �n�s�����I����
%�ǥX�G��
function O2 =ConnectedLine ( O2 ,x ,y )
    if ( y <= col )
        %�z�L4-connected�N�u��
        if ( x-1 >= 1 )%�W��
            O2( x-1 , y ) = light;
        end
        if ( x+1 <= row )%�U��
            O2( x+1 ,y ) = light;
        end
        if ( y-1 >= 1)%����
            O2( x , y-1 ) = light;
        end
        if ( y+1 <=col )%�k��
            O2( x , y+1 ) = light;
        end
        
        O2( x , y ) = light;
        
    end
end
%ConnectedLine function����

end

