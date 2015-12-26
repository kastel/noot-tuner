% Test options
maxtrials = 100; % number of trials
minfreq = 16;
maxfreq = 200;
noise = 0.5;
offset = 10;

% Create the global results table: C freq noise refined_freq err
if ~exist('results', 'var')
    results = [];
end

t = (0:(options.WindowSize-1))/options.SampleRate;

fftdd = double(options.SampleRate)/options.WindowSize;

disp('C freq noise f_r err_cents');

btime = now;

for n = 1:maxtrials
    % Pick a random frequency
    f = minfreq + rand*(maxfreq-minfreq);
    
    % Create input signal
    x = (sin(2*pi*f*t + rand) + noise*rand(size(t))).*hanning(length(t))' + offset*rand;
    
    % Simulate an fft
    firstf = round(f/fftdd)*fftdd;
    unused = fft(x);% This is just to lose some time
    sprintf('%d', unused(1));
    
    fr = RefineFrequency(firstf, x, options);
    
    % Add result to the table
    row = [options.C, f, noise, fr, log2(fr/f)*1200];
    results = [results; row];
    
    fprintf('%d %f %f %f %f\n', options.C, f, noise, fr, log2(fr/f)*1200);
end

etime = now;

fprintf('Throughput: %f buffers/s\n', maxtrials/(etime-btime)/86400)
clear unused
