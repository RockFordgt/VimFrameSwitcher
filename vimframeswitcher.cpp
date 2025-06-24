#include "vimframeswitcher.hpp"
#include "vimframeswitcherconstants.hpp"

#include <coreplugin/actionmanager/actioncontainer.h>
#include <coreplugin/actionmanager/actionmanager.h>
#include <coreplugin/actionmanager/command.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/editormanager/editormanager.h>
#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/icontext.h>
#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <texteditor/texteditor.h>

#include <QTextEdit>
#include <QAction>
#include <QMessageBox>
#include <QMainWindow>
#include <QMenu>
#include <QPoint>

#include <algorithm>
#include <map>

namespace VimFrameSwitcher {
namespace Internal {

VimFrameSwitcherPlugin::VimFrameSwitcherPlugin()
{
    // Create your members
}

VimFrameSwitcherPlugin::~VimFrameSwitcherPlugin()
{
    // Unregister objects from the plugin manager's object pool
    // Delete members
}

Utils::Result<> VimFrameSwitcherPlugin::initialize(const QStringList &arguments)
{
    // Register objects in the plugin manager's object pool
    // Load settings
    // Add actions to menus
    // Connect to other plugins' signals
    // In the initialize function, a plugin can be sure that the plugins it
    // depends on have initialized their members.

    Q_UNUSED(arguments)

    auto action_left = new QAction(tr("Jump to left"), this);
    auto action_right = new QAction(tr("Jump to right"), this);
    auto action_up = new QAction(tr("Jump to up"), this);
    auto action_down = new QAction(tr("Jump to down"), this);

    Core::Command *cmd_left = Core::ActionManager::registerAction(action_left,  Constants::ACTION_LEFT_ID, Core::Context(Core::Constants::C_GLOBAL));
    Core::Command *cmd_right= Core::ActionManager::registerAction(action_right, Constants::ACTION_RIGHT_ID, Core::Context(Core::Constants::C_GLOBAL));
    Core::Command *cmd_up   = Core::ActionManager::registerAction(action_up,    Constants::ACTION_UP_ID, Core::Context(Core::Constants::C_GLOBAL));
    Core::Command *cmd_down = Core::ActionManager::registerAction(action_down,  Constants::ACTION_DOWN_ID, Core::Context(Core::Constants::C_GLOBAL));

    cmd_left->setDefaultKeySequences(QList<QKeySequence>({
                                                             QKeySequence(tr("Ctrl+E,H")),
                                                             QKeySequence(tr("Alt+H")),
                                                         }));

    cmd_right->setDefaultKeySequences(QList<QKeySequence>({
                                                              QKeySequence(tr("Ctrl+E,L")),
                                                              QKeySequence(tr("Alt+L")),
                                                          }));
    cmd_up->setDefaultKeySequences(QList<QKeySequence>({
                                                           QKeySequence(tr("Ctrl+E,K")),
                                                           QKeySequence(tr("Alt+K")),
                                                       }));
    cmd_down->setDefaultKeySequences(QList<QKeySequence>({
                                                             QKeySequence(tr("Ctrl+E,J")),
                                                             QKeySequence(tr("Alt+J")),
                                                         }));

    connect(action_left,  &QAction::triggered, this, &VimFrameSwitcherPlugin::triggerLeft_h);
    connect(action_right, &QAction::triggered, this, &VimFrameSwitcherPlugin::triggerRight_l);
    connect(action_up,    &QAction::triggered, this, &VimFrameSwitcherPlugin::triggerUp_k);
    connect(action_down,  &QAction::triggered, this, &VimFrameSwitcherPlugin::triggerDown_j);

    Core::ActionContainer *menu = Core::ActionManager::createMenu(Constants::MENU_ID);
    menu->menu()->setTitle(tr("VimFrameSwitcher"));
    menu->addAction(cmd_left);
    menu->addAction(cmd_right);
    menu->addAction(cmd_up);
    menu->addAction(cmd_down);

    Core::ActionManager::actionContainer(Core::Constants::M_TOOLS)->addMenu(menu);

    return Utils::ResultOk;
}

void VimFrameSwitcherPlugin::extensionsInitialized()
{
    // Retrieve objects from the plugin manager's object pool
    // In the extensionsInitialized function, a plugin can be sure that all
    // plugins that depend on it are completely initialized.
}

ExtensionSystem::IPlugin::ShutdownFlag VimFrameSwitcherPlugin::aboutToShutdown()
{
    // Save settings
    // Disconnect from signals that are not needed during shutdown
    // Hide UI (if you add UI that is not in the main window directly)
    return SynchronousShutdown;
}

void VimFrameSwitcherPlugin::triggerLeft_h()
{
    leftRight(Left);
}

void VimFrameSwitcherPlugin::triggerRight_l()
{
    leftRight(Right);
}

void VimFrameSwitcherPlugin::triggerUp_k()
{
    upDown(Up);
}

void VimFrameSwitcherPlugin::triggerDown_j()
{
    upDown(Down);
}

void VimFrameSwitcherPlugin::leftRight(VimFrameSwitcherPlugin::LeftRightDir direction)
{
    getCurrentCursorPos();
    std::map<int, Core::IEditor*> jumpsMap;
    for (auto editor: editors){
        if (editor != Core::EditorManager::currentEditor()) {
            QRect editorRect(editor->widget()->mapToGlobal(editor->widget()->rect().topLeft()),
                             editor->widget()->mapToGlobal(editor->widget()->rect().bottomRight()));

            if ( ( editorRect.y() < currentTextCursorPosition.y() ) && (editorRect.bottom()> currentTextCursorPosition.y()) ) {
                int newDistance=currentTextCursorPosition.x() - editorRect.left();
                switch (direction) {
                case Left:
                    if (newDistance > 0) {
                        jumpsMap[newDistance] = editor;
                    }
                    break;
                case Right:
                    if (newDistance < 0) {
                        jumpsMap[newDistance] = editor;
                    }
                }
            }
        }
    }
    if (jumpsMap.size()>0){
        switch (direction) {
        case Left:
            Core::EditorManager::activateEditor(jumpsMap.begin()->second);
            break;
        case Right:
            Core::EditorManager::activateEditor(jumpsMap.rbegin()->second);
        }
    }

}

void VimFrameSwitcherPlugin::upDown(VimFrameSwitcherPlugin::UpDownDir direction)
{
    getCurrentCursorPos();
    std::map<int, Core::IEditor*> jumpsMap;
    for (auto editor: editors){
        if (editor != Core::EditorManager::currentEditor()) {
            QRect editorRect(editor->widget()->mapToGlobal(editor->widget()->rect().topLeft()),
                             editor->widget()->mapToGlobal(editor->widget()->rect().bottomRight()));
            if ( ( editorRect.x() < currentTextCursorPosition.x() ) && (editorRect.right()> currentTextCursorPosition.x()) ) {
                int newDistance=currentTextCursorPosition.y() - editorRect.bottom();
                switch (direction) {
                case Up:
                    if (newDistance > 0) {
                        jumpsMap[newDistance] = editor;
                    }
                    break;
                case Down:
                    if (newDistance < 0) {
                        jumpsMap[newDistance] = editor;
                    }
                }
            }
        }
    }
    if (jumpsMap.size()>0){
        switch (direction) {
        case Up:
            Core::EditorManager::activateEditor(jumpsMap.begin()->second);
            break;
        case Down:
            Core::EditorManager::activateEditor(jumpsMap.rbegin()->second);
        }
    }
}

void VimFrameSwitcherPlugin::getCurrentCursorPos()
{
    Core::IEditor* current = Core::EditorManager::currentEditor();
    TextEditor::BaseTextEditor *textEdit = qobject_cast<TextEditor::BaseTextEditor*>(current);
    if (textEdit){
        currentTextCursorPosition =  current->widget()->mapToGlobal( textEdit->editorWidget()->cursorRect(textEdit->textCursor()).topLeft() );
        editors = Core::EditorManager::visibleEditors();
    }else{
        currentTextCursorPosition = QPoint(0,0);
    }
}


} // namespace Internal
} // namespace VimFrameSwitcher
