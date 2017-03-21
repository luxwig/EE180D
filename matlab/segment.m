function r = segment(m)
    SAMPLE_RATE = 200;
    
    %fprintf('segment Inside\n');
    t = m(:,1);
    t = t-t(1);
    zgyro = m(:,7);
    p_val = [];
    p_pos = [];
    %fprintf('segment Inside - zgyro\n');
    [p_val,p_pos]=findpeaks(zgyro,'MinPeakDistance',200,'MinPeakHeight',50);
    
    tx = zeros(length(p_val)-1,SAMPLE_RATE);
    
    %fprintf('segment Inside - findpeaksfinish\n');
    %fprintf('%f\n',p_val(1));
    %fprintf('segment Inside - findpeaksfinish2\n');
    %fprintf('%f\n',p_pos(1));
    %fprintf('segment Inside - findpeaksfinish3\n');
    %fprintf('segment Inside - findpeaksfinish4\n');
    m(:,7) = m(:,7)- mean(m(:,7));
    %fprintf('segment Inside - demean\n');
    for i = [1:length(p_pos)-1]
        len = p_pos(i+1)- p_pos(i);
        downs = interp1([1:len],m([p_pos(i):p_pos(i+1)-1],7),[1:len/SAMPLE_RATE:len]);
        tx(i,:) = downs;
    end
    %fprintf('segment Inside - read return\n');
        %hold off
     r = tx;
end

