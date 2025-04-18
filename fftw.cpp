
#include <iostream>
#include <vector>
#include <complex>
#include <fftw3.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <random>
#include <algorithm>

// Constants
const double PI = 3.14159265358979323846;

// Structure to hold peak information
struct Peak {
    int index;
    double magnitude;
};

// Generate a sample signal: sine wave + noise
std::vector<double> generateSignal(int N, double freq, double sampleRate, std::default_random_engine& generator, double noiseAmplitude = 0.5) {
    std::vector<double> signal(N);
    std::uniform_real_distribution<double> distribution(-0.5, 0.5);
    for (int n = 0; n < N; ++n) {
        // Sine wave component
        signal[n] = sin(2 * PI * freq * n / sampleRate);
        // Add noise
        signal[n] += noiseAmplitude * distribution(generator);
    }
    return signal;
}

// Perform FFT and return the magnitude spectrum
std::vector<double> performFFT(const std::vector<double>& signal) {
    int N = static_cast<int>(signal.size());

    // Allocate FFT input and output arrays
    fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    if (!in || !out) {
        std::cerr << "Failed to allocate FFTW arrays." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Copy real signal into complex input array
    for (int i = 0; i < N; ++i) {
        in[i][0] = signal[i]; // Real part
        in[i][1] = 0.0;       // Imaginary part
    }

    // Create FFT plan
    fftw_plan plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // Calculate magnitude spectrum (only first N/2 points)
    std::vector<double> magnitude(N / 2);
    for (int i = 0; i < N / 2; ++i) {
        magnitude[i] = sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]);
    }

    // Cleanup
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return magnitude;
}

// Detect peaks in the spectrum above a specified threshold
std::vector<Peak> detectPeaks(const std::vector<double>& spectrum, double threshold, int minDistance = 1) {
    std::vector<Peak> peaks;

    for (size_t i = 1; i < spectrum.size() - 1; ++i) {
        if (spectrum[i] > threshold && spectrum[i] > spectrum[i - 1] && spectrum[i] > spectrum[i + 1]) {
            peaks.push_back({static_cast<int>(i), spectrum[i]});
        }
    }

    // Optional: filter peaks to ensure minimum distance (if needed)
    // For simplicity, assuming no filtering here; can be added if necessary.

    return peaks;
}

// Display spectrum data
void displaySpectrum(const std::vector<double>& spectrum, int maxPoints = 50) {
    int step = std::max(1, static_cast<int>(spectrum.size() / maxPoints));
    for (size_t i = 0; i < spectrum.size(); i += step) {
        std::cout << i << ": " << spectrum[i] << std::endl;
    }
}

// Display detected peaks
void displayPeaks(const std::vector<Peak>& peaks) {
    std::cout << "Detected peaks:" << std::endl;
    for (const auto& peak : peaks) {
        std::cout << "Index: " << peak.index << ", Magnitude: " << peak.magnitude << std::endl;
    }
}

int main() {
    // Initialize random engine once
    std::default_random_engine generator(static_cast<unsigned int>(time(nullptr)));

    int sampleRate = 1024;  // Hz
    int N = 1024;           // Number of samples
    double freq = 50.0;     // Signal frequency in Hz

    // Generate signal
    auto signal = generateSignal(N, freq, sampleRate, generator);

    // Perform FFT
    auto magnitudeSpectrum = performFFT(signal);

    // Display spectrum
    std::cout << "Magnitude Spectrum:" << std::endl;
    displaySpectrum(magnitudeSpectrum);

    // Detect multiple peaks above a threshold (e.g., 30% of max magnitude)
    double maxMag = *std::max_element(magnitudeSpectrum.begin(), magnitudeSpectrum.end());
    double threshold = 0.3 * maxMag; // Adjust as needed

    auto peaks = detectPeaks(magnitudeSpectrum, threshold);

    // Display detected peaks
    displayPeaks(peaks);

    return 0;
}
