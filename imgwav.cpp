//#include "imgwav.h"

/*
imgwav::imgwav()
{

    const double TWO_PI = 2 * M_PI;
    const int SAMPLE_RATE = 44100;
    const int BIT_DEPTH = 16;

}

void imgwav::add_sine(FILE *fpOutputFile, WAV_FILE_INFO WavInfo, float length, std::vector<float> freqs){

    float max_no = pow(2, this->BIT_DEPTH) / 2;
    length *= this->SAMPLE_RATE;

    for(int pos = 0; pos <= length; pos++){

        int count = 0;
        float val = 0;

        for(count = 0; count < freqs.size(); count+=2){

            float time = ((float)pos / this->SAMPLE_RATE) * freqs[count];
            val += sin(TWO_PI * time)*10/(pow(10, freqs[count + 1]));
        }

        val /= count+1;
        float samp = val * max_no;
        double srcData[1] = {samp}; //wav_write_data wants you to pass in an array of doubles but i only want to write one number at a time
        wav_write_data(srcData, fpOutputFile, WavInfo, 1);

    }

}

void imgwav::writeout(QString QFilename, QDir wavOutputDirectory){

    std::string filename = QFilename.toStdString();

    //setup wav file
    WAV_FILE_INFO WavInfo;
    WavInfo.SampleRate = this->SAMPLE_RATE;
    WavInfo.NumberOfChannels = 1;
    WavInfo.NumberOfSamples = 0.2 * SAMPLE_RATE; //temporary value since the real value is calculated at runtime
    WavInfo.WordLength = this->BIT_DEPTH;
    WavInfo.BytesPerSample = this->BIT_DEPTH / 8;
    WavInfo.DataFormat = 1;

    FILE *fpOutputFile;
    if((fpOutputFile = fopen("output.wav", "wb")) == NULL) {
        exit(1);
    }

    wav_write_header(fpOutputFile, WavInfo);

    //load file and store
    int srcW, srcH, srcC;
    unsigned char *data = stbi_load(filename.c_str(), &srcW, &srcH, &srcC, 3); //may seem counterintuitive to go from QString to string to c-string but it avoids a lot of errors this way

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

*/
