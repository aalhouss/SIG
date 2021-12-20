xnT=[1 3 2 2 2 1]
nx=[-2 -1 0 1 2 3];
tx=[-0.4 -0.2 0.0 0.2 0.4 0.6];
hnT=[ 1 0 -1 1 -1 ];
nh=[-1 0 1 2 3];
th=[-0.2 0.0 0.2 0.4 0.6];
t=0.2
nxl=-2;
nxr=3;
nxe=6;
nhl=-1;
nhr=3;
nhe=5;
ix=length(xnT);
ih=length(hnT);
iy=ix+ih-1;
nyr=nxr+nhr;
nyl=nxl+nhl;
ty=nyl*t:t:nyr*t;
ny=nyl:nyr;
iy=ix+ih-1;
y=zeros(1,iy); 
for i = 1:ix
    for k = 1:ih
      y(i+k-1) = y(i+k-1) + hnT(k)*xnT(i);
    end
end
figure(1)
subplot(3,1,1);
stem(nx,xnT);
title("X[nT]");
xlabel("n");
subplot(3,1,2);
stem(nh,hnT);
title("H[nT]");
xlabel("n");
subplot(3,1,3);
stem(ny,y);
title("y[nT]");
xlabel("n")


figure(2);
subplot(3,1,1);
stem(tx,xnT);
title("X[nT]");
xlabel("t");
 subplot(3,1,2);
 stem(th,hnT);
title("H[nT]");
xlabel("t");
subplot(3,1,3);
stem(ty,y);
 title("y[nT]");
xlabel("t");