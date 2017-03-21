catdata_match;
[val1, pos1]= zanalysis(walk{1,7});
[val2, pos2]= zanalysis(ascend{1,7});
[val3, pos3]= zanalysis(descend{1,7});

fileID=fopen('train.txt','w');
fprintf(fileID,'%d 4 3\n', length(val1)+ceil(length(val2)/2)+ceil(length(val3)/2));
for i = [1:length(val1)]
    fprintf(fileID,'%f %f %f %f\n', val1(i,1), val1(i,2), pos1(i,1), pos1(i,2));
    %fprintf(fileID,point2feature([pos1(i,:)' val1(i,:)']));
    fprintf(fileID,'1 -1 -1\n');
end

for i = [1:ceil(length(val2)/2)]
    fprintf(fileID,'%f %f %f %f\n', val2(i,1), val2(i,2), pos2(i,1), pos2(i,2));
    fprintf(fileID,'-1 1 -1\n');
end

for i = [1:ceil(length(val3)/2)]
    fprintf(fileID,'%f %f %f %f\n', val3(i,1), val3(i,2), pos3(i,1) ,pos3(i,2));
    fprintf(fileID,'-1 -1 1\n');
end

fclose(fileID);