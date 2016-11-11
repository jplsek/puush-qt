#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class QString;
class QVariant;

class Settings
{
public:
    Settings();

    // generic format of this is <option what/when> <value>
    enum Setting {
        NO_SETTING = 0,
        SETTINGS_VERSION = 1,

        ACCOUNT_API_KEY = 2,
        ACCOUNT_EMAIL = 3,

        ON_PUUSH_SOUND = 4,
        ON_PUUSH_COPY_LINK_TO_CLIPBOARD = 5,
        ON_PUUSH_OPEN_LINK_IN_BROWSER = 6,

        LOCAL_SAVE_ENABLED = 7,
        LOCAL_SAVE_PATH = 8,
        LOCAL_SAVE_NAME = 9,

        TRAY_CLICK_ACTION = 10,

        COMPRESSION_PHILOSOPHY = 11,

        ENABLE_EXPLORER_CONTEXT_MENU = 12,

        FULLSCREEN_CAPTURE_MODE = 13,

        ENABLE_EXPERIMENTAL_FEATURES = 14,

        SHOW_SELECTION_RECTANGLE = 15,

        IMAGE_QUALITY = 16,
    };

    enum RadioValue {
        // tray click action
        OPEN_SETTINGS = 0,
        OPEN_CAPTURE  = 1,
        OPEN_UPLOADS  = 2,

        // compression philosophy
        PNG_ALWAYS = 3,
        JPG_ALWAYS = 4,
        IMAGE_TYPE_SMALLER = 5,

        // fullscreen capture mode
        ALL_DESKTOPS = 6,
        CURRENT_DESKTOP = 7,
        PRIMARY_DESKTOP = 8,
    };

    const static QString CURRENT_SETTINGS_VERSION;

    QVariant value(Setting s);
    bool contains(Setting s);
    void setValue(Setting s, QVariant val);

    bool radio_value_is(Setting s, RadioValue v);

    const static QString radio_values[];

    void resetSettings();
    void setEmptyToDefaults();

private:
    QSettings s;

    const static QString setting_names[];
};

#endif // SETTINGS_H
