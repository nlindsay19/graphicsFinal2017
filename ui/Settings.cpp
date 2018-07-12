/*!

 Settings.h
 CS123 Support Code

 @author  Evan Wallace (edwallac)
 @date    9/1/2010

 This file contains various settings and enumerations that you will need to
 use in the various assignments. The settings are bound to the GUI via static
 data bindings.

**/

#include "Settings.h"
#include <QFile>
#include <QSettings>

Settings settings;

/**
 * Loads the application settings, or, if no saved settings are available, loads default values for
 * the settings. You can change the defaults here.
 */
void Settings::loadSettingsOrDefaults() {
    // Set the default values below
    QSettings s("CS123", "CS123");

    // Space Jam
    space_radio = s.value("space_radio", ORBITING).toInt();
    bumpMapping  = s.value("bumpMapping", false).toBool();
    lighting = s.value("lighting", true).toBool();
    hdr = s.value("hdr", true).toBool();
}

void Settings::saveSettings() {
    QSettings s("CS123", "CS123");

    // Space Jam
    s.setValue("space_radio", space_radio);
    s.setValue("bumpMapping", bumpMapping);
    s.setValue("lighting", lighting);
    s.setValue("hdr", hdr);
}
