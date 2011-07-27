% RefineFrequency: applies a frequency refinement algorithm (continuous
% power spectrum)
%
%   oldfreq: frequency value to be refines
%   buffer   input signal
%   options  options struct
function freq = RefineFrequency(oldfreq, buffer, options)
    Cs = {@PowerSpectrum1, @PowerSpectrum2, @PowerSpectrum3};
    C = Cs{options.C};
    PowerSpectrum = @(a, b, c)C(a, b); % Shorthand
    
    %The following code is pasted from powerspectrum.cpp, translated to
    %MATLAB
    relfreq = oldfreq/options.SampleRate;
    mindd = relfreq*(power(2.0, options.ExpectedPrecision/12.0) - 1.0);
    fftdd = double(options.SampleRate)/length(buffer)^2;
    
    if fftdd <= mindd
        freq = oldfreq;
        return
    end
    
    %X: frequency, Y: power
    %initialisation
    centreX = relfreq;
    leftX = relfreq - fftdd;
    rightX = relfreq + fftdd;
    
    if leftX < 0
        leftX = 25.0/options.SampleRate; %can't handle negative frequencies!
    end
    
    centreY = PowerSpectrum(buffer, centreX, options);
    rightY = PowerSpectrum(buffer, rightX, options);
    leftY = PowerSpectrum(buffer, leftX, options);

    while (centreX - leftX > mindd)
        %case 3: _-^
        if (leftY < centreY && centreY < rightY)
            leftX = centreX;
            leftY = centreY;
            centreX = (centreX + rightX)/2;

        %case 4: ^-_
        elseif (leftY > centreY && centreY > rightY)
            rightX = centreX;
            rightY = centreY;
            centreX = (leftX + centreX)/2;

        %case 1: _^-
        %case 2: -^_
        elseif (centreY > rightY && centreY > leftY)
            clX = (centreX + leftX)/2;
            crX = (centreX + rightX)/2;
            clY = PowerSpectrum(buffer, clX, options);
            crY = PowerSpectrum(buffer, crX, options);

            if (crY > centreY)
                %right part
                leftX = centreX;
                leftY = centreY;
                centreX = crX;
                centreY = crY;
            elseif (clY > centreY)
                %left part
                rightX = centreX;
                rightY = centreY;
                centreX = clX;
                centreY = clY;
            else
                %central part
                rightX = crX;
                rightY = crY;
                leftX = clX;
                leftY = clY;
            end
            
        %other cases: discard
        else
            display('Abnormal power spectrum local shape. Discarding');
            freq = nan;
            return
        end
    end
    
    %@todo
    if (rightY > centreY)
        centreX = rightX;
    end
    if (leftY > centreY)
        centreX = leftX;
    end
    
    freq = centreX*options.SampleRate;
    
end