%�p��@�ӹϤ���Histogram
%�ǤJ�G���}�C �H�ΥL�i��ܪ��C��ƭ�
%�ǥXVector�x�s0-255������T �]���n���վ� �]���ڭ̨ϥ�1-256���O�N��0-255
function V1 = calHistogram(A2 ,color)
      if ( ndims(A2) == 2)
          [row col] = size(A2);
          V1 = zeros(1,color );
          for ( i = 1: row )
              for j = 1:col
                  if ( A2(i,j) <=color &&  A2(i ,j) ~= -1)
                      tmp = uint8( A2(i,j) ) +1;% 0-255 -->1-256
                      V1( 1, tmp ) = V1( 1 ,tmp ) +1;
                  end
              end
          end
      end