#include "settings.h"

#include <QDesktopServices>
#include <QSettings>
#include <QString>
#include <QVariant>

const QString OFFICIAL_PUUSH_BASE_URL = "https://puush.me/";
const QString OFFICIAL_PUUSH_API_URL  = OFFICIAL_PUUSH_BASE_URL + "api/";

const QString Settings::GENERAL_GROUP_NAME   = "basic"; // [general] -> [basic] because it was conflicting with QSettings [general]
const QString Settings::ADVANCED_GROUP_NAME  = "advanced";
const QString Settings::EXTRA_GROUP_NAME     = "extra";
const QString Settings::USER_INFO_GROUP_NAME = "user-info";

const QString Settings::CURRENT_SETTINGS_VERSION = "2016-12-1-01"; // YYYY-MM-DD-revision


// generic format of this is <option what/when> <value>
// These values should not change; as it will break peoples saved settings.
const QString Settings::setting_names[] = {
    [Settings::Setting::NO_SETTING] = "",

    [Settings::Setting::SETTINGS_VERSION] = "settings-version",

    [Settings::Setting::ACCOUNT_API_KEY] = "key",
    [Settings::Setting::ACCOUNT_EMAIL] = "email",

    [Settings::Setting::ON_PUUSH_SOUND] = "sound-on-puush",
    [Settings::Setting::ON_PUUSH_COPY_LINK_TO_CLIPBOARD] = "copy-link-to-clipboard",
    [Settings::Setting::ON_PUUSH_OPEN_LINK_IN_BROWSER] = "open-link-in-browser",

    [Settings::Setting::LOCAL_SAVE_ENABLED] = "local-save-enabled",
    [Settings::Setting::LOCAL_SAVE_PATH]    = "local-save-path",
    [Settings::Setting::LOCAL_SAVE_NAME]    = "local-save-name",

    [Settings::Setting::TRAY_CLICK_ACTION] = "tray-click-action",

    [Settings::Setting::COMPRESSION_PHILOSOPHY] = "compression-philosophy",

    [Settings::Setting::FULLSCREEN_CAPTURE_MODE] = "fullscreen-capture-mode",

    [Settings::Setting::IMAGE_QUALITY] = "quality",

    [Settings::Setting::BASE_URL] = "base-url",
    [Settings::Setting::API_URL]  = "api-url",
};

const QString Settings::groups[] = {
    [Settings::Setting::NO_SETTING] = Settings::EXTRA_GROUP_NAME,

    [Settings::Setting::SETTINGS_VERSION] = Settings::EXTRA_GROUP_NAME,

    [Settings::Setting::ACCOUNT_API_KEY] = Settings::USER_INFO_GROUP_NAME,
    [Settings::Setting::ACCOUNT_EMAIL] = Settings::USER_INFO_GROUP_NAME,

    [Settings::Setting::ON_PUUSH_SOUND] = Settings::GENERAL_GROUP_NAME,
    [Settings::Setting::ON_PUUSH_COPY_LINK_TO_CLIPBOARD] = Settings::GENERAL_GROUP_NAME,
    [Settings::Setting::ON_PUUSH_OPEN_LINK_IN_BROWSER] = Settings::GENERAL_GROUP_NAME,

    [Settings::Setting::LOCAL_SAVE_ENABLED] = Settings::GENERAL_GROUP_NAME,
    [Settings::Setting::LOCAL_SAVE_PATH]    = Settings::GENERAL_GROUP_NAME,
    [Settings::Setting::LOCAL_SAVE_NAME]    = Settings::GENERAL_GROUP_NAME,

    [Settings::Setting::TRAY_CLICK_ACTION] = Settings::GENERAL_GROUP_NAME,

    [Settings::Setting::COMPRESSION_PHILOSOPHY] = Settings::ADVANCED_GROUP_NAME,

    [Settings::Setting::FULLSCREEN_CAPTURE_MODE] = Settings::ADVANCED_GROUP_NAME,

    [Settings::Setting::IMAGE_QUALITY] = Settings::ADVANCED_GROUP_NAME,

    [Settings::Setting::BASE_URL] = Settings::EXTRA_GROUP_NAME,
    [Settings::Setting::API_URL]  = Settings::EXTRA_GROUP_NAME,
};

const QString Settings::radio_values[] = {
    [Settings::RadioValue::OPEN_SETTINGS]   = "open-settings",
    [Settings::RadioValue::OPEN_UPLOADS]    = "open-uploads",
    [Settings::RadioValue::UPLOAD_FILE]     = "upload-file",
    [Settings::RadioValue::CAPTURE_DESKTOP] = "capture-desktop",
    [Settings::RadioValue::CAPTURE_AREA]    = "capture-screen",
    [Settings::RadioValue::CAPTURE_WINDOW]  = "capture-window",
    [Settings::RadioValue::OPEN_ACCOUNT]    = "open-account",

    [Settings::RadioValue::PNG_ALWAYS]         = "png-always",
    [Settings::RadioValue::JPG_ALWAYS]         = "jpg-always",
    [Settings::RadioValue::IMAGE_TYPE_SMALLER] = "smart",

    [Settings::RadioValue::ALL_DESKTOPS]    = "all-desktops",
    [Settings::RadioValue::CURRENT_DESKTOP] = "current-desktop",
    [Settings::RadioValue::PRIMARY_DESKTOP] = "primary-desktop",
};

