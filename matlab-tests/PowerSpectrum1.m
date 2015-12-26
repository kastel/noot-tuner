% This is the C function, old version
%    x        input signal
%    f        frequencies to test (ranged in [0,1])
function out = PowerSpectrum1(x, f)
    t = 0:(length(x)-1);
    out = zeros(size(f));
    
    for n = 1:length(f)
        out(n) = sum(sin(2*pi*f(n)*t).*x)^2 + sum(cos(2*pi*f(n)*t).*x)^2;
    end
end