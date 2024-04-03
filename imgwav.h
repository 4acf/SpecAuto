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
    ~imgwav();
    void writeout(QString, QString, QDir);
private:
    void add_sine(FILE*, WAV_FILE_INFO, float, std::vector<float>);
};

#endif

#endif

#endif
