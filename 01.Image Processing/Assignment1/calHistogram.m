%計算一個圖片的Histogram
%傳入二維陣列 以及他可表示的顏色數值
%傳出Vector儲存0-255相關資訊 因為要做調整 因此我們使用1-256分別代表0-255
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