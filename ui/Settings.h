/**
 * @file    Settings.h
 *
 * This file contains various settings and enumerations that you will need to use in the various
 * assignments. The settings are bound to the GUI via static data bindings.
 */

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include "lib/BGRA.h"

/**
 * @struct Settings
 *
 * Stores application settings for the CS123 GUI.
 *
 * You can access all app settings through the "settings" global variable.
 * The settings will be automatically updated when things are changed in the
 * GUI (the reverse is not true however: changing the value of a setting does
 * not update the GUI).
*/

enum SpaceJam {
    STATIONARY,
    ORBITING,
    BEZIER,
    NUM_SHAPES
};

struct Settings {
    // Loads settings from disk, or fills in default values if no saved settings exist.
    void loadSettingsOrDefaults();

    // Saves the current settings to disk.
    void saveSettings();

    // Space Jam
    bool bumpMapping;
    int space_radio; // the space jam radio button currently selected
    bool lighting;
    bool hdr;

    // Shapes
    bool useSceneviewScene;
    int shapeType;              // Selected shape type
    int shapeParameter1;

    // Camtrans
    bool useOrbitCamera;        // Use the built-in orbiting camera instead of the Camtrans camera
    float cameraPosX;
    float cameraPosY;
    float cameraPosZ;
    float cameraRotU;
    float cameraRotV;
    float cameraRotN;
    float cameraFov;            // The camera's field of view, which is twice the height angle.
    float cameraNear;           // The distance from the camera to the near clipping plane.
    float cameraFar;            // The distance from the camera to the far clipping plane.

    // Ray
    bool usePointLights;        // Enable or disable point lighting.
    bool useDirectionalLights;  // Enable or disable directional lighting (extra credit).
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
