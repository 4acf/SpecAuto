#ifndef IMGWAV_H
#define IMGWAV_H

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "wav_file.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <vector>
#include <cmath>
#include <string>

class imgwav
{
public:
    imgwav();
    void writeout(QString, QString, QDir);
private:
    void add_sine(FILE*, WAV_FILE_INFO, float, std::vector<float>);

    double TWO_PI;
    int SAMPLE_RATE;
    int BIT_DEPTH;

};

imgwav::imgwav()
{

    this->TWO_PI = 2 * M_PI;
    this->SAMPLE_RATE = 44100;
    this->BIT_DEPTH = 16;

}

void imgwav::add_sine(FILE *fpOutputFile, WAV_FILE_INFO WavInfo, float length, std::vector<float> freqs){

    float max_no = pow(2, this->BIT_DEPTH) / 2;
    length *= this->SAMPLE_RATE;

    for(int pos = 0; pos <= length; pos++){

        int count = 0;
        float val = 0;

        for(count = 0; count < freqs.size(); count+=2){

            float time = ((float)pos / this->SAMPLE_RATE) * freqs[count];
            val += sin(this->TWO_PI * time)*10/(pow(10, freqs[count + 1]));
        }

        val /= count+1;
        float samp = val * max_no;
        double srcData[1] = {samp}; //wav_write_data wants you to pass in an array of doubles but i only want to write one number at a time
        wav_write_data(srcData, fpOutputFile, WavInfo, 1);

    }

}

void imgwav::writeout(QString tempdirPath, QString QFilename, QDir wavOutputDirectory){

    //setup file path to read images from
    QString inputFilePath = tempdirPath + QString("/") + QFilename;
    std::string filename = inputFilePath.toStdString();

    QFilename.chop(4);

    //setup file output path
    QString outputFilePath = wavOutputDirectory.path() + QString("/") + QFilename;
    std::string output = outputFilePath.toStdString() + ".wav";

    //setup wav file
    WAV_FILE_INFO WavInfo;
    WavInfo.SampleRate = this->SAMPLE_RATE;
    WavInfo.NumberOfChannels = 1;
    WavInfo.NumberOfSamples = 0.2 * this->SAMPLE_RATE; //temporary value since the real value is calculated at runtime
    WavInfo.WordLength = this->BIT_DEPTH;
    WavInfo.BytesPerSample = this->BIT_DEPTH / 8;
    WavInfo.DataFormat = 1;

    FILE *fpOutputFile;
    if((fpOutputFile = fopen(output.c_str(), "wb")) == NULL) { //may seem counterintuitive to go from QString to string to c-string but it avoids a lot of errors this way
        exit(1);
    }

    wav_write_header(fpOutputFile, WavInfo);

    //load file and store
    int srcW = 0, srcH = 0, srcC = 3;
    unsigned char *data = stbi_load(filename.c_str(), &srcW, &srcH, &srcC, 3);

    WavInfo.NumberOfSamples = (0.2 * this->SAMPLE_RATE) * srcW;

    //loop through pixels
    for(int x = 0; x < srcW; x++){

        std::vector<float> t;

        for(int y = 0; y < srcH; y++){

            //get rgb variables
            const unsigned char *pixel = data + (3 * (y * srcW + x));

            int r = (int)pixel[0];
            int g = (int)pixel[1];
            int b = (int)pixel[2];

            if(r > 10 || g > 10 && b > 10){

                float c = 4.25 - 4.25*(r + g + b)/(256*3);

                float percentage = (float)(y+1)/(float)(srcH+1);
                int d = 22000 - (percentage * 22000);

                t.push_back(d);
                t.push_back(c);

            }
        }

        add_sine(fpOutputFile, WavInfo, .2, t);

    }

    //clean up memory allocations and complete the wav header
    stbi_image_free(data);
    wav_write_header(fpOutputFile, WavInfo);
    fclose(fpOutputFile);

}

#endif

#endif

#endif
