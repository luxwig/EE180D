function [ val, pos ] = fp( x )
%FP Summary of this function goes here
%   Detailed explanation goes here
    [val,pos] = findpeaks(x,'MinPeakDistance',50);

end

