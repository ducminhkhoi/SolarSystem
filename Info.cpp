//
// Created by Khoi Duc Minh Nguyen on 11/30/16.
//

#include <string>
using namespace std;
const int instructions_length = 37;
string instructions[instructions_length] = {
                    "------INSTRUCTIONS------",
                    "Left Click for adjust View",
                    "Right Click for more options",
                    "Middle Click for zoom in/out",
                    "KEY_LEFT for decrease self rotate speed",
                    "KEY_RIGHT for increase self rotate speed",
                    "KEY_UP for decrease orbital speed",
                    "KEY_DOWN for increase orbital speed",
                    "Press 'h' for hide this instructions",
                    "Press 'Q' for quit",
                    "Press 'O' for toggle Scale Orbit or not",
                    "Press 'D' for toggle Scale Diameter or not",
                    "Press 'B' for toggle Show Planets' orbits",
                    "Press 'N' for toggle Show Planets' names",
                    "Press 'F' for freeze animation",
                    "Press 'R' for reset View",
                    "Press 'S' for toggle Playing sound",
                    "Press '1' for Info about Sun",
                    "Press '2' for Info about Mercury",
                    "Press '3' for Info about Venus",
                    "Press '4' for Info about Earth",
                    "Press '5' for Info about Mars",
                    "Press '6' for Info about Jupiter",
                    "Press '7' for Info about Saturn",
                    "Press '8' for Info about Uranus",
                    "Press '9' for Info about Neptune",
                    "Press 'P' for Info about Pluto",
                    "Press 'X' for Info about Moon",
                    "Press 'C' for Info about Comet",
                    "Press 'U' for view from Sun",
                    "Press 'M' for view from Moon",
                    "Press 'E' for view from Earth",
                    "Press 'A' for view from Saturn",
                    "Press 'T' for view from Comet",
                    "Press 'I' for view from Closer View",
                    "Press 'V' for view from Overall View",
                    "Press 'Enter' for begin Voyage around Solar System",
};

const int info_length = 1;
string info_string[info_length] = {
        "Credits: Textures and sound from http://www.solarsystemscope.com"
};

const int info_2_length = 2;
string info_2_string[info_2_length] = {
        "Click Screen and Press 'H' for instructions",
        "Credits: Textures and sound from http://www.solarsystemscope.com"
};

const int view_from_19_length = 3;
string view_from_19[view_from_19_length] = {
        "The Solar System has 1 Sun and 8 official planets",
        "Mercury, Venus, Earth, Mars,",
        "Jupiter, Saturn, Uranus, Neptune"
};

const int view_from_18_length = 4;
string view_from_18[view_from_18_length] = {
        "The inner planets Mercury, Venus, Earth, Mars",
        "They are all terrestrial planets (Solid surface)",
        "The outer planets: Jupiter, Saturn, Uranus, Neptune",
        "They are all giant gas planets (Gas Surface)",
};

const int view_from_17_length = 1;
string view_from_17[view_from_17_length] = {
        "Here is the view from Comet 67P around the Sun",
};

const int view_from_16_length = 2;
string view_from_16[view_from_16_length] = {
        "Here is the View from Saturn to the Sun",
        "You can see it rings and its self rotation",
};

const int view_from_15_length = 2;
string view_from_15[view_from_15_length] = {
        "Here is the View from Earth",
        "Through 1 Earth day and night",
};

const int view_from_14_length = 3;
string view_from_14[view_from_14_length] = {
        "Here is the View from Moon to Earth",
        "You can see the light from Sun",
        "To create Earth Day and Night",
};

const int view_from_13_length = 3;
string view_from_13[view_from_13_length] = {
        "Here is the View from Sun",
        "You can see all planets orbit around it",
        "With different speeds depends their distances",
};

const int sun_info_length = 14;
string sun_info[sun_info_length] = {
                    "------SUN'S INFORMATION------",
                    "Equatorial Diameter:    109 x Earth",
                    "Surface Gravity:        28 x Earth",
                    "Surface Temperature:    5505 C",
                    "Rotation Period:        25 days",
                    "Galaxy Orbit Period:    23 M years",
                    "Galaxy Center Distance: 30 900 000 M Km",
                    "The star at the center of Solar System.",
                    "Sun's mass = 99.86% of total mass",
                    "Roughly Hydrogen: 3/4, Helium: 1/4",
                    "From Sun to Earth: 149.6 M km (1 AU)",
                    "Light from Sun reach Earth: 8 m 19 s",
                    "Brighter than 85% of stars in Milky Way",
                    "Located about 2/3 from center of MW",
};


const int mercury_info_length = 12;
string mercury_info[mercury_info_length] = {
        "------MERCURY'S INFORMATION------",
        "Equatorial Diameter:    0.38 x Earth",
        "Surface Gravity:        0.38 x Earth",
        "Temperature - Day:      473 C",
        "Temperature - Night:    -183 C",
        "Rotation Period:        59 days",
        "Solar Orbit Period:     88 days",
        "Mean Distance From Sun: 0.39 x Earth",
        "Innermost and smallest planet in SS",
        "No atmosphere to retain heat",
        "Greatest temp variation of all planets",
        "Seen: spring at dusk and autumn before dawn",
};

const int venus_info_length = 11;
string venus_info[venus_info_length] = {
        "------VENUS'S INFORMATION------",
        "Equatorial Diameter:    0.95 x Earth",
        "Surface Gravity:        0.9 x Earth",
        "Surface Temperature:    462 C",
        "Rotation Period:        243 days",
        "Solar Orbit Period:     225 days",
        "Mean Distance From Sun: 0.72 x Earth",
        "Extremely hot and dense atmosphere",
        "Comprise H2SO4, dust, CO2",
        "Greenhouse Effect, pressure = 92 x Earth",
        "Seen: One of brightest object at night",
};

