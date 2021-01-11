%做G inverse用的  找最接近的z
%輸入：數字s ,查s = G(z)相對應的值 
function z = Ginverse(s ,GofZ1 ,cl)   
    %s界在min ~c1之間 而min通常是最大的min
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