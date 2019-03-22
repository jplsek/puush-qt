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
        ACCOUNT_PREMIUM = 4,
        ACCOUNT_PREMIUM_EXPIRY = 5,
        ACCOUNT_DISK_USAGE = 6,

        ON_PUUSH_SOUND = 7,
        ON_PUUSH_COPY_LINK_TO_CLIPBOARD = 8,
        ON_PUUSH_OPEN_LINK_IN_BROWSER = 9,

        LOCAL_SAVE_ENABLED = 10,
        LOCAL_SAVE_PATH = 11,
        LOCAL_SAVE_NAME = 12,

        TRAY_CLICK_ACTION = 13,

        COMPRESSION_PHILOSOPHY = 14,

        FULLSCREEN_CAPTURE_MODE = 15,

        IMAGE_QUALITY = 16,

        BASE_URL = 17,
        API_URL = 18,

        BINDING_UPLOAD_FILE = 19,
        BINDING_CAPTURE_DESKTOP = 20,
        BINDING_CAPTURE_AREA = 21,
        BINDING_CAPTURE_WINDOW = 22,
        BINDING_UPLOAD_CLIPBOARD = 23,
        BINDING_TOGGLE_PUUSH = 24,
    };

    // these numbers can change.
    enum RadioValue {
        // tray click action
        OPEN_SETTINGS = 0,
        OPEN_UPLOADS = 1,
        UPLOAD_FILE = 2,
        UPLOAD_CLIPBOARD = 3,
        CAPTURE_DESKTOP = 4,
        CAPTURE_AREA = 5,
        CAPTURE_WINDOW = 6,
        OPEN_ACCOUNT = 7,

        // compression philosophy
        PNG_ALWAYS = 8,
        JPG_ALWAYS = 9,
        IMAGE_TYPE_SMALLER = 10,

        // fullscreen capture mode
        ALL_DESKTOPS = 11,
        CURRENT_DESKTOP = 12,
        PRIMARY_DESKTOP = 13,
    };

    const static QString CURRENT_SETTINGS_VERSION;

    const static QString GENERAL_GROUP_NAME;
    const static QString ADVANCED_GROUP_NAME;
    const static QString EXTRA_GROUP_NAME;
    const static QString USER_INFO_GROUP_NAME;
    const static QString BINDINGS_GROUP_NAME;

    QVariant value(Setting s);
    bool contains(Setting s);
    void setValue(Setting s, QVariant val);
    QVariant resetValue(Setting s);
    void setRadioValue(Setting s, RadioValue val);

    bool radioValueIs(Setting s, RadioValue v);

    void resetGeneralSettings();
    void resetAdvancedSettings();
    void resetExtraSettings();
    void resetUserInfoSettings();
    void resetBindingsSettings();

    void setEmptyToDefaults();

private:
    QSettings s;

    static std::map<Settings::Setting, QString> setting_names;
    static std::map<Settings::RadioValue, QString> radio_values;
    static std::map<Settings::Setting, QString> groups;
    static std::map<Settings::Setting, QString> default_values;
};

#endif // SETTINGS_H
