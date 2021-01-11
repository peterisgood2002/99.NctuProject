%對原始檔進行Histogram equalization
%得到 T :r--> S對應
% sr = T(r) = (L-1) /MN * Rsum(0 ,k) 
% r之index為1-256  分別表示 0-255 S之內容表示為T(r) +1 以做統一性
%傳入：這張圖的相關統計資料 總pixel數
function S1 = histEQ( R1 , mn, cl)
    if ( isvector(R1) )%只有在他是灰階圖且是vector的情況下才能夠使用此function
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
       S1 = S1 + ones(1 , cl ,'uint32');%將所有值加1以從0-255存至1-256
    end



end