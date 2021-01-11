%找到threshold 根據課本P.742方法找到threshold點 邊界部份不要處理
%傳入：一張圖
%傳出：找到該張圖的Threshold資料
function t = findThreshold( dDA2 )
   [row col] = size(dDA2);
   %找intensity最大和最小
   maximum = max ( max(dDA2) );
   minimun = min (min(dDA2) );
   deltat = 1;
   t = ( maximum + minimun ) / 2;
   pret = 0;
   while ( abs(t - pret) > deltat ) %小於deltat時即可跳出迴圈
       pret = t;
       
       %將圖分成兩塊 一塊> Threshold 另一塊 <Threshold
       %並計算他們的intensity values
       total1 = 0; total2 = 0;
       n1 = 0; n2 = 0;%兩塊各自的數量
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