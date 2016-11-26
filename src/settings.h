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
    // these numbers can change.
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

        FULLSCREEN_CAPTURE_MODE = 12,

        ENABLE_EXPERIMENTAL_FEATURES = 13,

        SHOW_SELECTION_RECTANGLE = 14,

        IMAGE_QUALITY = 15,

        BASE_URL = 16,
        API_URL = 17,
    };

    // these numbers can change.
    enum RadioValue {
        // tray click action
        OPEN_SETTINGS = 0,
        OPEN_UPLOADS  = 1,

        // compression philosophy
        PNG_ALWAYS = 2,
        JPG_ALWAYS = 3,
        IMAGE_TYPE_SMALLER = 4,

        // fullscreen capture mode
        ALL_DESKTOPS = 5,
        CURRENT_DESKTOP = 6,
        PRIMARY_DESKTOP = 7,
    };

    const static QString CURRENT_SETTINGS_VERSION;

    const static QString GENERAL_GROUP_NAME;
    const static QString ADVANCED_GROUP_NAME;
    const static QString EXTRA_GROUP_NAME;
    const static QString USER_INFO_GROUP_NAME;

    QVariant value(Setting s);
    bool contains(Setting s);
    void setValue(Setting s, QVariant val);
    void setRadioValue(Setting s, RadioValue val);

    bool radioValueIs(Setting s, RadioValue v);

    void resetGeneralSettings();
    void resetAdvancedSettings();
    void resetExtraSettings();
    void resetUserInfoSettings();

    void setEmptyToDefaults();

private:
    QSettings s;

    const static QString setting_names[];
    const static QString radio_values[];
    const static QString groups[];
};

#endif // SETTINGS_H
