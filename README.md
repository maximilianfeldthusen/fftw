
## Documentation

### **Overview**

This program generates a synthetic signal composed of a sine wave with added noise, computes its Fast Fourier Transform (FFT) to analyze its frequency components, detects peaks in the spectrum, and displays the results.

---

### **Included Libraries**

- `<iostream>`: for input/output operations.
- `<vector>`: to handle dynamic arrays.
- `<complex>`: for complex number operations (though not directly used here).
- `<fftw3.h>`: FFTW library for performing FFTs.
- `<cmath>`: mathematical functions like `sin()` and `sqrt()`.
- `<cstdlib>`, `<ctime>`, `<random>`: for random number generation.
- `<algorithm>`: for functions like `max_element()`.

---

### **Constants and Data Structures**

- `PI`: a constant for π.
- `struct Peak`: to store information about detected peaks (their index and magnitude).

---

### **Functions**

#### 1. `generateSignal()`
- **Purpose:** Creates a synthetic signal of length `N` samples containing a sine wave of a specified frequency (`freq`) plus random noise.
- **Parameters:**
  - `N`: number of samples.
  - `freq`: sine wave frequency.
  - `sampleRate`: sampling rate in Hz.
  - `generator`: random engine for noise.
  - `noiseAmplitude`: amplitude of added noise (default 0.5).
- **Process:**
  - For each sample `n`, compute `sin(2π * freq * n / sampleRate)`.
  - Add random noise uniformly distributed between -0.5 and 0.5, scaled by `noiseAmplitude`.
- **Returns:** a vector containing the noisy sine wave.

---

#### 2. `performFFT()`
- **Purpose:** Computes the FFT of the input signal and returns the magnitude spectrum.
- **Process:**
  - Allocates memory for FFT input (`in`) and output (`out`) arrays using FFTW.
  - Copies the real input signal into the complex input array (`in`), setting imaginary parts to zero.
  - Creates an FFT plan and executes it.
  - Calculates the magnitude of the first half of the FFT output (since the FFT of real signals is symmetric).
  - Frees FFTW resources.
- **Returns:** a vector of magnitudes representing the spectrum.

---

#### 3. `detectPeaks()`
- **Purpose:** Finds local maxima in the spectrum that are above a specified threshold.
- **Parameters:**
  - `spectrum`: magnitude spectrum.
  - `threshold`: minimum magnitude to consider a peak.
  - `minDistance`: minimum separation between peaks (not fully implemented here).
- **Process:**
  - Iterates through the spectrum (excluding first and last points).
  - Checks if a point is greater than its neighbors and above the threshold.
  - Collects such points as peaks.
- **Returns:** a vector of `Peak` objects.

---

#### 4. `displaySpectrum()`
- **Purpose:** Prints a summarized view of the spectrum, limiting the number of printed points for readability.
- **Parameters:**
  - `spectrum`: the magnitude spectrum.
  - `maxPoints`: maximum number of points to display (default 50).

---

#### 5. `displayPeaks()`
- **Purpose:** Prints information about the detected peaks.

---

### **Main Function Workflow**

1. **Initialize Random Generator:**
   - Seeds with current time for noise randomness.

2. **Parameters:**
   - `sampleRate = 1024 Hz`
   - `N = 1024 samples`
   - `freq = 50 Hz` (signal frequency)

3. **Generate Signal:**
   - Calls `generateSignal()` to create a noisy sine wave.

4. **Compute FFT:**
   - Calls `performFFT()` to get the spectrum.

5. **Display Spectrum:**
   - Calls `displaySpectrum()` to print a summarized spectrum.

6. **Peak Detection:**
   - Finds the maximum magnitude in the spectrum.
   - Sets a threshold at 30% of this maximum.
   - Calls `detectPeaks()` to find peaks above this threshold.

7. **Display Detected Peaks:**
   - Calls `displayPeaks()`.

---

### **Summary**

This code performs a basic spectral analysis:
- Generates a noisy sine wave signal.
- Uses FFT to analyze its frequency content.
- Detects prominent peaks in the spectrum, which correspond to significant frequency components.
- Outputs the spectrum and the peaks for inspection.

This approach is common in signal processing applications for identifying dominant frequencies in noisy signals.

---

**Note:** To run this code, you need to have FFTW installed on your system and compile with the appropriate flags, e.g., `-lfftw3`.
