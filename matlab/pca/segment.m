function [r] = segment(filePath, sample_rate)
if nargin < 2
    sample_rate = 200;
end

%{
filePath = '../data/WALK/ludwig/';
%filePath = '../data/DESCEND/';
name = {'1.csv','2.csv','3.csv','4.csv'};

%}

m = csvread(filePath,3,1);
t = m(:,1);
t = t-t(1);
zgyro = m(:,7);
%plot(t,zgyro);
[p_val,p_pos]=findpeaks(zgyro,'MinPeakDistance',200,'MinPeakHeight',50);
%hold on
%scatter(t(p_pos),p_val);
%hold off
clear r
clear ur

tempd = [];
for i = [1:length(p_pos)-1]
    len = p_pos(i+1)- p_pos(i);
    tempd = [tempd;len/sample_rate];
end

r{1,8} = tempd;

for ax = 1:7
    %figure
    tempd = [];
    m(:,ax) = m(:,ax)- mean(m(:,ax));
    %hold on
    for i = [1:length(p_pos)-1]
        len = p_pos(i+1)- p_pos(i);
        %p = len/sample_rate;
        [p,q] = rat(sample_rate/len);
        downs = resample(m([p_pos(i):p_pos(i+1)-1],ax),p,q);
        %downs = interp1([1:len],[1:sample_rate],m([p_pos(i):p_pos(i+1)-1],ax));
        tempd=[tempd; downs'];
        %plot(t([p_pos(i):p_pos(i+1)-1])-t(p_pos(i)), m([p_pos(i):p_pos(i+1)-1],ax));
    end
    %hold off
    r{1,ax} = tempd;
end

%{
t = r{1,1};
[p,q] = size(t);
for i = 1:q
    t(:,i)=t(:,i)-t(1,i);
end
for ax = 1:7
    figure
    hold on
    axd = r{1,ax};
    [p,q] = size(axd);
    for i = 1:q
        plot(axd(:,i),'g')
    end
    hold off
    pause
end
%}
end
