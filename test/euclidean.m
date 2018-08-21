function dist = euclidean(x1, y1, x2, y2)
    xdiff = x1 - x2;
    ydiff = y1 - y2;
    
    dist = sqrt(xdiff * xdiff + ydiff * ydiff);
end

