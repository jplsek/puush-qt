#include "settings.h"

#include <QDesktopServices>
#include <QSettings>
#include <QString>
#include <QVariant>

const QString Settings::GENERAL_GROUP_NAME   = "basic"; // [general] -> [basic] because it was conflicting with QSettings [general]
const QString Settings::ADVANCED_GROUP_NAME  = "advanced";
const QString Settings::EXTRA_GROUP_NAME     = "extra";
const QString Settings::USER_INFO_GROUP_NAME = "user-info";
const QString Settings::BINDINGS_GROUP_NAME  = "key-bindings";

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

    [Settings::Settings::BINDING_UPLOAD_FILE] = "upload-file-key-binding",
    [Settings::Settings::BINDING_CAPTURE_DESKTOP] = "capture-desktop-key-binding",
    [Settings::Settings::BINDING_CAPTURE_AREA] = "capture-area-key-binding",
    [Settings::Settings::BINDING_CAPTURE_WINDOW] = "capture-window-key-binding",
    [Settings::Settings::BINDING_UPLOAD_CLIPBOARD] = "upload-clipboard-key-binding",
    [Settings::Settings::BINDING_TOGGLE_PUUSH] = "toggle-puush-key-binding",
};

const QString Settings::default_values[] = {
    [Settings::Setting::NO_SETTING] = "",

    [Settings::Setting::SETTINGS_VERSION] = "2017-20-4-00", // YYYY-MM-DD-revision

    [Settings::Setting::ACCOUNT_API_KEY] = "",
    [Settings::Setting::ACCOUNT_EMAIL] = "",

    [Settings::Setting::ON_PUUSH_SOUND] = "false",
    [Settings::Setting::ON_PUUSH_COPY_LINK_TO_CLIPBOARD] = "true",
    [Settings::Setting::ON_PUUSH_OPEN_LINK_IN_BROWSER] = "false",

    [Settings::Setting::LOCAL_SAVE_ENABLED] = "true",
    [Settings::Setting::LOCAL_SAVE_PATH]    = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
    [Settings::Setting::LOCAL_SAVE_NAME]    = "'ss' (yyyy-MM-dd at hh.mm.ss",

    [Settings::Setting::TRAY_CLICK_ACTION] = "",

    [Settings::Setting::COMPRESSION_PHILOSOPHY] = "",

    [Settings::Setting::FULLSCREEN_CAPTURE_MODE] = "",

    [Settings::Setting::IMAGE_QUALITY] = "90",

    [Settings::Setting::BASE_URL] = "https://puush.me/",
    [Settings::Setting::API_URL]  = "https://puush.me/api/",

    [Settings::Settings::BINDING_UPLOAD_FILE] = "Ctrl+Shift+F1",
    [Settings::Settings::BINDING_CAPTURE_DESKTOP] = "Ctrl+Shift+F3",
    [Settings::Settings::BINDING_CAPTURE_AREA] = "Ctrl+Shift+F4",
    [Settings::Settings::BINDING_CAPTURE_WINDOW] = "Ctrl+Shift+F2",
    [Settings::Settings::BINDING_UPLOAD_CLIPBOARD] = "Ctrl+Shift+F5",
    [Settings::Settings::BINDING_TOGGLE_PUUSH] = "Ctrl+Shift+F6",
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

    [Settings::Settings::BINDING_UPLOAD_FILE] = Settings::BINDINGS_GROUP_NAME,
    [Settings::Settings::BINDING_CAPTURE_DESKTOP] = Settings::BINDINGS_GROUP_NAME,
    [Settings::Settings::BINDING_CAPTURE_AREA] = Settings::BINDINGS_GROUP_NAME,
    [Settings::Settings::BINDING_CAPTURE_WINDOW] = Settings::BINDINGS_GROUP_NAME,
    [Settings::Settings::BINDING_UPLOAD_CLIPBOARD] = Settings::BINDINGS_GROUP_NAME,
    [Settings::Settings::BINDING_TOGGLE_PUUSH] = Settings::BINDINGS_GROUP_NAME,
};

