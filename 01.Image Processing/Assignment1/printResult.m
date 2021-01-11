function printResult(I3)
 [row col color] = size(I3);
 M3 = zeros( size(I3) ,'uint8');
 for i = 1:row
     for j=1:col
         for k= 1 :color
             tmp = I3(i ,j ,k);
             if  tmp == 257 
                 M3(i ,j , k)= 255;
             else M3(i ,j ,k) =tmp;
             end
         end
     end
 end
 imwrite(M3 ,'2.jpg');
end