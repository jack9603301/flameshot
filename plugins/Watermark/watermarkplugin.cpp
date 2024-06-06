#include "watermarkplugin.h"
#include <QPixmap>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QColor>
#include <QFileInfo>

WatermarkPlugin::WatermarkPlugin(QObject* parent)
  : QObject(parent) {

}

bool WatermarkPlugin::load(std::map<std::string, std::string> &PluginConfig) {
    this->PluginConfig = PluginConfig;
    return true;
}


void WatermarkPlugin::unload() {
    return;
}

bool WatermarkPlugin::ImagePost(QPixmap &pixmap) {
    if(!QFileInfo(this->PluginConfig["Watemark"].c_str()).exists()) {
        return false;
    }
    QImage ForeGroundImage(this->PluginConfig["Watemark"].c_str());
    QScreen* screen = QGuiApplication::primaryScreen();
    QSize imgfg_size = ForeGroundImage.size();
    QImage ForeGround(screen->size(), ForeGroundImage.format());
    if((screen->size().width() > imgfg_size.width())||
        (screen->size().height() > imgfg_size.height())) {
        QPainter paint(&ForeGround);
        if(screen->size().width() > imgfg_size.width()) {
            int x = 0;
            while(x+imgfg_size.width() < screen->size().width()) {
                QRect rect(x, 0, imgfg_size.width(), imgfg_size.height());
                paint.drawImage(rect,ForeGroundImage);
                x+= imgfg_size.width();
            }
            QRect rect(x, 0, screen->size().width(), imgfg_size.height());
            if(x < screen->size().width()) {
                paint.drawImage(rect,ForeGroundImage);
            }
            ForeGroundImage = ForeGround.copy(rect);
            imgfg_size = ForeGroundImage.size();
        }
        if(screen->size().height() > imgfg_size.height()) {
            int y = 0;
            while(y+imgfg_size.height() < screen->size().height()) {
                QRect rect(0, y, imgfg_size.width(), imgfg_size.height());
                paint.drawImage(rect,ForeGroundImage);
                y+= imgfg_size.height();
            }
            QRect rect(0, y, imgfg_size.width(), screen->size().height());
            if(y < screen->size().height()) {
                paint.drawImage(rect,ForeGroundImage);
            }
        }
    } else {
        ForeGround = ForeGroundImage;
    }
    imgfg_size = ForeGround.size();

    QSize imgbg_size = pixmap.toImage().size();

    QRect ResultFGRect((screen->size().width() - imgbg_size.width())/2, (screen->size().height() - imgbg_size.height())/2,
                       pixmap.size().width(), pixmap.height());

    QImage ResultFG = ForeGround.copy(ResultFGRect);
    QImage ResultBG = pixmap.toImage();
    bool AddBottomImage = false;
    QSize ResultFullSize(ResultBG.size().width(), ResultBG.size().height());
    QImage ButtonImageRaw;
    if(QFileInfo(this->PluginConfig["Bottom"].c_str()).exists()) {
        AddBottomImage = true;
        ButtonImageRaw = QImage(this->PluginConfig["Bottom"].c_str());
        double wradio = ResultBG.size().width() / 1920.0;
        double hradio = ResultBG.size().height() / 1080.0;
        int genwidth = ButtonImageRaw.size().width()*wradio;
        int genheight = ButtonImageRaw.size().height() * hradio;
        ButtonImageRaw = ButtonImageRaw.scaled(genwidth, genheight, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
        QSize genSize(ResultFullSize.width(), ButtonImageRaw.size().height());
        QImage genBottom(genSize, QImage::Format_RGBA8888_Premultiplied);
        QPainter paint(&genBottom);
        QRect rect(0, 0, genSize.width(), genSize.height());
        paint.fillRect(rect, QBrush(QColor(this->PluginConfig["ColorBG"].c_str())));
        rect.setLeft((genSize.width() - ButtonImageRaw.size().width()) / 2);
        rect.setWidth(ButtonImageRaw.size().width());
        paint.drawImage(rect, ButtonImageRaw);
        ButtonImageRaw = genBottom;
        ResultFullSize.setHeight(ResultBG.size().height() + ButtonImageRaw.size().height());
    }
    QImage BackGround(ResultFullSize, QImage::Format_RGBA8888_Premultiplied);
    QPainter paint(&BackGround);
    QRect ResultSize(0, 0, ResultBG.size().width(), ResultBG.size().height());
    paint.drawImage(ResultSize, ResultBG);
    paint.drawImage(ResultSize, ResultFG);
    if (AddBottomImage) {
        ResultSize.setLeft(0);
        ResultSize.setTop(ResultBG.size().height());
        ResultSize.setWidth(ResultBG.size().width());
        ResultSize.setHeight(ButtonImageRaw.size().height());
        paint.drawImage(ResultSize, ButtonImageRaw);
    }

    pixmap = QPixmap::fromImage(BackGround);
    return true;
}

bool WatermarkPlugin::PrintPre(QPixmap &pixmap) {
    if(!QFileInfo(this->PluginConfig["PrintBG"].c_str()).exists()) {
        return false;
    }
    QImage PrintBG(this->PluginConfig["PrintBG"].c_str());
    QScreen* screen = QGuiApplication::primaryScreen();
    PrintBG = PrintBG.scaled(screen->size().width() + 600, screen->size().height() + 600, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPainter paint(&PrintBG);
    QSize PrintSize=PrintBG.size();
    QRect rect((PrintSize.width() - pixmap.size().width()) /2,
               (PrintSize.height() - pixmap.size().height()) /2,
               pixmap.size().width(), pixmap.size().height());
    paint.drawPixmap(rect, pixmap);
    pixmap = QPixmap::fromImage(PrintBG);
    return true;
}

WatermarkPlugin::~WatermarkPlugin() {

}
