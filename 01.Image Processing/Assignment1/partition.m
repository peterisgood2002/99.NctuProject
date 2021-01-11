%將兩張圖做比較並且分開資料 以0表示不輸出的部分 
%傳入：原始圖 mask 和以什麼顏色做mask 
%傳出：分開的圖 內部若有值為257表示被切開的區域
function R3 = partition( SM3 ,MM3 ,Color)
    [row col color] = size(SM3);
    R3 = zeros(  size(SM3) );
    for i = 1 : row
        for j=1 :col
            for k = 1 :color
                source = SM3(i ,j ,k);
                if ( MM3(i ,j ,1) == Color(1 ,1) && MM3(i ,j ,2) == Color(1 ,2) && MM3(i,j,3) ==Color(1,3) )
                    R3 (i ,j ,k) = source;
                else R3(i ,j ,k) = -1;
                end
            end
        end
    end
end