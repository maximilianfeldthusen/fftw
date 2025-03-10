
#include <iostream>
#include <vector>
#include <complex>
#include <fftw3.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>

const double PI = 3.14159265358979323846;

// Function to generate a sample signal (sine wave + noise)
std::vector<double> generateSignal(int N, double freq, double sampleRate) {
    std::vector<double> signal(N);
    double amplitude = 1.0;
    double noiseAmplitude = 0.5;

    // Using <random> for better noise generation
    std::default_random_engine generator(static_cast<unsigned int>(time(0)));
    std::uniform_real_distribution<double> distribution(-0.5, 0.5);

    for (int n = 0; n < N; ++n) {
        // Sine wave component
        signal[n] = amplitude * sin(2 * PI * freq * n / sampleRate);
        // Adding some random noise
        signal[n] += noiseAmplitude * distribution(generator);
    }
    return signal;
}

// Function to perform FFT and return the magnitude spectrum
std::vector<double> performFFT(const std::vector<double>& signal) {
    int N = signal.size();
    fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    
    // Copying the signal into the input array
    for (int i = 0; i < N; ++i) {
        in[i][0] = signal[i]; // Real part
        in[i][1] = 0.0;      // Imaginary part
    }

    // Perform FFT using FFTW
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);

    // Calculate the magnitude spectrum
    std::vector<double> magnitude(N / 2);
    for (int i = 0; i < N / 2; ++i) {
        magnitude[i] = std::sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
    }

    // Free the allocated memory
    fftw_free(in);
    fftw_free(out);

    return magnitude;
}

// Function to display data
void displayData(const std::vector<double>& data) {
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << i << ": " << data[i] << std::endl;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

    int sampleRate = 1024;  // Sample rate in Hz
    int N = 1024;           // Number of samples
    double freq = 50.0;     // Frequency of the sine wave in Hz

    // Generate a sample signal
    std::vector<double> signal = generateSignal(N, freq, sampleRate);

    // Perform FFT
    std::vector<double> magnitude = performFFT(signal);

    // Display the magnitude spectrum
    std::cout << "Magnitude Spectrum:" << std::endl;
    displayData(magnitude);

    return 0;
}

