%將處理rgb轉成grey
function G2 = rgbToGray ( I3 )
   [row col color] = size(I3);
   G2 = zeros(row ,col ,'double');
   for ( i = 1: row )
       for (j = 1 : col )
           G2(i ,j) = (  I3(i ,j ,1) +I3(i ,j ,2) + I3(i ,j ,3) )/3;
       end
   end