Settings::Settings(){
    if(s.value(setting_names[SETTINGS_VERSION]).toString() != CURRENT_SETTINGS_VERSION){
        // FIXME: show 'popup' saying some settings may have been reset due to settings key/value changes
        // <ok ill check> <bother me later>
        // if they say ok, then update the settings version. otherwise dont so this will be run again.
        // For now, just update it as jeremy and I know whats up.
        s.beginGroup(EXTRA_GROUP_NAME);
        s.setValue(setting_names[SETTINGS_VERSION], CURRENT_SETTINGS_VERSION);
    }
}


QVariant Settings::value(Setting s){
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    return this->s.value(setting_names[s]);
}

bool Settings::contains(Setting s){
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    return this->s.contains(setting_names[s]);
}

void Settings::setValue(Setting s, QVariant val){
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    this->s.setValue(setting_names[s], val);
}

void Settings::setRadioValue(Setting s, RadioValue val){
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    this->s.setValue(setting_names[s], radio_values[val]);
}

bool Settings::radioValueIs(Setting s, RadioValue v){
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    return this->s.value(setting_names[s]).toString() == radio_values[v];
}

void Settings::resetGeneralSettings(){
    setValue(ON_PUUSH_SOUND, false);
    setValue(ON_PUUSH_COPY_LINK_TO_CLIPBOARD, true);
    setValue(ON_PUUSH_OPEN_LINK_IN_BROWSER, false);

    setValue(LOCAL_SAVE_ENABLED, true);
    setValue(LOCAL_SAVE_PATH, QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    setValue(LOCAL_SAVE_NAME, "'ss' (yyyy-MM-dd hh.mm.ss)");

    setRadioValue(TRAY_CLICK_ACTION, OPEN_SETTINGS);
}

void Settings::resetAdvancedSettings(){
    setRadioValue(COMPRESSION_PHILOSOPHY, PNG_ALWAYS);

    setRadioValue(FULLSCREEN_CAPTURE_MODE, ALL_DESKTOPS);

    setValue(IMAGE_QUALITY, 90);
}

void Settings::resetExtraSettings(){
    s.endGroup();
    s.beginGroup(EXTRA_GROUP_NAME);
    s.setValue(setting_names[BASE_URL], OFFICIAL_PUUSH_BASE_URL);
    s.setValue(setting_names[API_URL],  OFFICIAL_PUUSH_API_URL);
    s.setValue(setting_names[SETTINGS_VERSION], CURRENT_SETTINGS_VERSION);
}

void Settings::resetUserInfoSettings(){
    s.endGroup();
    s.beginGroup(USER_INFO_GROUP_NAME);
    s.setValue(setting_names[ACCOUNT_EMAIL], "");
    s.setValue(setting_names[ACCOUNT_API_KEY],  "");
}

void Settings::setEmptyToDefaults(){
    // General

    if(!contains(ON_PUUSH_SOUND))
        setValue(ON_PUUSH_SOUND, false);
    if(!contains(ON_PUUSH_COPY_LINK_TO_CLIPBOARD))
        setValue(ON_PUUSH_COPY_LINK_TO_CLIPBOARD, true);
    if(!contains(ON_PUUSH_OPEN_LINK_IN_BROWSER))
        setValue(ON_PUUSH_OPEN_LINK_IN_BROWSER, false);

    if(!contains(LOCAL_SAVE_ENABLED))
        setValue(LOCAL_SAVE_ENABLED, true);
    if(!contains(LOCAL_SAVE_PATH) || s.value(setting_names[LOCAL_SAVE_PATH]).toString() == "")
        setValue(LOCAL_SAVE_PATH, QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
    if(!contains(LOCAL_SAVE_NAME))
        setValue(LOCAL_SAVE_NAME, "'ss' (yyyy-MM-dd at hh.mm.ss");

    if(!contains(TRAY_CLICK_ACTION))
        setRadioValue(TRAY_CLICK_ACTION, OPEN_SETTINGS);

    // Advanced

    if(!contains(COMPRESSION_PHILOSOPHY))
        setRadioValue(COMPRESSION_PHILOSOPHY, PNG_ALWAYS);

    if(!contains(FULLSCREEN_CAPTURE_MODE))
        setRadioValue(FULLSCREEN_CAPTURE_MODE, ALL_DESKTOPS);

    if(!contains(IMAGE_QUALITY))
        setValue(IMAGE_QUALITY, 90);

    // Extra

    if(!contains(SETTINGS_VERSION))
        setValue(SETTINGS_VERSION, CURRENT_SETTINGS_VERSION);

    if(!contains(BASE_URL))
        setValue(BASE_URL, OFFICIAL_PUUSH_BASE_URL);
    if(!contains(API_URL))
        setValue(API_URL,  OFFICIAL_PUUSH_API_URL);

    // User info

    if(!contains(ACCOUNT_EMAIL))
        setValue(ACCOUNT_EMAIL, "");
    if(!contains(ACCOUNT_API_KEY))
        setValue(ACCOUNT_API_KEY, "");
}
