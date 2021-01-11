%將會進行Correlation operation並且得到新的Array
%S = sum ( M(i,j)*A(i,j) )
%輸入：image array 和 mask
%輸出：sum of product的結果
function DS2 = correlationOP (DA2 ,DM2)
     [rmask cmask] = size(DM2);
     [row col] = size(DA2);
     
     DS2 = zeros(row ,col ,'double');%產生一個全是0的matrix
     for ( i = 1 : row )
         for ( j = 1 :col )
             %以A[i,j]為中心產生跟mask一樣大小的subarray
             DTA2 = subArray(DA2 ,i ,j ,rmask ,cmask );
             DTA2 = DTA2 .* DM2;%將相對應元素相乘
             DS2(i ,j) = sum ( sum(DTA2) );%將結果累加起來存於S中
         end
     end
     
%correlationOP結束

%找尋Array中的sub array 若超出範圍則以0取代
%傳入：Array 中心點 以及subarray大小
%傳出：subarray
function DSA2 = subArray ( DA2 , i , j ,row ,col )
    [arow acol] = size(DA2);
    
    DSA2 = zeros(row ,col ,'double');
    
    ti = ceil( i - (row / 2) );
    for ( k = 1 :row )
        tj = ceil( j - (col / 2) );
        for (l = 1: col)
            if ( ti<1 || ti>arow || tj<1 || tj>acol )%進行邊界處理
                DSA2(k ,l) = DA2(i,j);%如果超出邊界就讓他是中心點的值
            else
                DSA2(k ,l) = DA2(ti ,tj);
            end
            tj = tj+1;
        end
        ti = ti+1;
    end
            
end

end

