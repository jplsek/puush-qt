#include "settings.h"

#include <QDesktopServices>
#include <QSettings>
#include <QString>
#include <QVariant>

#include <iostream>

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

    [Settings::Setting::COMPRESSION_PHILOSOPHY] = "compression-philisophy",

    [Settings::Setting::ENABLE_EXPLORER_CONTEXT_MENU] = "enable-explorer-context-menu",

    [Settings::Setting::FULLSCREEN_CAPTURE_MODE] = "fullscreen-capture-mode",

    [Settings::Setting::ENABLE_EXPERIMENTAL_FEATURES] = "enable-experiemntal-features",

    [Settings::Setting::SHOW_SELECTION_RECTANGLE] = "show-selection-rectangle",

    [Settings::Setting::IMAGE_QUALITY] = "quality"
};

const QString Settings::radio_values[] = {
    [Settings::RadioValue::OPEN_SETTINGS] = "open-settings",
    [Settings::RadioValue::OPEN_CAPTURE]  = "open-capture",
    [Settings::RadioValue::OPEN_UPLOADS]  = "open-uploads",

    [Settings::RadioValue::PNG_ALWAYS]         = "png-always",
    [Settings::RadioValue::JPG_ALWAYS]         = "jpg-always",
    [Settings::RadioValue::IMAGE_TYPE_SMALLER] = "smart",

    [Settings::RadioValue::ALL_DESKTOPS]    = "all-desktops",
    [Settings::RadioValue::CURRENT_DESKTOP] = "current-desktop",
    [Settings::RadioValue::PRIMARY_DESKTOP] = "primary-desktop",
};

const QString Settings::CURRENT_SETTINGS_VERSION = "2016-11-11-01"; // YYYY-MM-DD-revision

Settings::Settings(){}


QVariant Settings::value(Setting s){
    return this->s.value(setting_names[s]);
}

bool Settings::contains(Setting s){
    return this->s.contains(setting_names[s]);
}

void Settings::setValue(Setting s, QVariant val){
    this->s.setValue(setting_names[s], val);
}

bool Settings::radio_value_is(Setting s, RadioValue v){
    std::cout << "s == " << s << "; v == " << v << std::endl;
    return this->s.value(setting_names[s]).toString() == radio_values[v];
}

void Settings::resetSettings(){
}

void Settings::setEmptyToDefaults(){
   if(!s.contains(setting_names[IMAGE_QUALITY]))
       s.setValue(setting_names[IMAGE_QUALITY], 90);

   if(!s.contains(setting_names[ACCOUNT_EMAIL]))
       s.setValue(setting_names[ACCOUNT_EMAIL], "");

   if(!s.contains(setting_names[SETTINGS_VERSION]))
       s.setValue(setting_names[SETTINGS_VERSION], CURRENT_SETTINGS_VERSION);

   if(!s.contains(setting_names[ACCOUNT_API_KEY]))
       s.setValue(setting_names[ACCOUNT_API_KEY], "");
   if(!s.contains(setting_names[ACCOUNT_EMAIL]))
       s.setValue(setting_names[ACCOUNT_EMAIL], "");

   if(!s.contains(setting_names[ON_PUUSH_SOUND]))
       s.setValue(setting_names[ON_PUUSH_SOUND], false);
   if(!s.contains(setting_names[ON_PUUSH_COPY_LINK_TO_CLIPBOARD]))
       s.setValue(setting_names[ON_PUUSH_COPY_LINK_TO_CLIPBOARD], true);
   if(!s.contains(setting_names[ON_PUUSH_OPEN_LINK_IN_BROWSER]))
       s.setValue(setting_names[ON_PUUSH_OPEN_LINK_IN_BROWSER], false);

   if(!s.contains(setting_names[LOCAL_SAVE_ENABLED]))
       s.setValue(setting_names[LOCAL_SAVE_ENABLED], true);
   if(!s.contains(setting_names[LOCAL_SAVE_PATH]))
       s.setValue(setting_names[LOCAL_SAVE_PATH], QStandardPaths::writableLocation(QStandardPaths::PicturesLocation));
   if(!s.contains(setting_names[LOCAL_SAVE_NAME]))
       s.setValue(setting_names[LOCAL_SAVE_NAME], "ss-yyyy-MM-dd_hh-mm-ss");

   if(!s.contains(setting_names[TRAY_CLICK_ACTION]))
       s.setValue(setting_names[TRAY_CLICK_ACTION], OPEN_SETTINGS);

   if(!s.contains(setting_names[COMPRESSION_PHILOSOPHY]))
       s.setValue(setting_names[COMPRESSION_PHILOSOPHY], PNG_ALWAYS);

   if(!s.contains(setting_names[ENABLE_EXPLORER_CONTEXT_MENU]))
       s.setValue(setting_names[ENABLE_EXPLORER_CONTEXT_MENU], false);

   if(!s.contains(setting_names[FULLSCREEN_CAPTURE_MODE]))
       s.setValue(setting_names[FULLSCREEN_CAPTURE_MODE], ALL_DESKTOPS);

   if(!s.contains(setting_names[ENABLE_EXPERIMENTAL_FEATURES]))
       s.setValue(setting_names[ENABLE_EXPERIMENTAL_FEATURES], false);

   if(!s.contains(setting_names[SHOW_SELECTION_RECTANGLE]))
       s.setValue(setting_names[SHOW_SELECTION_RECTANGLE], true);

   if(!s.contains(setting_names[IMAGE_QUALITY]))
       s.setValue(setting_names[IMAGE_QUALITY], 90);
}