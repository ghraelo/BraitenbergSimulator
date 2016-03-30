function [period] = FindPeriodicity(x,t)
%FindPeriodicity - detect periodicity in data
%   http://www.sciencedirect.com/science/article/pii/S0959152410000600

figure;
subplot(3,1,1);
plot(t,x);
title('x');

y = dct(x);

subplot(3,1,2);
plot(t,y);
title('dct(x)');

%remove noise
sigma = std(y);
sea_level = 3 * sigma;
y(abs(y) < sea_level) = 0;

subplot(3,1,3);
plot(t,y);
title('thresholded dct(x)');

%extract different frequency components

startIndices = find(y ~= 0);
startIndices(y(startIndices-1) == 0);

indexMatrix = [];
prevEndIndex = 0;
for n = 1 : length(startIndices)
    if(prevEndIndex < 0 || prevEndIndex > startIndices(n))
        continue
    end  
    endIndices = find(y ~= 0);

    endIndices = endIndices(endIndices > startIndices(n));
    endIndices = endIndices(y(endIndices+1) == 0);
    endIndices = endIndices(y(endIndices+2) == 0);
    endIndices = endIndices(y(endIndices+3) == 0);
    endIndices = endIndices(y(endIndices+4) == 0);
    if size(endIndices) > 0
        prevEndIndex = endIndices(1);
        mat =  [startIndices(n) endIndices(1)];
        indexMatrix = [indexMatrix;mat];
    else
        prevEndIndex = -1;
    end
end

period=[];
size(indexMatrix,1)
for n = 1 : size(indexMatrix,1)
    y_i = y;
    y_i(1:indexMatrix(n,1)) = 0;
    y_i(indexMatrix(n,end):end) = 0;
    
    x_i = idct(y_i);
    figure;
    plot(t,x_i);
    
    %zero crossings    
    Z_i = zerocross(x_i);
    zlength = length(Z_i)
    T_i = [];
    for m = 2 : length(Z_i)
        T_i = [T_i 2*(Z_i(m) - Z_i(m-1))];
    end
    T_i
    meanT_i = mean(T_i);
    sigmaT_i = std(T_i);
    
    if((sigmaT_i / meanT_i) < (1/3))
        period = [period meanT_i];
    end
    
end

end

