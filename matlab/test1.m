% Test options
maxtrials = 100; % number of trials
minfreq = 25;
maxfreq = 4000;
noise = 0.5;

% Create the global results table: C freq noise refined_freq err
if ~exist('results', 'var')
    results = [];
end

t = (0:(options.WindowSize-1))/options.SampleRate;

fftdd = double(options.SampleRate)/options.WindowSize;

disp('C freq noise f_r err');

for n = 1:maxtrials
    % Pick a random frequency
    f = minfreq + rand*(maxfreq-minfreq);
    
    % Create input signal
    x = (sin(2*pi*f*t + rand) + noise*rand(size(t))).*hanning(length(t))';
    
    % Simulate an fft
    firstf = round(f/fftdd)*fftdd;
    
    fr = RefineFrequency(firstf, x, options);
    
    % Add result to the table
    row = [options.C, f, noise, fr, abs(fr/f-1)];
    results = [results; row];
    
    fprintf('%d %f %f %f %f\n', options.C, f, noise, fr, abs(fr/f-1));
end