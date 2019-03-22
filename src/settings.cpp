#include "settings.h"

#include <QDesktopServices>
#include <QSettings>
#include <QString>
#include <QVariant>

// [general] -> [basic] because it was conflicting with QSettings [general]
const QString Settings::GENERAL_GROUP_NAME   = "basic";
const QString Settings::ADVANCED_GROUP_NAME  = "advanced";
const QString Settings::EXTRA_GROUP_NAME     = "extra";
const QString Settings::USER_INFO_GROUP_NAME = "user-info";
const QString Settings::BINDINGS_GROUP_NAME  = "key-bindings";

// generic format of this is <option what/when> <value>
// These values should not change; as it will break peoples saved settings.
std::map<Settings::Setting, QString> Settings::setting_names = {
    {NO_SETTING, ""},

    {SETTINGS_VERSION, "settings-version"},

    {ACCOUNT_API_KEY, "key"},
    {ACCOUNT_EMAIL, "email"},
    {ACCOUNT_PREMIUM, "premium"},
    {ACCOUNT_PREMIUM_EXPIRY, "premium-expiry"},
    {ACCOUNT_DISK_USAGE, "disk-usage"},

    {ON_PUUSH_SOUND, "sound-on-puush"},
    {ON_PUUSH_COPY_LINK_TO_CLIPBOARD, "copy-link-to-clipboard"},
    {ON_PUUSH_OPEN_LINK_IN_BROWSER, "open-link-in-browser"},

    {LOCAL_SAVE_ENABLED, "local-save-enabled"},
    {LOCAL_SAVE_PATH, "local-save-path"},
    {LOCAL_SAVE_NAME, "local-save-name"},

    {TRAY_CLICK_ACTION, "tray-click-action"},

    {COMPRESSION_PHILOSOPHY, "compression-philosophy"},

    {FULLSCREEN_CAPTURE_MODE, "fullscreen-capture-mode"},

    {IMAGE_QUALITY, "quality"},

    {BASE_URL, "base-url"},
    {API_URL, "api-url"},

    {BINDING_UPLOAD_FILE, "upload-file-key-binding"},
    {BINDING_CAPTURE_DESKTOP, "capture-desktop-key-binding"},
    {BINDING_CAPTURE_AREA, "capture-area-key-binding"},
    {BINDING_CAPTURE_WINDOW, "capture-window-key-binding"},
    {BINDING_UPLOAD_CLIPBOARD, "upload-clipboard-key-binding"},
    {BINDING_TOGGLE_PUUSH, "toggle-puush-key-binding"},
};

std::map<Settings::Setting, QString> Settings::default_values = {
    {NO_SETTING, ""},

    // YYYY-MM-DD-revision
    {SETTINGS_VERSION, "2017-05-02-00"},

    {ACCOUNT_API_KEY, ""},
    {ACCOUNT_EMAIL, ""},
    {ACCOUNT_PREMIUM, ""},
    {ACCOUNT_PREMIUM_EXPIRY, ""},
    {ACCOUNT_DISK_USAGE, ""},

    {ON_PUUSH_SOUND, "false"},
    {ON_PUUSH_COPY_LINK_TO_CLIPBOARD, "true"},
    {ON_PUUSH_OPEN_LINK_IN_BROWSER, "false"},

    {LOCAL_SAVE_ENABLED, "true"},
    {LOCAL_SAVE_PATH, QStandardPaths::writableLocation(QStandardPaths::PicturesLocation)},
    {LOCAL_SAVE_NAME, "'ss' (yyyy-MM-dd 'at' hh.mm.ss)"},

    {TRAY_CLICK_ACTION, ""},

    {COMPRESSION_PHILOSOPHY, ""},

    {FULLSCREEN_CAPTURE_MODE, ""},

    {IMAGE_QUALITY, "90"},

    {BASE_URL, "https://puush.me/"},
    {API_URL, "https://puush.me/api/"},

    {BINDING_UPLOAD_FILE, "Ctrl+Shift+F1"},
    {BINDING_CAPTURE_DESKTOP, "Ctrl+Shift+F3"},
    {BINDING_CAPTURE_AREA, "Ctrl+Shift+F4"},
    {BINDING_CAPTURE_WINDOW, "Ctrl+Shift+F2"},
    {BINDING_UPLOAD_CLIPBOARD, "Ctrl+Shift+F5"},
    {BINDING_TOGGLE_PUUSH, "Ctrl+Shift+F6"},
};

