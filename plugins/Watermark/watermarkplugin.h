#ifndef WATERMARKPLUGIN_H
#define WATERMARKPLUGIN_H

#include <QGenericPlugin>
#include "corepluginInterface.h"

class WatermarkPlugin : public QObject, private CorePluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID FlameshotPlugin_iid FILE
                      "Watermark.json")
    Q_INTERFACES(CorePluginInterface)
public:
    explicit WatermarkPlugin(QObject* parent = nullptr);
    virtual ~WatermarkPlugin() override;

    virtual bool load(std::map<std::string, std::string> &PluginConfig) final;
    virtual void unload() final;
    virtual bool ImagePost(QPixmap &pixmap) final;
    virtual bool PrintPre(QPixmap &pixmap) final;
private:
    std::map<std::string, std::string> PluginConfig;
};

#endif // WATERMARKPLUGIN_H
