function [ format, f ] = point2feature( p )
%POINT2FEATURE Summary of this function goes here
%   Detailed explanation goes here
    app = p;
    f = [];
    val = p(:,2);
    if (val(1)<=val(2))
        f = [1];
    else
        f = [-1];
    end
    f = [f (max(val)-min(val))/min(val)];
    f = [f p(:,1)'];
    format = '';
    for i = 1:length(f)
        format = [format sprintf('%f ',f(i))];
    end
    format =[format '\n'];
end

