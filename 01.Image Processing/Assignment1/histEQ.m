%���l�ɶi��Histogram equalization
%�o�� T :r--> S����
% sr = T(r) = (L-1) /MN * Rsum(0 ,k) 
% r��index��1-256  ���O��� 0-255 S�����e��ܬ�T(r) +1 �H���Τ@��
%�ǤJ�G�o�i�Ϫ������έp��� �`pixel��
function S1 = histEQ( R1 , mn, cl)
    if ( isvector(R1) )%�u���b�L�O�Ƕ��ϥB�Ovector�����p�U�~����ϥΦ�function
       S1 = zeros ( 1 ,cl);
      
       for ( i =1 : cl )
           sum = 0;
           for (k = 1 : i)
               sum = sum + R1(1 ,k);
           end
           S1( 1 ,i) = (cl-1 )/mn * sum;
           
       end
       %round of int
       S1 = uint32(S1);
       S1 = S1 + ones(1 , cl ,'uint32');%�N�Ҧ��ȥ[1�H�q0-255�s��1-256
    end



end