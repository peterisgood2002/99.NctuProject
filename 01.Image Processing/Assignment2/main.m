%�D�{���i�J�I �H�U�O���{�������]�w
%1.���{�������ܼƩR�W�� 
%  (1)�}�C�ܼƦW�٫᭱�@�w�|�����Ʀr �H��K���Ѧ��ܼƨ쩳��3*3��array��2*2��array
%     �}�C�ܼƤ@�w�j�g�Ӥj�g�e����ܥL����ƫ��A
%  (2)�Y�O�ɮצW�٪��ܼ� �ܼƦW�٤��@�w�|��name
%  (3)�¼Ʀr�ܼƦW�ٱN���|�b�᭱������name�μƦr�X�{
%  (4)�ܼƤ������r�����N�q
%     I�Ginput  M�Gmatrix��mask H�Ghistogram
%  (5)���ץHdegree���
%2.�i��Edge Detection ���T�ӨB�J
%  (1)Image smoothing for noise reduction
%  (2)Detection of edge points
%  (3)Edge Localiztion

clear;
iname = '.\data\data.bmp';

%Ū��
iIM3 = imread(iname);


%(1)Image smoothing for noise reduction
%smooth mask
SM = [ 1 1 1;
       1 1 1;
       1 1 1];
SM = SM * (1/9);
%��RGB�C�@��matrix��smoothing
for ( i = 1 : 3)
    dSM3( : ,: ,i) = correlationOP( iIM3( : ,: ,i) ,SM );
    %�Nsmooth���G�g�X
    %str = strcat('.\result\smooth\' ,int2str(i) ,'.bmp');
    %imwrite( uint8( dSM3(: ,: ,i) ) ,str );
end
imwrite ( uint8(dSM3) ,'.\result\smooth\smooth.bmp');
%(2)Detection of edge points�G�B��image gradient����u
iG2 = edgeDetction(dSM3);

%(3)Edge Localiztion�G�i�� Hough Transformation ��edge�����u
nol = 40;
[R1 ,T1] = houghTransform ( iG2 , nol);

%(4)��C�@���u�e�u
O2 = plotLine( R1 ,T1 ,iG2);
iIM3 ( : ,: ,2) = iIM3 ( : ,: ,2) + O2;
imwrite (iIM3 ,'.\result\result.bmp');


