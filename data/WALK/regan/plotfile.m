clear
close all
m = csvread('1_t.csv',2,1);
x = {'x-axis','y-axis','z-axis'};
t = m(:,1)-m(1,1);
figure
for i = [2:4]
    subplot(3,1,i-1)
    plot(t,m(:,i))
    xlabel('Time (s)')
    title(x(1,i-1))
    ylabel('Acceleration (g)')
end
figure
for i = [5:7]
    subplot(3,1,i-4)
    plot(t,m(:,i))
    xlabel('Time (s)')
    title(x(1,i-4))
    ylabel('Acceleration (g)')
end