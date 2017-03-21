function [ r, features ] = get_feature( m )
    for i = 1:10
        for j = 2:7
            fprintf('%f ',m(i,j));
        end
        fprintf('\n');
    end
    r = segment(m);
    [val1, pos1]= zanalysis(r);
    features = [val1 pos1];
end

