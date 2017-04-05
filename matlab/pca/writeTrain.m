catdata;
overall = [w_m;a_m;d_m];

sp = [19 20 19 23 24 24];
typ = [ones(1,19), ones(1,20)*2, ones(1,19)*3, ones(1,23)*4, ones(1,24)*5, ones(1,24)*6];
B = TreeBagger(20,[w_m;a_m;d_m],typ, 'Method', 'classification');
overall = [wt_m;at_m;dt_m];
predChar1 = B.predict(overall);
ccc = char(predChar1);
length(find(ccc=='1'))
length(find(ccc=='2'))
length(find(ccc=='3'))
length(find(ccc=='4'))
length(find(ccc=='5'))
length(find(ccc=='6'))