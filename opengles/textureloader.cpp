#include "textureloader.h"
#include <QImage>
#include <QFile>
#include <QByteArray>


char *TextureLoader::loadTexture(const QString &s, int &bpp, int dscale)
{
    QImage img = QImage(s);
    if (img.isNull())
        return 0;
    int w = img.width();
    int h = img.height();
    if (w*h ==  0)
        return 0;
    bpp = img.byteCount()/(w*h);
    if (bpp == 0)
        return 0;
    if (dscale > 1)
        img = img.scaled(w/dscale, h/dscale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    return imageToRaw(img);
}

QImage TextureLoader::rawToImage(int bpp, int w, int h, char * data)
{
    QImage img = QImage(w, h, (bpp == 3 ? QImage::Format_RGB888 : QImage::Format_RGB888));
    for (int i = 0; i < h; i++) {
        uchar * p = img.scanLine(i);
        for (int j = 0; j < w; j++) {
            for (int k = 0; k < bpp; k++)
                p[j*bpp+k] = data[w*i*bpp + j*bpp+k];
        }
    }
    return img;
}

char *TextureLoader::loadRAWTexture(const QString &s, int w, int h, int bpp, int dscale)
{
    QFile * f = new QFile(s);
    if (!f->open(QIODevice::ReadOnly))
        return 0;
    QByteArray ba = f->readAll();
    f->close();
    delete f;
    if (ba.size() == 0)
        return 0;
    if (dscale > 1) {
        QImage img = rawToImage(bpp, w, h, ba.data()).scaled(w/dscale, h/dscale, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        return imageToRaw(img);
    }
    uchar * data = new uchar[ba.size()];
    memcpy(data, ba.data(), ba.size());
    return (char*) data;
}

char *TextureLoader::imageToRaw(const QImage &img)
{
    char * data = new char[img.byteCount()];
    int bpp = img.byteCount()/(img.width()*img.height());
    for (int i = 0; i < img.height(); i++) {
        const uchar * p = img.scanLine(i);
        for (int j = 0; j < img.width(); j++) {
            for (int k = 0; k < bpp; k++)
                 data[img.width()*i*bpp + j*bpp + k] = p[j*bpp + k];
                 char c = data[img.width()*i*bpp + j*bpp];
		 data[img.width()*i*bpp + j*bpp] = data[img.width()*i*bpp + j*bpp + 2];
		 data[img.width()*i*bpp + j*bpp+2] = c;
	    }
    }
    return data;
}
