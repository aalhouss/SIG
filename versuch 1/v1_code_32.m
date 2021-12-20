f0=150;
fs=22020;
dauer=3;
t=0:1/fs:dauer;
x0=0
winkel0=0
xm=[ 0.1  0.05 0.2 0.1  0.05]
m=[ 2 6 10 11 14]
xm_winkel=[ -pi/4 pi/3 pi/4 -pi/2 -pi/3 ]
XnT=x0*cos(winkel0);
for i=1:1:length(xm)
XnT=XnT+2*xm(i)*cos(2*pi*m(i)*f0*t+xm_winkel(i));
end
dauers=0.01;
t=0:1/fs:dauers;
stem(t,XnT(1:1:length(t)));
sound(XnT,fs);