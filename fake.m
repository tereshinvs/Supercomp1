 A = [ 1 2 4 ]
 B = [ 7.3326 4.74 3.02 ]
 C = [ B(1) B(1)/2 B(1)/4 ]
 
 hold on
 plot(A, B)
 plot(A, C, 'g')
 
 xlabel('Processor number')
 ylabel('Time')
 
 legend('Real time', 'Estimated time')
 