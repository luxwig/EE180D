catdata;
[val1, pos1]= zanalysis(walk_t{1,7});
[val2, pos2]= zanalysis(ascend{1,7});
[val3, pos3]= zanalysis(descend{1,7});

close all
fileID=fopen('test.txt','w');
fprintf(fileID,'%d 4 3\n', length(val1)+ ...
    length(val2)-floor(length(val2)/2)+1 + ...
    length(val3)-floor(length(val3)/2)+1);

hold on
for i = [1:length(val1)]
    scatter([pos1(i,1), pos1(i,2)],[val1(i,1), val1(i,2)],'r')
    fprintf(fileID,'%f %f %f %f\n', val1(i,1), val1(i,2), pos1(i,1), pos1(i,2));
    %fprintf(fileID,'1 -1 -1\n');
end

for i = [floor(length(val2)/2):length(val2)]
     scatter([pos2(i,1), pos2(i,2)],[val2(i,1), val2(i,2)],'b')
    fprintf(fileID,'%f %f %f %f\n', val2(i,1), val2(i,2), pos2(i,1), pos2(i,2));
    %fprintf(fileID,'-1 1 -1\n');
end

for i = [floor(length(val3)/2):length(val3)]
     scatter([pos3(i,1), pos3(i,2)],[val3(i,1), val3(i,2)],'g')
    fprintf(fileID,'%f %f %f %f\n', val3(i,1), val3(i,2), pos3(i,1), pos3(i,2));
    %fprintf(fileID,'-1 1 -1\n');
end

hold off
fclose(fileID);