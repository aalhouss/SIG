xnT=[ 3 3 1 1 -2 -2 1 ]
nx=[ 1 2 3 4 5 6 7 ];
tx=[ 0.1 0.2 0.3 0.4 0.5 0.6 0.7 ];
hnT=[  -1 0 1 0 -1 0 1  ];
nh=[ -2 -1 0 1 2 3 4 ];
th=[ -0.2 -0.1 0 0.1 0.2 0.3 0.4 ];
t=0.1
nxl=1;
nxr=7;
nxe=7;
nhl=-2;
nhr=4;
nhe=7;
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
