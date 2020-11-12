#pragma once

#include "vimframeswitcher_global.hpp"

#include <extensionsystem/iplugin.h>

#include <QPoint>

namespace Core {
class IEditor;
}

namespace VimFrameSwitcher {
namespace Internal {

class VimFrameSwitcherPlugin : public ExtensionSystem::IPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QtCreatorPlugin" FILE "VimFrameSwitcher.json")

    enum LeftRightDir{
        Left,
        Right
    };

    enum UpDownDir {
        Up,
        Down
    };

public:
    VimFrameSwitcherPlugin();
    ~VimFrameSwitcherPlugin() override;

    bool initialize(const QStringList &arguments, QString *errorString) override;
    void extensionsInitialized() override;
    ShutdownFlag aboutToShutdown() override;

private:
    void triggerLeft_h();
    void triggerRight_l();
    void triggerUp_k();
    void triggerDown_j();


    void leftRight(LeftRightDir direction);
    void upDown(UpDownDir direction);

    void getCurrentCursorPos();
    QPoint currentTextCursorPosition;
    QList<Core::IEditor*> editors;
};

} // namespace Internal
} // namespace VimFrameSwitcher

