function train_g( fname, col )
close all
fileID=fopen(fname,'w');
fileIDx=fopen([int2str(col-1)],'w');
output=[];
name = {'1_t.csv','2_t.csv','3_t.csv','4_t.csv'};
m = csvread(char(name(1,i)),3,1);
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
    fprintf(fileIDx,'\t%d\n',i);
end
figure
plot(tt,x)
hold on
scatter(tt(pos),val)
scatter(tt(posmin),-valmin)
hold off
xlabel('Time (s)');
ylabel('Acceleration (g)');
xlim([0 max(tt)]);
title(['Speed ' int2str(i)]);
fprintf('Speed %d:\n',i)
fprintf('\tMean :%.3f\n',mean(m(:,col)))
fprintf('\tRMS  :%.3f\n',rms(m(:,col)))
fprintf('\tSTD  :%.3f\n',std(m(:,col)))
fprintf('\tMean Min :%.3f\n',mean(valmin))
fprintf('\tMean Max :%.3f\n',mean(val))
end
for i = [1:length(output)]
    fprintf(fileID,'%f %f %f\n', output(i,1), output(i,2), output(i,3));
end
fclose(fileID);
fclose(fileIDx);
copyfile(fname,['edison_fann_shadows/FANN_system/' fname]);
copyfile([int2str(col-1)],['edison_fann_shadows/FANN_system/' int2str(col-1)]);
end

