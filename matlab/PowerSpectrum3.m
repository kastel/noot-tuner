% This is the C function, third version
%    x        input signal
%    f        frequencies to test (ranged in [0,1])
function out = PowerSpectrum3(x, f)
    t = 0:(length(x)-1);
    out = zeros(size(f));
    
    for n = 1:length(f)
        % f is the frequency in periods/sample
        % floor(f*samples) is the number of periods
        % 1/F is the length of a period in samples
        tmax = floor(floor(length(t)*f(n)*2)*(1/2/f(n)));
        out(n) = (sum(sin(2*pi*f(n)*t(1:tmax)).*x(1:tmax))^2 + sum(cos(2*pi*f(n)*t(1:tmax)).*x(1:tmax))^2);
    end
end