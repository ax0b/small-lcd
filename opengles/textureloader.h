#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H
#include <QString>

#include <QImage>
class TextureLoader
{
public:
    static char * loadTexture(const QString& s, int &bpp, int dscale);
    static char * loadRAWTexture(const QString& s, int w, int h, int bpp, int dscale);
    static QImage rawToImage(int bpp, int w, int h, char *data);
    static char * imageToRaw(const QImage &img);
private:
    TextureLoader()
    {}

};

#endif // TEXTURELOADER_H
