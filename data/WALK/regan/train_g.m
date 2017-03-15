function train_g( fname, col )
close all
fileID=fopen(fname,'w');
output=[];
name = {'1.csv','2.csv','3.csv','4.csv'};
for i = [1:4]
m = csvread(char(name(1,i)),2,1);
tt = m(:,1)-m(1,1);
x = m(:,col)-mean(m(:,col));
minpeaks = 0.4;
if col == 4
    minpeaks = 0.2;
end
mind = 200;
[val,pos]=findpeaks(x,'MinPeakHeight',minpeaks, 'MinPeakDistance',mind);
height = 0.05;
[valmin,posmin] = findpeaks(-x,'MinPeakHeight',height, 'MinPeakDistance',mind);
while (length(valmin)<length(val))
    mind = mind - 1;
    [val,pos]=findpeaks(x,'MinPeakHeight',minpeaks, 'MinPeakDistance',mind);
    [valmin,posmin] = findpeaks(-x,'MinPeakHeight',height, 'MinPeakDistance',mind);
end
while (length(valmin)~=length(val))
   height = height+0.001;
   [valmin,posmin] = findpeaks(-x,'MinPeakHeight',height, 'MinPeakDistance',mind);
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
for i = [1:length(output)/2]
    fprintf(fileID,'%f %f %f\n', output(i*2-1,1), output(i*2-1,2), output(i*2-1,3));
    fprintf(fileID,'%d %d %d %d\n', output(i*2,1), output(i*2,2), output(i*2,3), output(i*2,4));
end
fclose(fileID);
copyfile(fname,['edison_fann_shadows/FANN_system/' fname]);

end

