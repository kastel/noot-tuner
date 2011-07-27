% This is the C function, third version
%    x        input signal
%    f        frequencies to test
%    options  option struct
function out = PowerSpectrum3(x, f, options)
    t = (0:(length(x)-1))./options.SampleRate;
    out = zeros(size(f));
    
    for n = 1:length(f)
        T = floor(2*f(n)*t(end))/(2*f(n));
        tmax = floor(T*options.SampleRate);
        out(n) = (sum(sin(2*pi*f(n)*t(1:tmax)).*x(1:tmax))^2 + sum(cos(2*pi*f(n)*t(1:tmax)).*x(1:tmax))^2);
    end
end