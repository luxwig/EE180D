fn = {                   
%{
                         '../data/WALK/ludwig/1.csv',
                         '../data/WALK/ludwig/2.csv',
                         '../data/WALK/ludwig/3.csv',
                         '../data/WALK/ludwig/4.csv',
                         '../data/RUN/6.csv',  
                         '../data/RUN/8.csv',
                         '../data/RUN/10.csv',
                         '../data/ASCEND/1.csv',
                         '../data/ASCEND/2.csv',
                         '../data/ASCEND/3.csv',
                         '../data/ASC_SPEED/asc_1.csv',
                         '../data/ASC_SPEED/asc_1_2.csv',
                         '../data/ASC_SPEED/asc_2_1.csv',
                         '../data/ASC_SPEED/asc_2_2.csv',
                         '../data/DESCEND/1.csv',
                         '../data/DESCEND/2.csv',
                         '../data/DESCEND/3.csv',
%}
'../data/DSC_SPEED/dsc_2_1.csv'};
file = textread('../output.txt','%s','delimiter','\n','whitespace','');                     
for i = 1:length(fn)
    index = [ 923,1061,1201,1327,1472,1587,1725,1851,1983,2114,2249] %str2num(char(file(i*5)))+1;
    m = csvread(char(fn(i)),2,1);
    plot(m(:,7));
    hold on
    plot(m(:,2)*100,'r');
    scatter(index,m(index,7));
    hold off
    pause
end
    
                        