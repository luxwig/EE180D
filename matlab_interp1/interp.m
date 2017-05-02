function [ p ] = interp( x, y , sample_rate )
%INTERP1 Summary of this function goes here
%   Detailed explanation goes here
    p = interp1(x, y, linspace(1,length(x),sample_rate));
end

