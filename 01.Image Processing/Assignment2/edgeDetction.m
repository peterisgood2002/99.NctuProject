%�B��image gradient�i��Edge Detection�ʧ@
%�z�LSobel operator�i�櫫�� ���� �H��45�ר� �M-45�ר���edge
%�ǤJ�G�@�i��
%�ǥX�G����Edge�L�᪺�� �ӥB�䤺���Ȥ��O0�N�O255
function iG2 =edgeDetction ( A3 ) 
    HM = [-1 -2 -1;
           0  0  0;
           1  2  1];%�P�_�����u
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
    
%edgeDetction function����

%�NGradiant�[�Wthreshold
%�ǤJ�GGradiant map threshold value
%�ǥX�G�u���¥ժ���
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
    
%addThreshold function����        
end