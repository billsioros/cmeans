function plot2D(filename)
    fid = fopen(filename);

    line1 = fgetl(fid);
    line2 = fgetl(fid);
    while ischar(line1) && ischar(line2)
        scatter(convert(line1), convert(line2), '*');
        grid on; hold on;
        
        line1 = fgetl(fid);
        line2 = fgetl(fid);
    end
end

function value = convert(line)
    line(line == 44) = ' ';
    value = cell2mat(textscan(char(line),'%f'));
end