std::map<Settings::Setting, QString> Settings::groups = {
    {NO_SETTING, Settings::EXTRA_GROUP_NAME},

    {SETTINGS_VERSION, Settings::EXTRA_GROUP_NAME},

    {ACCOUNT_API_KEY, Settings::USER_INFO_GROUP_NAME},
    {ACCOUNT_EMAIL, Settings::USER_INFO_GROUP_NAME},
    {ACCOUNT_PREMIUM, Settings::USER_INFO_GROUP_NAME},
    {ACCOUNT_PREMIUM_EXPIRY, Settings::USER_INFO_GROUP_NAME},
    {ACCOUNT_DISK_USAGE, Settings::USER_INFO_GROUP_NAME},

    {ON_PUUSH_SOUND, Settings::GENERAL_GROUP_NAME},
    {ON_PUUSH_COPY_LINK_TO_CLIPBOARD, Settings::GENERAL_GROUP_NAME},
    {ON_PUUSH_OPEN_LINK_IN_BROWSER, Settings::GENERAL_GROUP_NAME},

    {LOCAL_SAVE_ENABLED, Settings::GENERAL_GROUP_NAME},
    {LOCAL_SAVE_PATH, Settings::GENERAL_GROUP_NAME},
    {LOCAL_SAVE_NAME, Settings::GENERAL_GROUP_NAME},

    {TRAY_CLICK_ACTION, Settings::GENERAL_GROUP_NAME},

    {COMPRESSION_PHILOSOPHY, Settings::ADVANCED_GROUP_NAME},

    {FULLSCREEN_CAPTURE_MODE, Settings::ADVANCED_GROUP_NAME},

    {IMAGE_QUALITY, Settings::ADVANCED_GROUP_NAME},

    {BASE_URL, Settings::EXTRA_GROUP_NAME},
    {API_URL, Settings::EXTRA_GROUP_NAME},

    {BINDING_UPLOAD_FILE, Settings::BINDINGS_GROUP_NAME},
    {BINDING_CAPTURE_DESKTOP, Settings::BINDINGS_GROUP_NAME},
    {BINDING_CAPTURE_AREA, Settings::BINDINGS_GROUP_NAME},
    {BINDING_CAPTURE_WINDOW, Settings::BINDINGS_GROUP_NAME},
    {BINDING_UPLOAD_CLIPBOARD, Settings::BINDINGS_GROUP_NAME},
    {BINDING_TOGGLE_PUUSH, Settings::BINDINGS_GROUP_NAME},
};

std::map<Settings::RadioValue, QString> Settings::radio_values = {
    {OPEN_SETTINGS, "open-settings"},
    {OPEN_UPLOADS, "open-uploads"},
    {UPLOAD_FILE, "upload-file"},
    {UPLOAD_CLIPBOARD, "upload-clipboard"},
    {CAPTURE_DESKTOP, "capture-desktop"},
    {CAPTURE_AREA, "capture-screen"},
    {CAPTURE_WINDOW, "capture-window"},
    {OPEN_ACCOUNT, "open-account"},

    {PNG_ALWAYS, "png-always"},
    {JPG_ALWAYS, "jpg-always"},
    {IMAGE_TYPE_SMALLER, "smart"},

    {ALL_DESKTOPS, "all-desktops"},
    {CURRENT_DESKTOP, "current-desktop"},
    {PRIMARY_DESKTOP, "primary-desktop"},
};

Settings::Settings()
{
    if (s.value(setting_names[SETTINGS_VERSION]).toString() != default_values[SETTINGS_VERSION]) {
        // FIXME: show 'popup' saying some settings may have been reset due to settings key/value changes
        // <ok ill check> <bother me later>
        // if they say ok, then update the settings version. otherwise dont so this will be run again.
        // For now, just update it as jeremy and I know whats up.
        s.beginGroup(EXTRA_GROUP_NAME);
        s.setValue(setting_names[SETTINGS_VERSION], default_values[SETTINGS_VERSION]);
    }
}


