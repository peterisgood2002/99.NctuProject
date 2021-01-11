%主程式進入點 以下是此程式相關設定
%1.被分開的部分以 在陣列中會以257表示 以方便在Histogram時不處理
%2.因為Matlab Matix從1開時算起 因此在array中儲存的值都會是從1-256 到時候再進行相關轉換即可從1-256 ==> 0-255
%3.此程式為灰階處理 不支援彩色處理
%4.經過Photoshop處理後 很神奇的發現一件事情 明明是以R =(255 ,0,0) G = (0,255,0) B =(0,0,255)處理的影像
%  經過轉出成jpg檔以後 其相對應的RGB值為 R =(254 ,0,0) G = (0,255,1) B =(0,0,254)
%  因此我們將會以此神奇值做為處理
%5.此程式相關變數命名學 
%  (1)陣列變數名稱後面一定會接有數字 以方便辨識此變數到底為3*3的array或2*2的array
%     陣列變數一定大寫
%  (2)若是檔案名稱的變數 變數名稱內一定會有name
%  (3)純數字變數名稱將不會在後面有任何的name或數字出現
%  (4)變數中相關字元的意義
%     I：input P：partition M：matrix或mask R：reference H：histogram

clear;
%輸入相關資料
oiname = input('please input the original image file name：' ,'s');
mname = input('please input the mask image file name：' ,'s');
rname = input('please input the reference image file name：' ,'s');
oname = input('pleasr input the output image file name：' ,'s');
%oiname = '.\test\test.jpg';
%mname = '.\test\mask.jpg';
%rname = '.\test\ref.jpg';
%oname = '.\tmp\outimage.jpg';
rgbcolor = [ 254  0    0
              0  255   1 
              0   0   254];
         
%讀檔
IM3 = imread(oiname);%命名方法input image matrix 3*3
MM3 = imread(mname);
RM3 = imread(rname);

% output image matrix 2*2 因為是灰階
[row col color] = size(IM3);
OM2 = zeros(row ,col ,'double');%拆開背景 臉 衣服以後 計算完新的結果後 將會被加入此matrix構成一個新的圖片

%分別根據RGB 顏色拆開 計算並合成
%i = input ('input an integer');
for ( i=1:color) 
    c = [ rgbcolor(i ,1) rgbcolor(i,2) rgbcolor(i ,3) ];
    %切開
    PIM3 =partition(IM3 ,MM3 ,c);%partition image matrix 3*3
    PRM3 =partition(RM3 ,MM3 ,c);
    imwrite( uint8(PIM3) ,'.\tmp\im3.jpg');
    imwrite( uint8(PRM3) ,'.\tmp\pm3.jpg');
    
    %將彩色的轉成黑白
    PIM2 = rgbToGray(PIM3);%partition image matrix 2*2
    PRM2 = rgbToGray(PRM3);
    imwrite( uint8(PIM2) ,'.\tmp\im2.jpg');
    imwrite( uint8(PRM2) ,'.\tmp\pm2.jpg');
    
    %累積每一個image的level的次數
    level = 256;
    HIM1 = calHistogram(PIM2 ,level);%histogram of input image matrix    
    HRM1 = calHistogram(PRM2 ,level);
    
    %計算覆蓋部分到底有多少個pixel
    mn = 0;
    for ( i = 1:level)
        mn = mn + HIM1(1 ,i);
    end
    
    %計算s =T(r)之每一個r-->s
    S1 = histEQ(HIM1 ,mn,level);%
    %計算s =G(z) 因為是同一個公式 所以用同一個function代替
    GofZ1 = histEQ(HRM1 ,mn ,level);
    RToZ1 = rToZ(S1 ,GofZ1 ,level);
    %一個暫時的matrix存相對應的值
    TM2 = zeros(row ,col );
    %進行mapping r -->s
    for ( i = 1 : row )
        for (j = 1 : col)
            r = PIM2(i ,j);% 0-255
            r = r+1;%轉成1-256
            
            if ( r > 0)%0表示他是被切開的部分
                r = uint8(r);
                z = RToZ1( 1 ,r); % z = G-1(s) s-->z z值為1-256
                TM2(i ,j) = z - 1;%轉成0-255
            else TM2(i ,j) = -1;
            end
        end
    end
    
    %加入output image好output
    OM2 = TM2 + OM2;
    
end

HOM1 = calHistogram(OM2 ,level);
imwrite( uint8(OM2) ,oname);