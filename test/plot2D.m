function plot2D(filename)
    fid = fopen(filename);

    id = 1;
    
    line1 = fgetl(fid);
    line2 = fgetl(fid);
    while ischar(line1) && ischar(line2)
        x = convert(line1);
        y = convert(line2);
        scatter(x, y, '*'); grid on; hold on;
        
        cx = x(length(x));
        cy = y(length(y));
        txt = ['C' num2str(id)]; id = id + 1;
        text(cx, cy, txt);
        
        line1 = fgetl(fid);
        line2 = fgetl(fid);
    end
end

function value = convert(line)
    line(line == 44) = ' ';
    value = cell2mat(textscan(char(line),'%f'));
end
