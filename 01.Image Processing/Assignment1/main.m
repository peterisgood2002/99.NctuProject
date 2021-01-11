%�D�{���i�J�I �H�U�O���{�������]�w
%1.�Q���}�������H �b�}�C���|�H257��� �H��K�bHistogram�ɤ��B�z
%2.�]��Matlab Matix�q1�}�ɺ�_ �]���barray���x�s���ȳ��|�O�q1-256 ��ɭԦA�i������ഫ�Y�i�q1-256 ==> 0-255
%3.���{�����Ƕ��B�z ���䴩�m��B�z
%4.�g�LPhotoshop�B�z�� �ܯ��_���o�{�@��Ʊ� �����O�HR =(255 ,0,0) G = (0,255,0) B =(0,0,255)�B�z���v��
%  �g�L��X��jpg�ɥH�� ��۹�����RGB�Ȭ� R =(254 ,0,0) G = (0,255,1) B =(0,0,254)
%  �]���ڭ̱N�|�H�����_�Ȱ����B�z
%5.���{�������ܼƩR�W�� 
%  (1)�}�C�ܼƦW�٫᭱�@�w�|�����Ʀr �H��K���Ѧ��ܼƨ쩳��3*3��array��2*2��array
%     �}�C�ܼƤ@�w�j�g
%  (2)�Y�O�ɮצW�٪��ܼ� �ܼƦW�٤��@�w�|��name
%  (3)�¼Ʀr�ܼƦW�ٱN���|�b�᭱������name�μƦr�X�{
%  (4)�ܼƤ������r�����N�q
%     I�Ginput P�Gpartition M�Gmatrix��mask R�Greference H�Ghistogram

clear;
%��J�������
oiname = input('please input the original image file name�G' ,'s');
mname = input('please input the mask image file name�G' ,'s');
rname = input('please input the reference image file name�G' ,'s');
oname = input('pleasr input the output image file name�G' ,'s');
%oiname = '.\test\test.jpg';
%mname = '.\test\mask.jpg';
%rname = '.\test\ref.jpg';
%oname = '.\tmp\outimage.jpg';
rgbcolor = [ 254  0    0
              0  255   1 
              0   0   254];
         
%Ū��
IM3 = imread(oiname);%�R�W��kinput image matrix 3*3
MM3 = imread(mname);
RM3 = imread(rname);

% output image matrix 2*2 �]���O�Ƕ�
[row col color] = size(IM3);
OM2 = zeros(row ,col ,'double');%��}�I�� �y ��A�H�� �p�⧹�s�����G�� �N�|�Q�[�J��matrix�c���@�ӷs���Ϥ�

%���O�ھ�RGB �C���} �p��æX��
%i = input ('input an integer');
for ( i=1:color) 
    c = [ rgbcolor(i ,1) rgbcolor(i,2) rgbcolor(i ,3) ];
    %���}
    PIM3 =partition(IM3 ,MM3 ,c);%partition image matrix 3*3
    PRM3 =partition(RM3 ,MM3 ,c);
    imwrite( uint8(PIM3) ,'.\tmp\im3.jpg');
    imwrite( uint8(PRM3) ,'.\tmp\pm3.jpg');
    
    %�N�m�⪺�ন�¥�
    PIM2 = rgbToGray(PIM3);%partition image matrix 2*2
    PRM2 = rgbToGray(PRM3);
    imwrite( uint8(PIM2) ,'.\tmp\im2.jpg');
    imwrite( uint8(PRM2) ,'.\tmp\pm2.jpg');
    
    %�ֿn�C�@��image��level������
    level = 256;
    HIM1 = calHistogram(PIM2 ,level);%histogram of input image matrix    
    HRM1 = calHistogram(PRM2 ,level);
    
    %�p���л\�����쩳���h�֭�pixel
    mn = 0;
    for ( i = 1:level)
        mn = mn + HIM1(1 ,i);
    end
    
    %�p��s =T(r)���C�@��r-->s
    S1 = histEQ(HIM1 ,mn,level);%
    %�p��s =G(z) �]���O�P�@�Ӥ��� �ҥH�ΦP�@��function�N��
    GofZ1 = histEQ(HRM1 ,mn ,level);
    RToZ1 = rToZ(S1 ,GofZ1 ,level);
    %�@�ӼȮɪ�matrix�s�۹�������
    TM2 = zeros(row ,col );
    %�i��mapping r -->s
    for ( i = 1 : row )
        for (j = 1 : col)
            r = PIM2(i ,j);% 0-255
            r = r+1;%�ন1-256
            
            if ( r > 0)%0��ܥL�O�Q���}������
                r = uint8(r);
                z = RToZ1( 1 ,r); % z = G-1(s) s-->z z�Ȭ�1-256
                TM2(i ,j) = z - 1;%�ন0-255
            else TM2(i ,j) = -1;
            end
        end
    end
    
    %�[�Joutput image�noutput
    OM2 = TM2 + OM2;
    
end

HOM1 = calHistogram(OM2 ,level);
imwrite( uint8(OM2) ,oname);