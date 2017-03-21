function plotAllAxis( path )
%PLOTALLAXIS Summary of this function goes here
%   Detailed explanation goes here
figure
`
ltxt = {'X Accelerometer','Y Accelerometer','Z Accelerometer', 'X Gyro','Y Gyro','Z Gyro'};
m = csvread(path,2,1);
t = m(:,1);
t = t-t(1);
for j = [2:7]
        subplot(3,2,j-1)
        plot(t,m(:,j))
        xlim([min(t) max(t)])
        title(ltxt(j-1))
end

end

