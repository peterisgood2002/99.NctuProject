%�N�|�i��Correlation operation�åB�o��s��Array
%S = sum ( M(i,j)*A(i,j) )
%��J�Gimage array �M mask
%��X�Gsum of product�����G
function DS2 = correlationOP (DA2 ,DM2)
     [rmask cmask] = size(DM2);
     [row col] = size(DA2);
     
     DS2 = zeros(row ,col ,'double');%���ͤ@�ӥ��O0��matrix
     for ( i = 1 : row )
         for ( j = 1 :col )
             %�HA[i,j]�����߲��͸�mask�@�ˤj�p��subarray
             DTA2 = subArray(DA2 ,i ,j ,rmask ,cmask );
             DTA2 = DTA2 .* DM2;%�N�۹��������ۭ�
             DS2(i ,j) = sum ( sum(DTA2) );%�N���G�֥[�_�Ӧs��S��
         end
     end
     
%correlationOP����

%��MArray����sub array �Y�W�X�d��h�H0���N
%�ǤJ�GArray �����I �H��subarray�j�p
%�ǥX�Gsubarray
function DSA2 = subArray ( DA2 , i , j ,row ,col )
    [arow acol] = size(DA2);
    
    DSA2 = zeros(row ,col ,'double');
    
    ti = ceil( i - (row / 2) );
    for ( k = 1 :row )
        tj = ceil( j - (col / 2) );
        for (l = 1: col)
            if ( ti<1 || ti>arow || tj<1 || tj>acol )%�i����ɳB�z
                DSA2(k ,l) = DA2(i,j);%�p�G�W�X��ɴN���L�O�����I����
            else
                DSA2(k ,l) = DA2(ti ,tj);
            end
            tj = tj+1;
        end
        ti = ti+1;
    end
            
end

end

