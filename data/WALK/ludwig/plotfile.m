%%
%{
clear
close all
m = csvread('1.csv',2,1);
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
%}
%%
close all
fileID=fopen('file.txt','w');
output=[];
name = {'1.csv','2.csv','3.csv','4.csv','1_t.csv','2_t.csv','3_t.csv','4_t.csv'};
for i = [1:4]
m = csvread(char(name(1,i)),2,1);
tt = m(:,1)-m(1,1);
x = m(:,2)-mean(m(:,2));
[val,pos]=findpeaks(x,'MinPeakHeight',0.4, 'MinPeakDistance',200);
height = 0.05;
[valmin,posmin] = findpeaks(-x,'MinPeakHeight',height, 'MinPeakDistance',200);
while (length(valmin)~=length(val))
   height = height+0.001;
   [valmin,posmin] = findpeaks(-x,'MinPeakHeight',height, 'MinPeakDistance',200);
end
for j = [1:length(valmin)-1]
    t = [val(j) valmin(j) tt(pos(j+1))-tt(pos(j)) 0];
    output = [output; t];
    t = -ones(1,4);
    t(i) = 1;
    output = [output; t];
end
figure
plot(x)
hold on
scatter(pos,val)
scatter(posmin,-valmin)
hold off
end
fprintf(fileID,'81 3 4\n');
for i = [1:length(output)/2]
    fprintf(fileID,'%f %f %f\n', output(i*2-1,1), output(i*2-1,2), output(i*2-1,3));
    fprintf(fileID,'%d %d %d %d\n', output(i*2,1), output(i*2,2), output(i*2,3), output(i*2,4));
end
fclose(fileID);
%copyfile('file.txt','edison_fann_shadows/FANN_system/file.txt');
%%
close all
fileID=fopen('test.txt','w');
output=[];
name = {'1_t.csv','2_t.csv','3_t.csv','4_t.csv'};
for i = [1:4]
m = csvread(char(name(1,i)),2,1);
tt = m(:,1)-m(1,1);
x = m(:,2)-mean(m(:,2));
[val,pos]=findpeaks(x,'MinPeakHeight',0.4, 'MinPeakDistance',200);
height = 0.05;
[valmin,posmin] = findpeaks(-x,'MinPeakHeight',height, 'MinPeakDistance',200);
while (length(valmin)~=length(val))
   height = height+0.001;
   [valmin,posmin] = findpeaks(-x,'MinPeakHeight',height, 'MinPeakDistance',200);
end
for j = [1:length(valmin)-1]
    t = [val(j) valmin(j) tt(pos(j+1))-tt(pos(j)) 0];
    output = [output; t];
end
figure
plot(x)
hold on
scatter(pos,val)
scatter(posmin,-valmin)
hold off
end
for i = [1:length(output)]
    fprintf(fileID,'%f %f %f\n', output(i,1), output(i,2), output(i,3));
end
fclose(fileID);
copyfile('test.txt','edison_fann_shadows/FANN_system/test.txt')