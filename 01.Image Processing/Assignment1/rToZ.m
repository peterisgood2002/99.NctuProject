% R to Z ��r -z���ഫ
%�ǤJ�GS ,G(z) level
function Z = rToZ(  S1 ,GofZ1 ,cl)
   Z = zeros( 1 ,cl );
   for ( r = 1 : cl)
       s = S1( 1, r);
       Z( 1 ,r) = Ginverse(s ,GofZ1 ,cl); % z = G-1(s) s-->z z�Ȭ�1-256
      
   end
end