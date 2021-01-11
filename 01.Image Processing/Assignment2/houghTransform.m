%�i��Hough Transform�p��
%�c�ɩ�-90 ~ 90 �l�ɩ�-�﨤�u~���﨤�u����
%     ------------->�c
%     |
%     |
%     |
%    \|/�l 
%parameter coordinate�����u��ܪk�� �l = x * cos�c + y * sin�c
%�ڭ̥����M��local maximum�~��F�쪽�u�����񪺪��p
%�ӧڭ̤]���D�̤j�ȯŷ��j�� �]���ڭ̨C���ҧ�̤j�� 
%�åB�]�Uthreshold�N�̤j�� �N�̤j�Ȫ����I����mask�_�� �H�K�ڭ̧��L�󱵪�
%��J�G�@�i�w�g�M�w�nedge�ӥB�u������C�⪺�� �Q��X�h�ֱ��u
%��X�G�۹������u��(�lrho ,�c theta)��
function [RHO1 ,THETA1] = houghTransform ( iG2 , nol)
    %�@��nol���u
    RHO1 = zeros ( 1 ,nol ,'double');
    THETA1 = zeros ( 1 ,nol , 'double' );
    
    %�p��Accumulator cell
    [row col] = size( iG2 );
    bright = max( max(iG2) );
    rho = ceil ( sqrt( row^2 + col^2 ) );
    theta = 90;
    AC = calAC(iG2);
    
    %��MLocal maximum
    %�bparameter coordinate�� ��nol���̤j��
    i = 1;
    while ( i <= nol )
        %���F�[�ֳt�� �ڭ̨ϥΰ}�C�B�z�ӴM��̤j��
        %����C�@row�����̤j��
        [M1 R1] = max( AC );
        %�A�z�L���쪺�C�@row�̤j�ȧ�M�C�@col�̤j��
        [m c] = max( M1 );
        r = R1(1 ,c);
        %�N�̤j�Ȫ����I�]��0
        AC = mLocalMax(AC , r ,c );
        
        r = r - rho - 1;
        c = c - theta - 1;

        RHO1(1 ,i) = r;%�٭�
        THETA1(1 ,i) = c;

        i = i + 1;
             
    end
   
    
%�p��Accumulator Cells �]���y������ �]���ڭ̱N��normalized��0 ~2*rho
%��J�G��
%��X�GAccumulator cells�Ҳ��ͪ�matric
function AC = calAC (iG2)
    AC = zeros( 2*rho+1 ,2*theta+1 , 'uint16' );
    %�p��C�@�ӫG�I���i�ઽ�u��
    for ( x = 1 : row )
        for ( y = 1 : col )
            if ( iG2(x ,y) == bright )
                for ( t = -theta : theta )%�C���W�[1�Ө���
                    tt = pi * t / 180;
                    r = x * cos(tt) + y * sin(tt);
                 
                    if ( r >= -rho && r <= rho )%�b�ɽu�����~���Ӻ�Jarray��
                        i = uint16( r +rho+ 1 );%array�q1�}�l��_
                        j = t + theta + 1;
                    
                        AC( i , j ) = AC( i , j )+1;%�۹�����++
                    end
                end
            end
        end
    end
    imwrite (uint8(AC) ,'.\result\accumulator cell test.bmp');
end
%calAC function����


end
%houghTransform function����

%local maximum���������0
%�ǤJ�Gaccumulator cell �y��
%�ǥX�G�Qmask�᪺accumulator cell
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