const int earth_info_length = 12;
string earth_info[earth_info_length] = {
        "------EARTH'S INFORMATION------",
        "Equatorial Diameter:    0.009 x Sun",
        "Surface Gravity:        0.04 x Sun",
        "Surface Temperature:    15 C",
        "Rotation Period:        1 day",
        "Solar Orbit Period:     365 days",
        "Mean Distance From Sun: 150 M Km",
        "Densest planet in Solar System (SS)",
        "Largest of four terrestrial planets",
        "Surface: 70% is Ocean, 30% is Continent",
        "Tilt at 23.4 degree => seasons",
};

const int mars_info_length = 13;
string mars_info[mars_info_length] = {
        "------MARS'S INFORMATION------",
        "Equatorial Diameter:    0.53 x Earth",
        "Surface Gravity:        0.38 x Earth",
        "Summer Temperature:     17 C",
        "Winter Temperature:     -140 C",
        "Rotation Period:        1.03 day",
        "Solar Orbit Period:     687 days",
        "Mean Distance From Sun: 1.52 x Earth",
        "4th planet from Sun, 2nd smallest in SS",
        "Reddish surface caused by ion oxide",
        "Has rocky surface and polar ice caps",
        "Once has water ran freely on surface",
        "Moons: Phobos, Deimos very closed",
};

const int jupiter_info_length = 13;
string jupiter_info[jupiter_info_length] = {
        "------JUPITER'S INFORMATION------",
        "Equatorial Diameter:    11.2 x Earth",
        "Surface Gravity:        2.5 x Earth",
        "Surface Temperature:    -120 C",
        "Rotation Period:        9h 55m",
        "Solar Orbit Period:     11.9 years",
        "Mean Distance From Sun: 5.2 x Earth",
        "Largest planet in Solar System",
        "Need more 75 x to become a star",
        "Fastest self rotation => high winds",
        "Largest Moons: Io, Europa, Callisto, Ganymede",
        "4th brightest object in our skies",
};

const int saturn_info_length = 13;
string saturn_info[saturn_info_length] = {
        "------SATURN'S INFORMATION------",
        "Equatorial Diameter:    9.5 x Earth",
        "Surface Gravity:        1.06 x Earth",
        "Surface Temperature:    -125 C",
        "Rotation Period:        10h 39m",
        "Solar Orbit Period:     29 years",
        "Mean Distance From Sun: 9.5 x Earth",
        "2nd Largest planet, 6th from Sun",
        "Least dense of all planets in SS",
        "Largest and most invisible ring",
        "Largest moons: Titan and 150 others ",
};

const int uranus_info_length = 11;
string uranus_info[uranus_info_length] = {
        "------URANUS'S INFORMATION------",
        "Equatorial Diameter:    4.1 x Earth",
        "Surface Gravity:        0.89 x Earth",
        "Surface Temperature:    -210 C",
        "Rotation Period:        17h 14m",
        "Solar Orbit Period:     84 years",
        "Mean Distance From Sun: 19.2 x Earth",
        "3rd Largest Planet in Solar System",
        "Coldest planet in Solar System",
        "Discovered by William Herschel in 1781",
        "Tilt at 97.77, rolls around the sun",
};

const int neptune_info_length = 11;
string neptune_info[neptune_info_length] = {
        "------NEPTUNE'S INFORMATION------",
        "Equatorial Diameter:    3.9 x Earth",
        "Surface Gravity:        1.14 x Earth",
        "Surface Temperature:    -200 C",
        "Rotation Period:        16h 6m",
        "Solar Orbit Period:     165 years",
        "Mean Distance From Sun: 30 x Earth",
        "Farthest Planet From Sun",
        "Smallest but most dense of gas giants",
        "Discovered by Math. prediction in 1846",
        "Just complete 1 orbit around the Sun",
};

const int pluto_info_length = 11;
string pluto_info[pluto_info_length] = {
        "------PLUTO'S INFORMATION------",
        "Equatorial Diameter:    0.18 x Earth",
        "Surface Gravity:        0.07 x Earth",
        "Surface Temperature:    -228 C",
        "Rotation Period:        6.4 days",
        "Solar Orbit Period:     248 years",
        "Mean Distance From Sun: 39 x Earth",
        "Second largest known dwarf planet in SS",
        "Tilt at 120, extreme seasonal variation",
        "Discovered by Clyde Tombaugh in 1930",
        "Largest moon: Charon => binary system",
};

const int moon_info_length = 10;
string moon_info[moon_info_length] = {
        "------MOON'S INFORMATION------",
        "Equatorial Diameter:    0.27 x Earth",
        "Surface Gravity:        0.17 x Earth",
        "Surface Temperature:    -23 C",
        "Rotation Period:        27 days",
        "Earth Orbit Period:     27 days",
        "Mean Dist. From Earth:  384 000 Km",
        "Greatest influence on Earth",
        "Larger than Pluto",
        "Same size as Sun from Earth",
};

const int comet_info_length = 9;
string comet_info[comet_info_length] = {
        "------COMET 67P'S INFORMATION------",
        "Fullname: Churyumov–Gerasimenko",
        "Maximum velocity:       135 000 km/h",
        "Dimension:              4.3 x 4.1 Km",
        "Rotation Period:        12.4 hours",
        "Solar Orbit Period:     6.45 years",
        "Mean Dist. From Sun:  3.46 x Earth",
        "Discovered in September 20, 1969",
        "European Space Agency's Rosetta mission",
};