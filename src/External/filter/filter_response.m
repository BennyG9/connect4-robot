alpha = 0.5;
fsamp = 1000;

%Frequency Responses 
w = linspace(-pi, pi, 2000);

%creating EMA filter & first difference filter
z = exp(1i*w);
H1 = alpha ./ (1 - (1-alpha)*(z).^(-1));
D = 1 - z.^(-1);
H = H1 .* D;

figure("Name","Individual Frequency Responses", "NumberTitle", "off");

%plot magnitude response of EMA filter
subplot(2,1,1);
plot(w, abs(H1));
xlim([-pi pi]);
xlabel('\omega (rad/sample)');
ylabel('|H1(e^{j\omega})|');
title("Magnitude Response of EMA Filter");
grid on;

%plot magnitude response of first difference filter
subplot(2,1,2);
plot(w, abs(D));
xlim([-pi pi]);
xlabel('\omega (rad/sample)');
ylabel('|D(e^{j\omega})|');
title("Magnitude Response of First Difference Filter");
grid on;


figure("Name","Cascaded Frequency Response", "NumberTitle", "off");

%plot magnitude response of cascaded filter
subplot(2,1,1);
plot(w, abs(H));
xlim([-pi pi]);
xlabel('\omega (rad/sample)');
ylabel('|H(e^{j\omega})|');
title("Magnitude Response of EMA + First Difference Filter");
grid on;

%plot phase response of cascaded filter
subplot(2,1,2);
plot(w, phase(H));
xlim([-pi pi]);
xlabel('\omega (rad/sample)');
ylabel('\angleH(e^{j\omega})');
title("Phase Response of EMA + First Difference Filter");
grid on;


%Pole-Zero Plot
%z transform numerator and denominator coefficients 
num_z_coeff = alpha * [1, -1];
denom_z_coeff = [1, -1*(1-alpha)];

%plot Pole-Zero plot
figure("Name","Pole-Zero Plot", "NumberTitle", "off");
zplane(num_z_coeff, denom_z_coeff);
title("Pole-Zero Plot of EMA + First Difference Filter");



%Simulated IR Sensor Signal
%generate simulated signal
t = 0 : 1/fsamp : 2;
b = 8*cos(2*pi*2*t) + 3; %exaggerated baseline IR sensor reading oscillation
p = -10 * exp(-((t-0.4).^2)/(2*0.01^2)) - 10 * exp(-((t-1.16).^2)/(2*0.01^2)) - 10 * exp(-((t-1.4).^2)/(2*0.01^2)); %gaussian pulse to simulate piece dropped in
x = b + p; %sensor signal = baseline + pulse
x_padded = [x, zeros(1, 2^13-length(x))]; %zero padding

%create fft of filter
N = length(x_padded);
w_fft = 2*pi*(0:N-1)/N;
z_fft = exp(1i*w_fft);
H1_fft = alpha ./ (1 - (1-alpha)*(z_fft).^(-1));
D_fft = 1 - z_fft.^(-1);
H_fft = H1_fft .* D_fft;
%create fft of simulated signal
X = fft(x_padded, N);
%apply filter
Y = X .* H_fft;
%get new signal back in time domain
y_padded = ifft(Y);
y = real(y_padded(1:length(x)));
%apply threshold
threshold = -0.4;
dig = y < threshold;

figure("Name","Simulated Signal Filtering", "NumberTitle", "off");

%plot simulated signal
subplot(3,1,1);
plot(t, x);
axis([0 2 -25 15])
title("Simulated Signal");
xlabel('t (s)');
ylabel('Input Magnitude');
grid on;
xline(0.375,'r--');
xline(1.135,'r--');
xline(1.375,'r--');

%plot filtered signal
subplot(3,1,2);
plot(t, y);
axis([0 2 -1 1])
yline(threshold, '--r', 'Threshold');
title("Filtered Simulated Signal");
xlabel('t (s)');
ylabel('Filtered Magnitude');
grid on;
xline(0.375,'r--');
xline(1.135,'r--');
xline(1.375,'r--');

%plot digital signal from thresholding the filtered signal
subplot(3,1,3);
plot(t, real(dig));
axis([0 2 -0.2 1.2])
title("Digital Signal");
xlabel('t (s)');
ylabel('Thresholded Signal');
grid on;
xline(0.375,'r--');
xline(1.135,'r--');
xline(1.375,'r--');
