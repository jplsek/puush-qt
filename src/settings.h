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

        IMAGE_QUALITY = 13,

        BASE_URL = 14,
        API_URL = 15,
    };

    // these numbers can change.
    enum RadioValue {
        // tray click action
        OPEN_SETTINGS = 0,
        OPEN_UPLOADS = 1,
        UPLOAD_FILE = 2,
        CAPTURE_DESKTOP = 3,
        CAPTURE_AREA = 4,
        CAPTURE_WINDOW = 5,
        OPEN_ACCOUNT = 6,

        // compression philosophy
        PNG_ALWAYS = 7,
        JPG_ALWAYS = 8,
        IMAGE_TYPE_SMALLER = 9,

        // fullscreen capture mode
        ALL_DESKTOPS = 10,
        CURRENT_DESKTOP = 11,
        PRIMARY_DESKTOP = 12,
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
