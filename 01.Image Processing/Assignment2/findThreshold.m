%���threshold �ھڽҥ�P.742��k���threshold�I ��ɳ������n�B�z
%�ǤJ�G�@�i��
%�ǥX�G���ӱi�Ϫ�Threshold���
function t = findThreshold( dDA2 )
   [row col] = size(dDA2);
   %��intensity�̤j�M�̤p
   maximum = max ( max(dDA2) );
   minimun = min (min(dDA2) );
   deltat = 1;
   t = ( maximum + minimun ) / 2;
   pret = 0;
   while ( abs(t - pret) > deltat ) %�p��deltat�ɧY�i���X�j��
       pret = t;
       
       %�N�Ϥ������ �@��> Threshold �t�@�� <Threshold
       %�íp��L�̪�intensity values
       total1 = 0; total2 = 0;
       n1 = 0; n2 = 0;%����U�۪��ƶq
       for ( i = 1 : row )
           for ( j=1 : col )
               if ( dDA2(i ,j) > t) 
                   total1 = total1 + dDA2(i,j);
                   n1 = n1 + 1;
               else
                   total2 = total2 +dDA2(i,j);
                   n2 = n2 + 1;
               end
           end
       end
       
       t = ( ( total1 / n1 ) + ( total2 /n2 ) )/2;
   end
end