%�N��i�ϰ�����åB���}��� �H0��ܤ���X������ 
%�ǤJ�G��l�� mask �M�H�����C�ⰵmask 
%�ǥX�G���}���� �����Y���Ȭ�257��ܳQ���}���ϰ�
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