%��G inverse�Ϊ�  ��̱���z
%��J�G�Ʀrs ,�ds = G(z)�۹������� 
function z = Ginverse(s ,GofZ1 ,cl)   
    %s�ɦbmin ~c1���� ��min�q�`�O�̤j��min
    z = -1;
    min = -1;
    if ( isvector(GofZ1) )
        for ( i=cl :-1:1 )
            if ( GofZ1(1 ,i ) == s)
                z = i;
            elseif ( GofZ1(1 ,i) < s && min ==-1 )
                min = i;
            end
        end
        
        if (z == -1 ) 
            z = min;
    end
end