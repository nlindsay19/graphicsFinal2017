 _________________________
< Final project! 12/16/17 >
 -------------------------
        \   ^__^
         \  (oo)\_______
            (__)\       )\/\
                ||----w |
                ||     ||

Welcome to the space jam (space jam)!

To use our program:
We have a controls bar to the left of the main window. Use that to toggle the features that we implemented.

Stationary mode shows the planets in a fixed position. You'll notice they still rotate on their own axes though!
Orbit mode shows the planets orbiting the sun.
Both stationary and orbit modes are interactive. Feel free to click and drag to change the viewing angle and scroll to change the zoom along the Z axis.

Bezier mode is non-interactive. Take a guided journey through our planetary system! Additional lights have been added in this view, specifically to light Neptune, as otherwise you'll be seeing a black circle (because it's the back of Neptune and therefore not lit by the sun). The bezier curve is piecewise, with each planet and the sun forming a beginning and end point on curves that are joined together to form a continuous path. Note that we added some pauses at each planet and the sun for your viewing convenience (without these pauses, you'd get a more smooth feeling curve but the planets would zip by too quickly to see).

Features:
-Texture mapping on the skybox, planets, and sun
-Bump mapping on Mercury, Venus, Earth, and Mars (use the check box in the UI to turn on and off)
-HDR/Bloom on the sun (you'll also see the effects on Saturn and Uranus when in Bezier mode)
-Piecewise Bezier curve for a guided tour of the solar system!

Design decisions:
View is our main class where everything is drawn.
The Sun class handles the sun's lighting (the sun is lit with 6 extra point lights) and the sun's texture mapping. We also made the Sun a different class because of HDR and bloom (we didn't want to bloom all planets, just the sun).
The Planet class handles bump mapping, texture mapping, and lighting for each of the planets.
The Skybox class handles the skybox's texture mapping.
The BezierCurveUtil class handles the logic for generating points along a cubic bezier curve given a start, end, and two control points.
The OrbitingCamera class is our camera representation. We modified the TA-given Orbiting camera class for our purposes, notably to be able to modify the location of the camera.

To use our UI best:
We'd recommend checking out the bezier curve mode first, before trying out orbiting or stationary. This is because if you click and drag in orbiting and stationary, it messes with the camera's position, and we didn't end up resetting the camera to the right spot for the bezier mode. This is more of an issue with the UI and toggling on and off the different modes rather than our implementation.

Overall: have fun and thanks for a great semester!