const QString Settings::radio_values[] = {
    [Settings::RadioValue::OPEN_SETTINGS]    = "open-settings",
    [Settings::RadioValue::OPEN_UPLOADS]     = "open-uploads",
    [Settings::RadioValue::UPLOAD_FILE]      = "upload-file",
    [Settings::RadioValue::UPLOAD_CLIPBOARD] = "upload-clipboard",
    [Settings::RadioValue::CAPTURE_DESKTOP]  = "capture-desktop",
    [Settings::RadioValue::CAPTURE_AREA]     = "capture-screen",
    [Settings::RadioValue::CAPTURE_WINDOW]   = "capture-window",
    [Settings::RadioValue::OPEN_ACCOUNT]     = "open-account",

    [Settings::RadioValue::PNG_ALWAYS]         = "png-always",
    [Settings::RadioValue::JPG_ALWAYS]         = "jpg-always",
    [Settings::RadioValue::IMAGE_TYPE_SMALLER] = "smart",

    [Settings::RadioValue::ALL_DESKTOPS]    = "all-desktops",
    [Settings::RadioValue::CURRENT_DESKTOP] = "current-desktop",
    [Settings::RadioValue::PRIMARY_DESKTOP] = "primary-desktop",
};

Settings::Settings(){
    if(s.value(setting_names[SETTINGS_VERSION]).toString() != default_values[SETTINGS_VERSION]){
        // FIXME: show 'popup' saying some settings may have been reset due to settings key/value changes
        // <ok ill check> <bother me later>
        // if they say ok, then update the settings version. otherwise dont so this will be run again.
        // For now, just update it as jeremy and I know whats up.
        s.beginGroup(EXTRA_GROUP_NAME);
        s.setValue(setting_names[SETTINGS_VERSION], default_values[SETTINGS_VERSION]);
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
    setValue(ON_PUUSH_SOUND, default_values[ON_PUUSH_SOUND]);
    setValue(ON_PUUSH_COPY_LINK_TO_CLIPBOARD, default_values[ON_PUUSH_COPY_LINK_TO_CLIPBOARD]);
    setValue(ON_PUUSH_OPEN_LINK_IN_BROWSER, default_values[ON_PUUSH_OPEN_LINK_IN_BROWSER]);

    setValue(LOCAL_SAVE_ENABLED, default_values[LOCAL_SAVE_ENABLED]);
    setValue(LOCAL_SAVE_PATH, default_values[LOCAL_SAVE_PATH]);
    setValue(LOCAL_SAVE_NAME, default_values[LOCAL_SAVE_NAME]);

    setRadioValue(TRAY_CLICK_ACTION, OPEN_SETTINGS);
}

void Settings::resetAdvancedSettings(){
    setRadioValue(COMPRESSION_PHILOSOPHY, PNG_ALWAYS);

    setRadioValue(FULLSCREEN_CAPTURE_MODE, ALL_DESKTOPS);

    setValue(IMAGE_QUALITY, default_values[IMAGE_QUALITY]);
}

void Settings::resetExtraSettings(){
    s.endGroup();
    s.beginGroup(EXTRA_GROUP_NAME);
    s.setValue(setting_names[BASE_URL], default_values[BASE_URL]);
    s.setValue(setting_names[API_URL],  default_values[API_URL]);
    s.setValue(setting_names[SETTINGS_VERSION], default_values[SETTINGS_VERSION]);
}

void Settings::resetUserInfoSettings(){
    s.endGroup();
    s.beginGroup(USER_INFO_GROUP_NAME);
    s.setValue(setting_names[ACCOUNT_EMAIL],   default_values[ACCOUNT_EMAIL]);
    s.setValue(setting_names[ACCOUNT_API_KEY], default_values[ACCOUNT_API_KEY]);
}

void Settings::resetBindingsSettings(){
    s.endGroup();
    s.beginGroup(BINDINGS_GROUP_NAME);
    s.setValue(setting_names[BINDING_UPLOAD_FILE],      default_values[BINDING_UPLOAD_FILE]);
    s.setValue(setting_names[BINDING_CAPTURE_DESKTOP],  default_values[BINDING_CAPTURE_DESKTOP]);
    s.setValue(setting_names[BINDING_CAPTURE_AREA],     default_values[BINDING_CAPTURE_AREA]);
    s.setValue(setting_names[BINDING_CAPTURE_WINDOW],   default_values[BINDING_CAPTURE_WINDOW]);
    s.setValue(setting_names[BINDING_UPLOAD_CLIPBOARD], default_values[BINDING_UPLOAD_CLIPBOARD]);
    s.setValue(setting_names[BINDING_TOGGLE_PUUSH],     default_values[BINDING_TOGGLE_PUUSH]);
}

QVariant Settings::resetValue(Setting s){
    this->s.endGroup();
    this->s.beginGroup(groups[s]);
    this->s.setValue(setting_names[s], default_values[s]);
    return value(s);
}

void Settings::setEmptyToDefaults(){
    // General

    if(!contains(ON_PUUSH_SOUND))
        setValue(ON_PUUSH_SOUND, default_values[ON_PUUSH_SOUND]);
    if(!contains(ON_PUUSH_COPY_LINK_TO_CLIPBOARD))
        setValue(ON_PUUSH_COPY_LINK_TO_CLIPBOARD, default_values[ON_PUUSH_COPY_LINK_TO_CLIPBOARD]);
    if(!contains(ON_PUUSH_OPEN_LINK_IN_BROWSER))
        setValue(ON_PUUSH_OPEN_LINK_IN_BROWSER, default_values[ON_PUUSH_OPEN_LINK_IN_BROWSER]);

    if(!contains(LOCAL_SAVE_ENABLED))
        setValue(LOCAL_SAVE_ENABLED, default_values[LOCAL_SAVE_ENABLED]);
    if(!contains(LOCAL_SAVE_PATH) || s.value(setting_names[LOCAL_SAVE_PATH]).toString() == "")
        setValue(LOCAL_SAVE_PATH, default_values[LOCAL_SAVE_PATH]);
    if(!contains(LOCAL_SAVE_NAME))
        setValue(LOCAL_SAVE_NAME, default_values[LOCAL_SAVE_NAME]);

    if(!contains(TRAY_CLICK_ACTION))
        setRadioValue(TRAY_CLICK_ACTION, OPEN_SETTINGS);

    // Advanced

    if(!contains(COMPRESSION_PHILOSOPHY))
        setRadioValue(COMPRESSION_PHILOSOPHY, PNG_ALWAYS);

    if(!contains(FULLSCREEN_CAPTURE_MODE))
        setRadioValue(FULLSCREEN_CAPTURE_MODE, ALL_DESKTOPS);

    if(!contains(IMAGE_QUALITY))
        setValue(IMAGE_QUALITY, default_values[IMAGE_QUALITY]);

    // Extra

    if(!contains(SETTINGS_VERSION))
        setValue(SETTINGS_VERSION, default_values[SETTINGS_VERSION]);

    if(!contains(BASE_URL))
        setValue(BASE_URL, default_values[BASE_URL]);
    if(!contains(API_URL))
        setValue(API_URL,  default_values[API_URL]);

    // User info

    if(!contains(ACCOUNT_EMAIL))
        setValue(ACCOUNT_EMAIL, default_values[ACCOUNT_EMAIL]);
    if(!contains(ACCOUNT_API_KEY))
        setValue(ACCOUNT_API_KEY, default_values[ACCOUNT_API_KEY]);

    // Key bindings

    if(!contains(BINDING_UPLOAD_FILE))
        setValue(BINDING_UPLOAD_FILE, default_values[BINDING_UPLOAD_FILE]);
    if(!contains(BINDING_CAPTURE_DESKTOP))
        setValue(BINDING_CAPTURE_DESKTOP, default_values[BINDING_CAPTURE_DESKTOP]);
    if(!contains(BINDING_CAPTURE_AREA))
        setValue(BINDING_CAPTURE_AREA, default_values[BINDING_CAPTURE_AREA]);
    if(!contains(BINDING_CAPTURE_WINDOW))
        setValue(BINDING_CAPTURE_WINDOW, default_values[BINDING_CAPTURE_WINDOW]);
    if(!contains(BINDING_UPLOAD_CLIPBOARD))
        setValue(BINDING_UPLOAD_CLIPBOARD, default_values[BINDING_UPLOAD_CLIPBOARD]);
    if(!contains(BINDING_TOGGLE_PUUSH))
        setValue(BINDING_TOGGLE_PUUSH, default_values[BINDING_TOGGLE_PUUSH]);
}
