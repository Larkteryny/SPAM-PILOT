// Online C++ compiler to run C++ program online
#include <iostream>
#include <math.h>

float b0n_0 = 0;
float b1n_0 = 0;
float b2n_0 = 0;
float a1n_0 = 0;
float a2n_0 = 0;
float b0n_1 = 0;
float b1n_1 = 0;
float b2n_1 = 0;
float a1n_1 = 0;
float a2n_1 = 0;

void setup(double notch_freq, double bandwidth, double sample_freq)
{
    if (bandwidth <= 0.0 || sample_freq <= 0.0) {
        return; 
    }

    double Q = notch_freq / bandwidth;
    double omega = 2.0 * M_PI * notch_freq / sample_freq;
    double alpha = sin(omega) / (2.0 * Q);

    double b0 = 1.0;
    double b1 = -2.0 * cos(omega);
    double b2 = 1.0;
    double a0 = 1.0 + alpha;
    double a1 = -2.0 * cos(omega);
    double a2 = 1.0 - alpha;

    // normalize
    b0n_0 = (float)b0 / a0;
    b1n_0 = (float)b1 / a0;
    b2n_0 = (float)b2 / a0;
    a1n_0 = (float)a1 / a0;
    a2n_0 = (float)a2 / a0;   
}

/* Confirmed math is correct without float/double precision disparity
void update(double notch_freq, double bandwidth, double sample_freq_inv)
{
    // doesn't check for invalid bandwidth or sample_freq

    double omega = notch_freq * sample_freq_inv;
    double omega_sin = sin(omega);

    // normalized values
    b0n_1 = (float)(notch_freq / (notch_freq + omega_sin * bandwidth));
    b1n_1 = (float)(-2.0 * cos(omega) * b0n_1);
    b2n_1 = (float)(b0n_1); // could be removed?
    a1n_1 = (float)(b1n_1);
    a2n_1 = (float)((notch_freq - omega_sin * bandwidth) / (notch_freq + omega_sin * bandwidth));
}*/

void update(float notch_freq, float bandwidth, float sample_freq_inv)
/*
Fast notch coefficient updater
const bandwidth_inv: bandwidth / 2.0f
const sample_freq_inv: 2.0f * M_PI / sample_freq
*/
{
    // doesn't check for invalid bandwidth or sample_freq

    float omega = notch_freq * sample_freq_inv;
    float omega_sin = sinf(omega);

    // normalized values
    b0n_1 = notch_freq / (notch_freq + omega_sin * bandwidth);
    b1n_1 = -2.0f * cosf(omega) * b0n_;
    b2n_1 = b0n_1; // could be removed?
    a1n_1 = b1n_1;
    a2n_1 = (notch_freq - omega_sin * bandwidth) / (notch_freq + omega_sin * bandwidth);
}

int main() {
    // Write C++ code here
    std::cout << "Starting" << std::endl;
    
    for(float freq = 1; freq < 100; freq+=1) {
        for(float bandwidth = 1; bandwidth < 20; bandwidth+=2) {
            for(float sample = 100; sample < 2000; sample+=100) {
                setup(freq, bandwidth, sample);
                update(freq, bandwidth / 2.0, 2.0 * M_PI/sample);
                if(std::abs(b0n_0 - b0n_1) > 0.000001 || 
                    std::abs(b1n_0 - b1n_1) > 0.000001 || 
                    std::abs(b2n_0 - b2n_1) > 0.000001 || 
                    std::abs(a1n_0 - a1n_1) > 0.000001 || 
                    std::abs(a2n_0 - a2n_1) > 0.000001) {
                    std::cout << freq << ", " << bandwidth << ", " << sample << std::endl;
                    std::cout << "\n\t" << b0n_0 << "\t" << b0n_1 << "\n\t" << b1n_0 << "\t" << b1n_1 << "\n\t" << b2n_0 << "\t" << b2n_1 << "\n\t" << a1n_0 << "\t" << a1n_1 << "\n\t" << a2n_0 << "\t" << a2n_1 << std::endl;
                }
            }
        }
    }
    std::cout << "Completed" << std::endl;

    return 0;
}


/* Results:
Starting
74, 15, 100

	1.11253	1.11253
	0.139713	0.139712
	1.11253	1.11253
	0.139713	0.139712
	1.22507	1.22507
74, 17, 100

	1.12948	1.12948
	0.141842	0.14184
	1.12948	1.12948
	0.141842	0.14184
	1.25896	1.25896
74, 19, 100

	1.14695	1.14695
	0.144036	0.144035
	1.14695	1.14695
	0.144036	0.144035
	1.29391	1.29391
Completed
*/