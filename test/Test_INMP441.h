#include <Arduino.h>
#include <driver/i2s.h>
#include "arduinoFFT.h"

#define I2S_WS  7   // LRCK
#define I2S_SD  3   // DOUT
#define I2S_SCK 9   // BCLK

#define SAMPLES 512
#define SAMPLING_FREQ 40000

double vReal[SAMPLES];
double vImag[SAMPLES];
ArduinoFFT<double> FFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);


double freqHistory[3] = {0, 0, 0};
const double marge = 25.0;  // Hz
double seuil = 5000;  // règle selon ton micro

void setupI2S() {
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = SAMPLING_FREQ,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S,
    .intr_alloc_flags = 0,
    .dma_buf_count = 4,
    .dma_buf_len = 512,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };

  i2s_pin_config_t pin_config = {
    .bck_io_num = I2S_SCK,
    .ws_io_num = I2S_WS,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = I2S_SD
  };

  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);
}

void setup() {
  Serial.begin(115200);
  setupI2S();

  Serial.println("Micro est pret");
}

void loop() {
  int32_t buffer[SAMPLES];
  size_t bytesRead;
  i2s_read(I2S_NUM_0, &buffer, SAMPLES * sizeof(int32_t), &bytesRead, portMAX_DELAY);

  for (int i = 0; i < SAMPLES; i++) {
    vReal[i] = buffer[i] / 100000.0;  // mise à l’échelle
    vImag[i] = 0;
  }

  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.compute(FFT_FORWARD);
  FFT.complexToMagnitude();

    double maxAmplitude = 0;
    int maxIndex = 0;
    for (int i = 1; i < SAMPLES / 2; i++) {
        if (vReal[i] > maxAmplitude) {
            maxAmplitude = vReal[i];
            maxIndex = i;
        }
    }

    double freq = (maxIndex * SAMPLING_FREQ) / SAMPLES;
    

    if (maxAmplitude > seuil) {
        freqHistory[0] = freqHistory[1];
        freqHistory[1] = freqHistory[2];
        freqHistory[2] = freq;

        if (abs(freqHistory[2] - freqHistory[1]) < marge && abs(freqHistory[1] - freqHistory[0]) < marge) {
            Serial.print("Freq stable : ");
            Serial.print(freq);
            Serial.print(" Hz       --> ");
            Serial.print(maxAmplitude);
            Serial.println(" V");
        }
    } else {
        freqHistory[0] = freqHistory[1] = freqHistory[2] = 0;
    }
  delay(50);
}
