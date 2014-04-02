#include "ogographview.h"
#include "ogographviewplugin.h"

#include <QtCore/QtPlugin>

OGoGraphViewPlugin::OGoGraphViewPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void OGoGraphViewPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;
    
    // Add extension registrations, etc. here
    
    m_initialized = true;
}

bool OGoGraphViewPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *OGoGraphViewPlugin::createWidget(QWidget *parent)
{
    return new OGoGraphView(parent);
}

QString OGoGraphViewPlugin::name() const
{
    return "OGoGraphView";
}

QString OGoGraphViewPlugin::group() const
{
    return "";
}

QIcon OGoGraphViewPlugin::icon() const
{
    return QIcon();
}

QString OGoGraphViewPlugin::toolTip() const
{
    return QLatin1String("");
}

QString OGoGraphViewPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool OGoGraphViewPlugin::isContainer() const
{
    return false;
}

QString OGoGraphViewPlugin::domXml() const
{
    return QLatin1String(
         "<ui language=\"c++\">\n"
         "  <widget class=\"OGoGraphView\" name=\"oGoGraphView\">\n"
         "  </widget>\n"
         "</ui>\n"
    );
}

QString OGoGraphViewPlugin::includeFile() const
{
    return QLatin1String("ogographview.h");
}

//Q_EXPORT_PLUGIN2(ogographviewplugin, OGoGraphViewPlugin)
