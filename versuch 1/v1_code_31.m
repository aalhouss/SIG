a=0.3;
f=400;
dauer1=3;
fs=22050;
t=0:1/fs:dauer1;
x=a*cos(2*pi*f*t);
dauer2=0.01;
t=0:1/fs:dauer2;
stem(t,x(1:1:length(t)));
sound(x,fs);