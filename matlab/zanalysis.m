function [val pos] = zanalysis(z_a, t_scale)
    [len, q] = size(z_a);
    pos = [];
    val = [];
    for i = 1:len
        [p_val,p_pos]=findpeaks(-z_a(i,:),'MinPeakDistance',50);
    
        if length(p_val) > 2
            qs = sort(p_val,'descend');
            tp_pos = [ 
                p_pos(find(p_val==qs(1))), p_pos(find(p_val==qs(2)))
                ];
            p_pos = sort(tp_pos);
            p_val = z_a(i,p_pos);
        end
        if length(p_val) == 1
            continue;
        pos = [pos; p_pos];
        val = [val; z_a(i,p_pos)];
    end
    if nargin > 1
        pos(:,1) = pos(:,1).*t_scale;
        pos(:,2) = pos(:,2).*t_scale;
    end
end