QVariant Settings::value(Setting s)
{
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    return this->s.value(setting_names[s]);
}

bool Settings::contains(Setting s)
{
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    return this->s.contains(setting_names[s]);
}

void Settings::setValue(Setting s, QVariant val)
{
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    this->s.setValue(setting_names[s], val);
}

void Settings::setRadioValue(Setting s, RadioValue val)
{
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    this->s.setValue(setting_names[s], radio_values[val]);
}

bool Settings::radioValueIs(Setting s, RadioValue v)
{
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    return this->s.value(setting_names[s]).toString() == radio_values[v];
}

void Settings::resetGeneralSettings()
{
    setValue(ON_PUUSH_SOUND, default_values[ON_PUUSH_SOUND]);
    setValue(ON_PUUSH_COPY_LINK_TO_CLIPBOARD, default_values[ON_PUUSH_COPY_LINK_TO_CLIPBOARD]);
    setValue(ON_PUUSH_OPEN_LINK_IN_BROWSER, default_values[ON_PUUSH_OPEN_LINK_IN_BROWSER]);

    setValue(LOCAL_SAVE_ENABLED, default_values[LOCAL_SAVE_ENABLED]);
    setValue(LOCAL_SAVE_PATH, default_values[LOCAL_SAVE_PATH]);
    setValue(LOCAL_SAVE_NAME, default_values[LOCAL_SAVE_NAME]);

    setRadioValue(TRAY_CLICK_ACTION, OPEN_SETTINGS);
}

void Settings::resetAdvancedSettings()
{
    setRadioValue(COMPRESSION_PHILOSOPHY, PNG_ALWAYS);

    setRadioValue(FULLSCREEN_CAPTURE_MODE, ALL_DESKTOPS);

    setValue(IMAGE_QUALITY, default_values[IMAGE_QUALITY]);
}

void Settings::resetExtraSettings()
{
    s.endGroup();
    s.beginGroup(EXTRA_GROUP_NAME);
    s.setValue(setting_names[BASE_URL], default_values[BASE_URL]);
    s.setValue(setting_names[API_URL],  default_values[API_URL]);
    s.setValue(setting_names[SETTINGS_VERSION], default_values[SETTINGS_VERSION]);
}

void Settings::resetUserInfoSettings()
{
    s.endGroup();
    s.beginGroup(USER_INFO_GROUP_NAME);
    s.setValue(setting_names[ACCOUNT_EMAIL],   default_values[ACCOUNT_EMAIL]);
    s.setValue(setting_names[ACCOUNT_API_KEY], default_values[ACCOUNT_API_KEY]);
}

void Settings::resetBindingsSettings()
{
    s.endGroup();
    s.beginGroup(BINDINGS_GROUP_NAME);
    s.setValue(setting_names[BINDING_UPLOAD_FILE],      default_values[BINDING_UPLOAD_FILE]);
    s.setValue(setting_names[BINDING_CAPTURE_DESKTOP],  default_values[BINDING_CAPTURE_DESKTOP]);
    s.setValue(setting_names[BINDING_CAPTURE_AREA],     default_values[BINDING_CAPTURE_AREA]);
    s.setValue(setting_names[BINDING_CAPTURE_WINDOW],   default_values[BINDING_CAPTURE_WINDOW]);
    s.setValue(setting_names[BINDING_UPLOAD_CLIPBOARD], default_values[BINDING_UPLOAD_CLIPBOARD]);
    s.setValue(setting_names[BINDING_TOGGLE_PUUSH],     default_values[BINDING_TOGGLE_PUUSH]);
}

QVariant Settings::resetValue(Setting s)
{
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    this->s.setValue(setting_names[s], default_values[s]);
    return value(s);
}

