f = 10;
f2 = 13;
sr= 2000;
t = [0:1/sr:2];
x = sin(2*pi*f*t)+sin(2*pi*f2*t);
%x = awgn(x,4);

y = FindPeriodicity(x);

y = y/sr;

y = 1 ./ y