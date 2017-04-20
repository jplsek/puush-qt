#include "keybindings.h"

#include <QDebug>
#include <QKeyEvent>

#include "settings.h"

KeyBindings::KeyBindings(QObject *parent) : QObject(parent) {
    modifierKeys << Qt::Key_Shift << Qt::Key_Control << Qt::Key_Meta << Qt::Key_Alt << Qt::Key_AltGr;
    hotkeyManager = new UGlobalHotkeys();

    connect(hotkeyManager, &UGlobalHotkeys::activated, [=](size_t id) {
        switch(id) {
        case Settings::BINDING_UPLOAD_FILE:
            emit signalUploadFile();
            break;
        case Settings::BINDING_CAPTURE_DESKTOP:
            emit signalCaptureDesktop();
            break;
        case Settings::BINDING_CAPTURE_AREA:
            emit signalCaptureArea();
            break;
        case Settings::BINDING_CAPTURE_WINDOW:
            emit signalCaptureWindow();
            break;
        case Settings::BINDING_UPLOAD_CLIPBOARD:
            emit signalUploadClipboard();
            break;
        case Settings::BINDING_TOGGLE_PUUSH:
            emit signalTogglePuush();
            break;
        default:
             qDebug() << "Unknown binding: " << id;
        }
    });

    registerSequence(Settings::BINDING_UPLOAD_FILE);
    registerSequence(Settings::BINDING_CAPTURE_DESKTOP);
    registerSequence(Settings::BINDING_CAPTURE_AREA);
    registerSequence(Settings::BINDING_CAPTURE_WINDOW);
    registerSequence(Settings::BINDING_UPLOAD_CLIPBOARD);
    registerSequence(Settings::BINDING_TOGGLE_PUUSH);
}

QString KeyBindings::getKeySequence(int modifiers, int key) {
    if (modifierKeys.contains(key))
        return QKeySequence(modifiers).toString(QKeySequence::NativeText);
    else
        return QKeySequence(modifiers | key).toString(QKeySequence::NativeText);
}

bool KeyBindings::isProperBinding(int modifiers, int key) {
    return key != 0 && modifiers !=  0 && !modifierKeys.contains(key);
}

void KeyBindings::registerSequence(Settings::Setting key) {
    hotkeyManager->registerHotkey(s.value(key).toString(), key);
}

QString KeyBindings::getUploadFile() {
    return s.value(Settings::BINDING_UPLOAD_FILE).toString();
}

QString KeyBindings::getCaptureDesktop() {
    return s.value(Settings::BINDING_CAPTURE_DESKTOP).toString();
}

QString KeyBindings::getCaptureArea() {
    return s.value(Settings::BINDING_CAPTURE_AREA).toString();
}

QString KeyBindings::getCaptureWindow() {
    return s.value(Settings::BINDING_CAPTURE_WINDOW).toString();
}

QString KeyBindings::getUploadClipboard() {
    return s.value(Settings::BINDING_UPLOAD_CLIPBOARD).toString();
}

QString KeyBindings::getTogglePuush() {
    return s.value(Settings::BINDING_TOGGLE_PUUSH).toString();
}

void KeyBindings::setUploadFile(QString combo) {
    s.setValue(Settings::BINDING_UPLOAD_FILE, combo);
    registerSequence(Settings::BINDING_UPLOAD_FILE);
}

void KeyBindings::setCaptureDesktop(QString combo) {
    s.setValue(Settings::BINDING_CAPTURE_DESKTOP, combo);
    registerSequence(Settings::BINDING_CAPTURE_DESKTOP);
}

void KeyBindings::setCaptureArea(QString combo) {
    s.setValue(Settings::BINDING_CAPTURE_AREA, combo);
    registerSequence(Settings::BINDING_CAPTURE_AREA);
}

void KeyBindings::setCaptureWindow(QString combo) {
    s.setValue(Settings::BINDING_CAPTURE_WINDOW, combo);
    registerSequence(Settings::BINDING_CAPTURE_WINDOW);
}

void KeyBindings::setUploadClipboard(QString combo) {
    s.setValue(Settings::BINDING_UPLOAD_CLIPBOARD, combo);
    registerSequence(Settings::BINDING_UPLOAD_CLIPBOARD);
}

void KeyBindings::setTogglePuush(QString combo) {
    s.setValue(Settings::BINDING_TOGGLE_PUUSH, combo);
    registerSequence(Settings::BINDING_TOGGLE_PUUSH);
}

QString KeyBindings::resetUploadFile() {
    QString combo = s.resetValue(Settings::BINDING_UPLOAD_FILE).toString();
    registerSequence(Settings::BINDING_UPLOAD_FILE);
    return combo;
}

QString KeyBindings::resetCaptureDesktop() {
    QString combo = s.resetValue(Settings::BINDING_CAPTURE_DESKTOP).toString();
    registerSequence(Settings::BINDING_CAPTURE_DESKTOP);
    return combo;
}

QString KeyBindings::resetCaptureArea() {
    QString combo = s.resetValue(Settings::BINDING_CAPTURE_AREA).toString();
    registerSequence(Settings::BINDING_CAPTURE_AREA);
    return combo;
}

QString KeyBindings::resetCaptureWindow() {
    QString combo = s.resetValue(Settings::BINDING_CAPTURE_WINDOW).toString();
    registerSequence(Settings::BINDING_CAPTURE_WINDOW);
    return combo;
}

QString KeyBindings::resetUploadClipboard() {
    QString combo = s.resetValue(Settings::BINDING_UPLOAD_CLIPBOARD).toString();
    registerSequence(Settings::BINDING_UPLOAD_CLIPBOARD);
    return combo;
}

QString KeyBindings::resetTogglePuush() {
    QString combo = s.resetValue(Settings::BINDING_TOGGLE_PUUSH).toString();
    registerSequence(Settings::BINDING_TOGGLE_PUUSH);
    return combo;
}