void Settings::setEmptyToDefaults()
{
    // General

    if (!contains(ON_PUUSH_SOUND))
        setValue(ON_PUUSH_SOUND, default_values[ON_PUUSH_SOUND]);
    if (!contains(ON_PUUSH_COPY_LINK_TO_CLIPBOARD))
        setValue(ON_PUUSH_COPY_LINK_TO_CLIPBOARD, default_values[ON_PUUSH_COPY_LINK_TO_CLIPBOARD]);
    if (!contains(ON_PUUSH_OPEN_LINK_IN_BROWSER))
        setValue(ON_PUUSH_OPEN_LINK_IN_BROWSER, default_values[ON_PUUSH_OPEN_LINK_IN_BROWSER]);

    if (!contains(LOCAL_SAVE_ENABLED))
        setValue(LOCAL_SAVE_ENABLED, default_values[LOCAL_SAVE_ENABLED]);
    if (!contains(LOCAL_SAVE_PATH) || s.value(setting_names[LOCAL_SAVE_PATH]).toString() == "")
        setValue(LOCAL_SAVE_PATH, default_values[LOCAL_SAVE_PATH]);
    if (!contains(LOCAL_SAVE_NAME))
        setValue(LOCAL_SAVE_NAME, default_values[LOCAL_SAVE_NAME]);

    if (!contains(TRAY_CLICK_ACTION))
        setRadioValue(TRAY_CLICK_ACTION, OPEN_SETTINGS);

    // Advanced

    if (!contains(COMPRESSION_PHILOSOPHY))
        setRadioValue(COMPRESSION_PHILOSOPHY, PNG_ALWAYS);

    if (!contains(FULLSCREEN_CAPTURE_MODE))
        setRadioValue(FULLSCREEN_CAPTURE_MODE, ALL_DESKTOPS);

    if (!contains(IMAGE_QUALITY))
        setValue(IMAGE_QUALITY, default_values[IMAGE_QUALITY]);

    // Extra

    if (!contains(SETTINGS_VERSION))
        setValue(SETTINGS_VERSION, default_values[SETTINGS_VERSION]);

    if (!contains(BASE_URL))
        setValue(BASE_URL, default_values[BASE_URL]);
    if (!contains(API_URL))
        setValue(API_URL,  default_values[API_URL]);

    // User info

    if (!contains(ACCOUNT_EMAIL))
        setValue(ACCOUNT_EMAIL, default_values[ACCOUNT_EMAIL]);
    if (!contains(ACCOUNT_API_KEY))
        setValue(ACCOUNT_API_KEY, default_values[ACCOUNT_API_KEY]);
    if (!contains(ACCOUNT_PREMIUM))
        setValue(ACCOUNT_PREMIUM, default_values[ACCOUNT_PREMIUM]);
    if (!contains(ACCOUNT_PREMIUM_EXPIRY))
        setValue(ACCOUNT_PREMIUM_EXPIRY, default_values[ACCOUNT_PREMIUM_EXPIRY]);
    if (!contains(ACCOUNT_DISK_USAGE))
        setValue(ACCOUNT_DISK_USAGE, default_values[ACCOUNT_DISK_USAGE]);

    // Key bindings

    if (!contains(BINDING_UPLOAD_FILE))
        setValue(BINDING_UPLOAD_FILE, default_values[BINDING_UPLOAD_FILE]);
    if (!contains(BINDING_CAPTURE_DESKTOP))
        setValue(BINDING_CAPTURE_DESKTOP, default_values[BINDING_CAPTURE_DESKTOP]);
    if (!contains(BINDING_CAPTURE_AREA))
        setValue(BINDING_CAPTURE_AREA, default_values[BINDING_CAPTURE_AREA]);
    if (!contains(BINDING_CAPTURE_WINDOW))
        setValue(BINDING_CAPTURE_WINDOW, default_values[BINDING_CAPTURE_WINDOW]);
    if (!contains(BINDING_UPLOAD_CLIPBOARD))
        setValue(BINDING_UPLOAD_CLIPBOARD, default_values[BINDING_UPLOAD_CLIPBOARD]);
    if (!contains(BINDING_TOGGLE_PUUSH))
        setValue(BINDING_TOGGLE_PUUSH, default_values[BINDING_TOGGLE_PUUSH]);
}
