%主程式進入點 以下是此程式相關設定
%1.此程式相關變數命名學 
%  (1)陣列變數名稱後面一定會接有數字 以方便辨識此變數到底為3*3的array或2*2的array
%     陣列變數一定大寫而大寫前面表示他的資料型態
%  (2)若是檔案名稱的變數 變數名稱內一定會有name
%  (3)純數字變數名稱將不會在後面有任何的name或數字出現
%  (4)變數中相關字元的意義
%     I：input  M：matrix或mask H：histogram
%  (5)角度以degree表示
%2.進行Edge Detection 有三個步驟
%  (1)Image smoothing for noise reduction
%  (2)Detection of edge points
%  (3)Edge Localiztion

clear;
iname = '.\data\data.bmp';

%讀檔
iIM3 = imread(iname);


%(1)Image smoothing for noise reduction
%smooth mask
SM = [ 1 1 1;
       1 1 1;
       1 1 1];
SM = SM * (1/9);
%對RGB每一個matrix做smoothing
for ( i = 1 : 3)
    dSM3( : ,: ,i) = correlationOP( iIM3( : ,: ,i) ,SM );
    %將smooth結果寫出
    %str = strcat('.\result\smooth\' ,int2str(i) ,'.bmp');
    %imwrite( uint8( dSM3(: ,: ,i) ) ,str );
end
imwrite ( uint8(dSM3) ,'.\result\smooth\smooth.bmp');
%(2)Detection of edge points：運用image gradient找邊線
iG2 = edgeDetction(dSM3);

%(3)Edge Localiztion：進行 Hough Transformation 找edge的直線
nol = 40;
[R1 ,T1] = houghTransform ( iG2 , nol);

%(4)對每一條線畫線
O2 = plotLine( R1 ,T1 ,iG2);
iIM3 ( : ,: ,2) = iIM3 ( : ,: ,2) + O2;
imwrite (iIM3 ,'.\result\result.bmp');


