filePath = '../../data/WALK/ludwig/';
name = {'1.csv','2.csv','3.csv','4.csv'};
clear walk
walk{1,8} = []; 
for i = 1:4
    r = segment(strcat(filePath,char(name(i))));
    for j = 1:8
        walk{1,j} = [walk{1,j}; r{1,j}];
    end
end
w_m = [];
for i = 7:7
    w_m = [w_m walk{1,i}];
end

filePath = '../../data/WALK/ludwig/';
name = {'1_t.csv','2_t.csv','3_t.csv','4_t.csv'};
clear walk_t
walk_t{1,8} = []; 
for i = 1:4
    r = segment(strcat(filePath,char(name(i))));
    for j = 1:8
        walk_t{1,j} = [walk_t{1,j}; r{1,j}];
    end
end

wt_m = [];
for i = 7:7
    wt_m = [wt_m walk_t{1,i}];
end
%{
for i = 7:7
    figure
    hold on
    
    for j = 1:length(walk{1,8})
        t = (walk{1,i});
        plot(t(j,:))
    end
    [c s l] = pca(t);
    plot(c(:,1)*10,'r');
    hold off
    pause
end
%}

filePath = '../../data/ASCEND/';
name = {'1.csv','2.csv','3.csv','4.csv','5.csv','6.csv'};
clear ascend
ascend{1,8} = []; 
for i = 1:3
    r = segment(strcat(filePath,char(name(i))));
    for j = 1:8
        ascend{1,j} = [ascend{1,j}; r{1,j}];
    end
end
a_m=[];
for i = 7:7
    a_m = [a_m ascend{1,i}];
end

clear ascend_t
ascend_t{1,8} = []; 
for i = 4:6
    r = segment(strcat(filePath,char(name(i))));
    for j = 1:8
        ascend_t{1,j} = [ascend_t{1,j}; r{1,j}];
    end
end
at_m=[];
for i = 7:7
    at_m = [at_m ascend_t{1,i}];
end
%{
for i = 7:7
    figure
    hold on
    
    for j = 1:length(ascend{1,8})
        t = (ascend{1,i});
        plot(t(j,:))
    end
    [c s l] = pca(t);
    plot(c(:,1)*10,'r');
    hold off
    pause
end
%}

filePath = '../../data/DESCEND/';
name = {'1.csv','2.csv','3.csv','4.csv','5.csv','6.csv'};
clear descend
descend{1,8} = []; 
for i = 1:3
    r = segment(strcat(filePath,char(name(i))));
    for j = 1:8
        descend{1,j} = [descend{1,j}; r{1,j}];
    end
    
end
d_m=[];
for i = 7:7
    d_m = [d_m descend{1,i}];
end

clear descend_t
descend_t{1,8} = []; 
for i = 4:6
    r = segment(strcat(filePath,char(name(i))));
    for j = 1:8
        descend_t{1,j} = [descend_t{1,j}; r{1,j}];
    end
    
end
dt_m=[];
for i = 7:7
    dt_m = [dt_m descend_t{1,i}];
end
%{
for i = 7:7
    figure
    hold on
    
    for j = 1:length(descend{1,8})
        t = (descend{1,i});
        plot(t(j,:))
    end
    [c s l] = pca(t);
    plot(c(:,1)*10,'r');
    hold off
    pause
end
%}