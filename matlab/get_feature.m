function [pos, r, features ] = get_feature( m )
    [pos, r] = segment(m);
    [val1, pos1]= zanalysis(r);
    features = [val1 pos1];
end

