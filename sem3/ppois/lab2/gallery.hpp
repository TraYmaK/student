/**
 * @file gallery.hpp
 * @author James Maxwell
 * @brief Header file for the Gallery management system
 * @version 0.1
 * @date 2025-01-15
 * 
 * This file contains all classes for managing a gallery system including
 * artworks, exhibitions, tools, climate control, security, and visitor management.
 */

#pragma once
#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;

/**
 * @class ArtworkNotFoundException
 * @brief Exception thrown when artwork is not found
 */
class ArtworkNotFoundException : public runtime_error {
public:
    /**
     * @brief Construct a new ArtworkNotFoundException object
     * 
     * @param msg constant pointer to the error message
     */
    ArtworkNotFoundException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class NotEnoughArtworkException
 * @brief Exception thrown when there is not enough artwork available
 */
class NotEnoughArtworkException : public runtime_error {
public:
    /**
     * @brief Construct a new NotEnoughArtworkException object
     * 
     * @param msg constant pointer to the error message
     */
    NotEnoughArtworkException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class ToolNotAvailableException
 * @brief Exception thrown when a tool is not available for use
 */
class ToolNotAvailableException : public runtime_error {
public:
    /**
     * @brief Construct a new ToolNotAvailableException object
     * 
     * @param msg constant pointer to the error message
     */
    ToolNotAvailableException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class InvalidConditionException
 * @brief Exception thrown when climate conditions are invalid
 */
class InvalidConditionException : public runtime_error {
public:
    /**
     * @brief Construct a new InvalidConditionException object
     * 
     * @param msg constant pointer to the error message
     */
    InvalidConditionException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class TimerNotSetException
 * @brief Exception thrown when timer is not properly set
 */
class TimerNotSetException : public runtime_error {
public:
    /**
     * @brief Construct a new TimerNotSetException object
     * 
     * @param msg constant pointer to the error message
     */
    TimerNotSetException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class OverexposedArtworkException
 * @brief Exception thrown when artwork is overexposed to conditions
 */
class OverexposedArtworkException : public runtime_error {
public:
    /**
     * @brief Construct a new OverexposedArtworkException object
     * 
     * @param msg constant pointer to the error message
     */
    OverexposedArtworkException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class UnderexposedArtworkException
 * @brief Exception thrown when artwork is underexposed to conditions
 */
class UnderexposedArtworkException : public runtime_error {
public:
    /**
     * @brief Construct a new UnderexposedArtworkException object
     * 
     * @param msg constant pointer to the error message
     */
    UnderexposedArtworkException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class StorageException
 * @brief Exception thrown when storage operation fails
 */
class StorageException : public runtime_error {
public:
    /**
     * @brief Construct a new StorageException object
     * 
     * @param msg constant pointer to the error message
     */
    StorageException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class SecurityBreachException
 * @brief Exception thrown when security breach is detected
 */
class SecurityBreachException : public runtime_error {
public:
    /**
     * @brief Construct a new SecurityBreachException object
     * 
     * @param msg constant pointer to the error message
     */
    SecurityBreachException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class RestorationFailedException
 * @brief Exception thrown when restoration process fails
 */
class RestorationFailedException : public runtime_error {
public:
    /**
     * @brief Construct a new RestorationFailedException object
     * 
     * @param msg constant pointer to the error message
     */
    RestorationFailedException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class InsufficientSpaceException
 * @brief Exception thrown when there is insufficient space
 */
class InsufficientSpaceException : public runtime_error {
public:
    /**
     * @brief Construct a new InsufficientSpaceException object
     * 
     * @param msg constant pointer to the error message
     */
    InsufficientSpaceException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class InvalidAccessException
 * @brief Exception thrown when access is invalid or denied
 */
class InvalidAccessException : public runtime_error {
public:
    /**
     * @brief Construct a new InvalidAccessException object
     * 
     * @param msg constant pointer to the error message
     */
    InvalidAccessException(const char* msg) : runtime_error(msg) {}
};

/**
 * @class Dimension
 * @brief Class for representing measurement dimensions
 * 
 * Handles different measurement units and conversions between them.
 * Supports both metric and non-metric units with precision control.
 */
class Dimension {
private:
    const char* name;        ///< Name of the dimension unit
    double cmPerUnit;        ///< Conversion factor to centimeters per unit
    bool metric;            ///< Whether the dimension is metric
    int id;                 ///< Unique identifier for the dimension
    double precision;       ///< Precision value for measurements
    bool standard;          ///< Whether this is a standard dimension

public:
    /**
     * @brief Construct a new Dimension object
     * 
     * @param n constant pointer to the dimension name
     * @param c double value containing centimeters per unit
     * @param m boolean value indicating if metric
     * @param i integer value containing dimension identifier
     */
    Dimension(const char* n = "unit", double c = 1.0, bool m = true, int i = 0);

    /**
     * @brief Convert amount to centimeters
     * 
     * @param amount double value containing amount to convert
     * @return double containing converted value in centimeters
     */
    double toCentimeters(double amount) const;

    /**
     * @brief Check if dimension is metric
     * 
     * @return true if dimension is metric
     * @return false if dimension is not metric
     */
    bool isMetric() const;

    /**
     * @brief Get the dimension identifier
     * 
     * @return int containing dimension identifier
     */
    int getId() const;

    /**
     * @brief Set the precision value
     * 
     * @param p double value containing precision to set
     */
    void setPrecision(double p);

    /**
     * @brief Check if dimension is standard
     * 
     * @return true if dimension is standard
     * @return false if dimension is not standard
     */
    bool isStandard() const;
};

/**
 * @class Size
 * @brief Class for representing artwork size with dimension
 * 
 * Represents size with a specific dimension unit and handles
 * conversions and scaling operations.
 */
class Size {
private:
    double value;           ///< Size value in the dimension unit
    Dimension* dimension;  ///< Pointer to the dimension object
    bool approximate;       ///< Whether the size is approximate
    int id;                 ///< Unique identifier for the size

public:
    /**
     * @brief Construct a new Size object
     * 
     * @param v double value containing size value
     * @param d pointer to the Dimension object
     * @param appr boolean value indicating if approximate
     * @param i integer value containing size identifier
     */
    Size(double v = 0.0, Dimension* d = nullptr, bool appr = false, int i = 0);

    /**
     * @brief Convert size to centimeters
     * 
     * @return double containing size in centimeters
     * @throws StorageException if dimension is not set
     */
    double toCentimeters() const;

    /**
     * @brief Scale the size by a factor
     * 
     * @param factor double value containing scaling factor
     */
    void scale(double factor);

    /**
     * @brief Check if size is zero
     * 
     * @return true if size is zero or less
     * @return false if size is greater than zero
     */
    bool isZero() const;
};

/**
 * @class Artwork
 * @brief Main class for representing artworks in the gallery
 * 
 * Represents an artwork with comprehensive information including
 * name, size, value, fragility, authentication status, and insurance value.
 * Provides operations for size management and authentication.
 */
class Artwork {
private:
    const char* name;           ///< Name of the artwork
    Size size;                  ///< Size of the artwork
    double value;               ///< Monetary value of the artwork
    bool fragile;               ///< Whether the artwork is fragile
    int year;                   ///< Year the artwork was created
    const char* artist;         ///< Name of the artist
    bool authenticated;          ///< Whether the artwork is authenticated
    double insuranceValue;       ///< Insurance value of the artwork

public:
    /**
     * @brief Construct a new Artwork object
     * 
     * @param n constant pointer to the artwork name
     * @param s constant reference to the Size object
     * @param v double value containing artwork value
     * @param fr boolean value indicating if fragile
     */
    Artwork(const char* n, const Size& s, double v, bool fr);

    /**
     * @brief Add size to the artwork
     * 
     * @param v double value containing size to add in centimeters
     */
    void addSize(double v);

    /**
     * @brief Use a portion of the artwork size
     * 
     * @param vCm double value containing size to use in centimeters
     * @throws NotEnoughArtworkException if requested size exceeds available size
     */
    void useSize(double vCm);

    /**
     * @brief Check if artwork is fragile
     * 
     * @return true if artwork is fragile
     * @return false if artwork is not fragile
     */
    bool isFragile() const;

    /**
     * @brief Authenticate the artwork
     */
    void authenticate();

    /**
     * @brief Set the insurance value
     * 
     * @param val double value containing insurance value
     */
    void setInsuranceValue(double val);

    /**
     * @brief Get the year the artwork was created
     * 
     * @return int containing creation year
     */
    int getYear() const;

    /**
     * @brief Get the artist name
     * 
     * @return const char* containing artist name
     */
    const char* getArtist() const;
};

/**
 * @class GalleryTool
 * @brief Base class for all gallery tools
 * 
 * Provides common functionality for gallery tools including
 * availability checking, cleaning, and durability management.
 */
class GalleryTool {
protected:
    const char* name;        ///< Name of the tool
    bool clean;             ///< Whether the tool is clean
    bool available;         ///< Whether the tool is available
    bool busy;              ///< Whether the tool is currently in use
    int durability;         ///< Durability level of the tool

public:
    /**
     * @brief Construct a new GalleryTool object
     * 
     * @param n constant pointer to the tool name
     * @param c boolean value indicating if clean
     * @param a boolean value indicating if available
     * @param d integer value containing durability level
     */
    GalleryTool(const char* n = "tool",
                bool c = true,
                bool a = true,
                int d = 100);

    /**
     * @brief Use the tool
     * 
     * @throws ToolNotAvailableException if tool is not usable
     */
    void useTool();

    /**
     * @brief Clean the tool
     */
    void cleanTool();

    /**
     * @brief Break the tool (set unavailable and zero durability)
     */
    void breakTool();

    /**
     * @brief Check if tool is available
     * 
     * @return true if tool is available, clean, and has durability
     * @return false otherwise
     */
    bool isAvailable() const;
};

/**
 * @class Frame
 * @brief Class for gallery frames
 * 
 * Represents a frame tool with protective capabilities for artworks.
 */
class Frame : public GalleryTool {
private:
    bool protective;     ///< Whether the frame provides protection
    int width;           ///< Width of the frame
    int id;              ///< Unique identifier for the frame

public:
    /**
     * @brief Construct a new Frame object
     * 
     * @param n constant pointer to the frame name
     * @param p boolean value indicating if protective
     * @param w integer value containing frame width
     * @param i integer value containing frame identifier
     */
    Frame(const char* n = "Frame",
          bool p = true,
          int w = 50,
          int i = 0);

    /**
     * @brief Add protection to the frame
     */
    void addProtection();

    /**
     * @brief Remove protection from the frame
     */
    void removeProtection();

    /**
     * @brief Check if frame can protect artwork
     * 
     * @return true if frame is available and protective
     * @return false otherwise
     */
    bool canProtect() const;
};

/**
 * @class Lighting
 * @brief Class for gallery lighting systems
 * 
 * Represents lighting equipment with LED and dimming capabilities
 * for safe artwork illumination.
 */
class Lighting : public GalleryTool {
private:
    bool led;        ///< Whether the lighting uses LED technology
    bool dimmed;     ///< Whether the lighting is dimmed
    int id;          ///< Unique identifier for the lighting

public:
    /**
     * @brief Construct a new Lighting object
     * 
     * @param n constant pointer to the lighting name
     * @param l boolean value indicating if LED
     * @param d boolean value indicating if dimmed
     * @param i integer value containing lighting identifier
     */
    Lighting(const char* n = "Lighting",
             bool l = true,
             bool d = false,
             int i = 0);

    /**
     * @brief Dim the lighting
     */
    void dim();

    /**
     * @brief Brighten the lighting
     */
    void brighten();

    /**
     * @brief Check if lighting is safe for artwork
     * 
     * @return true if LED, not dimmed, and available
     * @return false otherwise
     */
    bool isSafeForArtwork() const;
};

/**
 * @class SecurityCamera
 * @brief Class for security camera systems
 * 
 * Represents security cameras with recording and angle control capabilities.
 */
class SecurityCamera : public GalleryTool {
private:
    double angle;        ///< Camera viewing angle in degrees
    bool recording;      ///< Whether the camera is recording
    bool active;         ///< Whether the camera is active

public:
    /**
     * @brief Construct a new SecurityCamera object
     * 
     * @param n constant pointer to the camera name
     * @param a double value containing viewing angle
     * @param r boolean value indicating if recording
     * @param act boolean value indicating if active
     */
    SecurityCamera(const char* n = "Camera",
                   double a = 90.0,
                   bool r = false,
                   bool act = false);

    /**
     * @brief Start recording
     * 
     * @throws ToolNotAvailableException if camera is not available
     */
    void startRecording();

    /**
     * @brief Stop recording
     */
    void stopRecording();

    /**
     * @brief Check if camera is recording
     * 
     * @return true if camera is recording
     * @return false otherwise
     */
    bool isRecording() const;
};

/**
 * @class DisplayStand
 * @brief Class for display stands
 * 
 * Represents display stands with capacity management for artworks.
 */
class DisplayStand : public GalleryTool {
private:
    double capacity;     ///< Display capacity in square meters
    bool hasCover;       ///< Whether the stand has a cover
    bool inUse;          ///< Whether the stand is currently in use

public:
    /**
     * @brief Construct a new DisplayStand object
     * 
     * @param n constant pointer to the stand name
     * @param c double value containing capacity in square meters
     * @param hc boolean value indicating if has cover
     * @param u boolean value indicating if in use
     */
    DisplayStand(const char* n = "Stand",
                 double c = 2.0,
                 bool hc = true,
                 bool u = false);

    /**
     * @brief Start display on the stand
     * 
     * @throws ToolNotAvailableException if stand is not available
     */
    void startDisplay();

    /**
     * @brief Stop display on the stand
     */
    void stopDisplay();

    /**
     * @brief Check if stand can display artwork of given size
     * 
     * @param sqMeters double value containing size in square meters
     * @return true if capacity is sufficient
     * @return false otherwise
     */
    bool canDisplay(double sqMeters) const;
};

/**
 * @class Timer
 * @brief Class for timing operations
 * 
 * Represents a timer for tracking elapsed time and managing time-based operations.
 */
class Timer {
private:
    int seconds;     ///< Total seconds for the timer
    bool running;    ///< Whether the timer is running
    int elapsed;     ///< Elapsed seconds
    int id;          ///< Unique identifier for the timer

public:
    /**
     * @brief Construct a new Timer object
     * 
     * @param s integer value containing seconds
     * @param r boolean value indicating if running
     * @param e integer value containing elapsed seconds
     * @param i integer value containing timer identifier
     */
    Timer(int s = 0, bool r = false, int e = 0, int i = 0);

    /**
     * @brief Start the timer
     * 
     * @param s integer value containing seconds to set
     * @throws TimerNotSetException if seconds is not positive
     */
    void start(int s);

    /**
     * @brief Advance the timer by delta seconds
     * 
     * @param delta integer value containing seconds to advance
     */
    void tick(int delta);

    /**
     * @brief Check if timer is finished
     * 
     * @return true if timer is finished
     * @return false otherwise
     */
    bool isFinished() const;
};

/**
 * @class RestorationKit
 * @brief Class for artwork restoration kits
 * 
 * Represents restoration equipment with equip/unequip functionality.
 */
class RestorationKit : public GalleryTool {
private:
    bool equipped;   ///< Whether the restoration kit is equipped

public:
    /**
     * @brief Construct a new RestorationKit object
     * 
     * @param n constant pointer to the kit name
     * @param eq boolean value indicating if equipped
     */
    RestorationKit(const char* n = "RestorationKit",
                   bool eq = false);

    /**
     * @brief Equip the restoration kit
     * 
     * @return bool indicating if successfully equipped
     */
    bool equip();

    /**
     * @brief Unequip the restoration kit
     * 
     * @return bool indicating if successfully unequipped
     */
    bool unequip();

    /**
     * @brief Start restoration process
     * 
     * @throws ToolNotAvailableException if kit is not available or not equipped
     */
    void restore();
};

/**
 * @class CleaningKit
 * @brief Class for cleaning kits
 * 
 * Represents cleaning equipment for artwork maintenance.
 */
class CleaningKit : public GalleryTool {
private:
    int id;      ///< Unique identifier for the cleaning kit

public:
    /**
     * @brief Construct a new CleaningKit object
     * 
     * @param n constant pointer to the kit name
     * @param i integer value containing kit identifier
     */
    CleaningKit(const char* n = "CleaningKit",
                int i = 0);

    /**
     * @brief Use the cleaning kit
     * 
     * @throws ToolNotAvailableException if kit is not available
     */
    void clean();
};

/**
 * @class ConditionProfile
 * @brief Class for climate condition profiles
 * 
 * Represents temperature profiles for gradual climate transitions.
 */
class ConditionProfile {
private:
    double startTemp;      ///< Starting temperature
    double targetTemp;     ///< Target temperature
    int duration;          ///< Duration in seconds
    bool gradual;          ///< Whether transition is gradual

public:
    /**
     * @brief Construct a new ConditionProfile object
     * 
     * @param s double value containing start temperature
     * @param t double value containing target temperature
     * @param d integer value containing duration in seconds
     * @param g boolean value indicating if gradual transition
     */
    ConditionProfile(double s = 20.0, double t = 20.0,
                     int d = 600, bool g = true);

    /**
     * @brief Get current temperature based on elapsed time
     * 
     * @param elapsed integer value containing elapsed seconds
     * @return double containing current temperature
     */
    double currentTemp(int elapsed) const;

    /**
     * @brief Check if target temperature is reached
     * 
     * @param current double value containing current temperature
     * @return true if target is reached
     * @return false otherwise
     */
    bool isReached(double current) const;

    /**
     * @brief Reset the profile with new parameters
     * 
     * @param s double value containing start temperature
     * @param t double value containing target temperature
     * @param d integer value containing duration in seconds
     */
    void reset(double s, double t, int d);
};

/**
 * @class ClimateControl
 * @brief Class for climate control systems
 * 
 * Manages temperature, humidity, and door state for artwork preservation.
 */
class ClimateControl {
private:
    double temperature;        ///< Current temperature
    bool on;                   ///< Whether climate control is on
    bool doorClosed;           ///< Whether the door is closed
    Timer controlTimer;        ///< Timer for climate control
    ConditionProfile profile;  ///< Temperature transition profile
    int elapsedSeconds;        ///< Elapsed seconds since start
    double humidity;           ///< Current humidity level
    double targetHumidity;     ///< Target humidity level
    bool alarmActive;          ///< Whether alarm is active

public:
    /**
     * @brief Construct a new ClimateControl object
     * 
     * @param t double value containing initial temperature
     * @param o boolean value indicating if on
     * @param d boolean value indicating if door closed
     */
    ClimateControl(double t = 0.0, bool o = false, bool d = true);

    /**
     * @brief Set climate conditions
     * 
     * @param t double value containing target temperature
     * @param warmupMinutes integer value containing warmup time in minutes
     * @throws InvalidConditionException if temperature or warmup time is invalid or door is open
     */
    void setConditions(double t, int warmupMinutes = 10);

    /**
     * @brief Turn off climate control
     */
    void turnOff();

    /**
     * @brief Close the door
     */
    void closeDoor();

    /**
     * @brief Open the door
     */
    void openDoor();

    /**
     * @brief Set timer in minutes
     * 
     * @param minutes integer value containing minutes
     * @throws TimerNotSetException if minutes is not positive
     */
    void setTimerMinutes(int minutes);

    /**
     * @brief Advance time by delta seconds
     * 
     * @param secondsDelta integer value containing seconds to advance
     */
    void tick(int secondsDelta);

    /**
     * @brief Check if climate control is on
     * 
     * @return true if climate control is on
     * @return false otherwise
     */
    bool isOn() const;

    /**
     * @brief Get the current temperature
     * 
     * @return double containing current temperature
     */
    double getTemperature() const;

    /**
     * @brief Check if door is closed
     * 
     * @return true if door is closed
     * @return false otherwise
     */
    bool isDoorClosed() const;

    /**
     * @brief Set the humidity level
     * 
     * @param h double value containing humidity level
     */
    void setHumidity(double h);

    /**
     * @brief Activate the alarm
     */
    void activateAlarm();

    /**
     * @brief Check if alarm is active
     * 
     * @return true if alarm is active
     * @return false otherwise
     */
    bool isAlarmActive() const;
};

/**
 * @class SecuritySystem
 * @brief Class for security systems
 * 
 * Manages security zones and alarm states for gallery protection.
 */
class SecuritySystem {
private:
    int zones;          ///< Total number of security zones
    int activeZones;    ///< Number of currently active zones
    bool alarm;         ///< Whether alarm is enabled
    bool on;            ///< Whether security system is on

public:
    /**
     * @brief Construct a new SecuritySystem object
     * 
     * @param z integer value containing total zones
     * @param act integer value containing active zones
     * @param a boolean value indicating if alarm enabled
     * @param o boolean value indicating if system is on
     */
    SecuritySystem(int z = 4, int act = 0, bool a = true, bool o = false);

    /**
     * @brief Activate a security zone
     */
    void activateZone();

    /**
     * @brief Deactivate a security zone
     */
    void deactivateZone();

    /**
     * @brief Get number of free zones
     * 
     * @return int containing number of free zones
     */
    int freeZones() const;
};

class Curator;

/**
 * @class Exhibition
 * @brief Base class for all exhibition types
 * 
 * Abstract base class providing common interface for organizing exhibitions.
 */
class Exhibition {
protected:
    const char* name;   ///< Name of the exhibition

public:
    /**
     * @brief Construct a new Exhibition object
     * 
     * @param n constant pointer to the exhibition name
     */
    Exhibition(const char* n);

    /**
     * @brief Destroy the Exhibition object
     */
    virtual ~Exhibition() {}

    /**
     * @brief Organize the exhibition (pure virtual)
     */
    virtual void organize() = 0;

    /**
     * @brief Get the exhibition name
     * 
     * @return const char* containing exhibition name
     */
    const char* getName() const {
        return name;
    }
};

/**
 * @class PaintingExhibition
 * @brief Class for painting exhibitions
 * 
 * Represents a painting exhibition with two paintings, frame, lighting, and timer.
 */
/**
 * @class PaintingExhibition
 * @brief Class for painting exhibitions
 * 
 * Represents a painting exhibition with two paintings, frame, lighting, and timer.
 */
class PaintingExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* painting1;          ///< First painting artwork
    Artwork* painting2;          ///< Second painting artwork
    Frame* use_frame;            ///< Frame to use for paintings
    Lighting* use_lighting;       ///< Lighting system to use
    Timer* use_displayTimer;      ///< Timer for display duration
    Curator* curator;             ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new PaintingExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param p1 pointer to the first painting artwork
     * @param p2 pointer to the second painting artwork
     * @param f pointer to the Frame object
     * @param l pointer to the Lighting object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    PaintingExhibition(const char* n,
                      Artwork* p1,
                      Artwork* p2,
                      Frame* f,
                      Lighting* l,
                      Timer* t,
                      Curator* cu);

    /**
     * @brief Organize the painting exhibition
     */
    void organize() override;
};

/**
 * @class SculptureExhibition
 * @brief Class for sculpture exhibitions
 * 
 * Represents a sculpture exhibition with sculpture, display stand, lighting, and frame.
 */
class SculptureExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* sculpture;       ///< Sculpture artwork
    DisplayStand* stand;      ///< Display stand for sculpture
    Lighting* lighting;       ///< Lighting system
    Frame* frame;             ///< Frame for display
    Curator* curator;          ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new SculptureExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param s pointer to the sculpture artwork
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param f pointer to the Frame object
     * @param cu pointer to the Curator object
     */
    SculptureExhibition(const char* n,
                       Artwork* s,
                       DisplayStand* st,
                       Lighting* l,
                       Frame* f,
                       Curator* cu);

    /**
     * @brief Organize the sculpture exhibition
     */
    void organize() override;
};

/**
 * @class PhotographyExhibition
 * @brief Class for photography exhibitions
 * 
 * Represents a photography exhibition with two photographs, frame, lighting, security camera, and timer.
 */
class PhotographyExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* photo1;              ///< First photograph artwork
    Artwork* photo2;              ///< Second photograph artwork
    Frame* frame;                 ///< Frame for photographs
    Lighting* lighting;           ///< Lighting system
    SecurityCamera* camera;       ///< Security camera
    Timer* displayTimer;          ///< Timer for display duration
    Curator* curator;             ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new PhotographyExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param ph1 pointer to the first photograph artwork
     * @param ph2 pointer to the second photograph artwork
     * @param fr pointer to the Frame object
     * @param l pointer to the Lighting object
     * @param cam pointer to the SecurityCamera object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    PhotographyExhibition(const char* n,
                         Artwork* ph1,
                         Artwork* ph2,
                         Frame* fr,
                         Lighting* l,
                         SecurityCamera* cam,
                         Timer* t,
                         Curator* cu);

    /**
     * @brief Organize the photography exhibition
     */
    void organize() override;
};

/**
 * @class MixedMediaExhibition
 * @brief Class for mixed media exhibitions
 * 
 * Represents a mixed media exhibition with three artworks, display stand, lighting, frame, timer, and restoration kit.
 */
class MixedMediaExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* artwork1;              ///< First mixed media artwork
    Artwork* artwork2;             ///< Second mixed media artwork
    Artwork* artwork3;            ///< Third mixed media artwork
    DisplayStand* stand;          ///< Display stand
    Lighting* lighting;           ///< Lighting system
    Frame* frame;                 ///< Frame for display
    Timer* displayTimer;          ///< Timer for display duration
    RestorationKit* restorationKit; ///< Restoration kit
    Curator* curator;             ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new MixedMediaExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param a1 pointer to the first artwork
     * @param a2 pointer to the second artwork
     * @param a3 pointer to the third artwork
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param f pointer to the Frame object
     * @param t pointer to the Timer object
     * @param rk pointer to the RestorationKit object
     * @param cu pointer to the Curator object
     */
    MixedMediaExhibition(const char* n,
                        Artwork* a1,
                        Artwork* a2,
                        Artwork* a3,
                        DisplayStand* st,
                        Lighting* l,
                        Frame* f,
                        Timer* t,
                        RestorationKit* rk,
                        Curator* cu);

    /**
     * @brief Organize the mixed media exhibition
     */
    void organize() override;
};

/**
 * @class DigitalArtExhibition
 * @brief Class for digital art exhibitions
 * 
 * Represents a digital art exhibition with two digital artworks, display stand, lighting, security camera, and timer.
 */
class DigitalArtExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* digital1;             ///< First digital artwork
    Artwork* digital2;           ///< Second digital artwork
    DisplayStand* stand;          ///< Display stand
    Lighting* lighting;           ///< Lighting system
    SecurityCamera* camera;       ///< Security camera
    Timer* displayTimer;          ///< Timer for display duration
    Curator* curator;             ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new DigitalArtExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param d1 pointer to the first digital artwork
     * @param d2 pointer to the second digital artwork
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param cam pointer to the SecurityCamera object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    DigitalArtExhibition(const char* n,
                         Artwork* d1,
                         Artwork* d2,
                         DisplayStand* st,
                         Lighting* l,
                         SecurityCamera* cam,
                         Timer* t,
                         Curator* cu);

    /**
     * @brief Organize the digital art exhibition
     */
    void organize() override;
};

/**
 * @class VintageExhibition
 * @brief Class for vintage exhibitions
 * 
 * Represents a vintage exhibition with two vintage artworks, frame, climate control, and restoration kit.
 */
class VintageExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* vintage1;            ///< First vintage artwork
    Artwork* vintage2;           ///< Second vintage artwork
    Frame* frame;                ///< Frame for display
    ClimateControl* climate;     ///< Climate control system
    RestorationKit* restorationKit; ///< Restoration kit
    Curator* curator;            ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new VintageExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param v1 pointer to the first vintage artwork
     * @param v2 pointer to the second vintage artwork
     * @param f pointer to the Frame object
     * @param cc pointer to the ClimateControl object
     * @param rk pointer to the RestorationKit object
     * @param cu pointer to the Curator object
     */
    VintageExhibition(const char* n,
                      Artwork* v1,
                      Artwork* v2,
                      Frame* f,
                      ClimateControl* cc,
                      RestorationKit* rk,
                      Curator* cu);

    /**
     * @brief Organize the vintage exhibition
     */
    void organize() override;
};

/**
 * @class ContemporaryExhibition
 * @brief Class for contemporary exhibitions
 * 
 * Represents a contemporary exhibition with two contemporary artworks, display stand, lighting, security camera, and timer.
 */
class ContemporaryExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* contemporary1;       ///< First contemporary artwork
    Artwork* contemporary2;      ///< Second contemporary artwork
    DisplayStand* stand;          ///< Display stand
    Lighting* lighting;          ///< Lighting system
    SecurityCamera* camera;       ///< Security camera
    Timer* displayTimer;          ///< Timer for display duration
    Curator* curator;             ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new ContemporaryExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param c1 pointer to the first contemporary artwork
     * @param c2 pointer to the second contemporary artwork
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param cam pointer to the SecurityCamera object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    ContemporaryExhibition(const char* n,
                           Artwork* c1,
                           Artwork* c2,
                           DisplayStand* st,
                           Lighting* l,
                           SecurityCamera* cam,
                           Timer* t,
                           Curator* cu);

    /**
     * @brief Organize the contemporary exhibition
     */
    void organize() override;
};

/**
 * @class MinimalistExhibition
 * @brief Class for minimalist exhibitions
 * 
 * Represents a minimalist exhibition with one minimalist artwork, frame, and lighting.
 */
class MinimalistExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* minimalist;      ///< Minimalist artwork
    Frame* frame;            ///< Frame for display
    Lighting* lighting;       ///< Lighting system
    Curator* curator;         ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new MinimalistExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param m pointer to the minimalist artwork
     * @param f pointer to the Frame object
     * @param l pointer to the Lighting object
     * @param cu pointer to the Curator object
     */
    MinimalistExhibition(const char* n,
                         Artwork* m,
                         Frame* f,
                         Lighting* l,
                         Curator* cu);

    /**
     * @brief Organize the minimalist exhibition
     */
    void organize() override;
};

/**
 * @class AbstractExhibition
 * @brief Class for abstract art exhibitions
 * 
 * Represents an abstract art exhibition with two abstract artworks, display stand, lighting, frame, and timer.
 */
class AbstractExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* abstract1;        ///< First abstract artwork
    Artwork* abstract2;       ///< Second abstract artwork
    DisplayStand* stand;      ///< Display stand
    Lighting* lighting;       ///< Lighting system
    Frame* frame;             ///< Frame for display
    Timer* displayTimer;      ///< Timer for display duration
    Curator* curator;          ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new AbstractExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param a1 pointer to the first abstract artwork
     * @param a2 pointer to the second abstract artwork
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param f pointer to the Frame object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    AbstractExhibition(const char* n,
                       Artwork* a1,
                       Artwork* a2,
                       DisplayStand* st,
                       Lighting* l,
                       Frame* f,
                       Timer* t,
                       Curator* cu);

    /**
     * @brief Organize the abstract exhibition
     */
    void organize() override;
};

/**
 * @class PortraitExhibition
 * @brief Class for portrait exhibitions
 * 
 * Represents a portrait exhibition with two portrait artworks, frame, lighting, and security camera.
 */
class PortraitExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* portrait1;        ///< First portrait artwork
    Artwork* portrait2;        ///< Second portrait artwork
    Frame* frame;             ///< Frame for display
    Lighting* lighting;        ///< Lighting system
    SecurityCamera* camera;    ///< Security camera
    Curator* curator;          ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new PortraitExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param p1 pointer to the first portrait artwork
     * @param p2 pointer to the second portrait artwork
     * @param f pointer to the Frame object
     * @param l pointer to the Lighting object
     * @param cam pointer to the SecurityCamera object
     * @param cu pointer to the Curator object
     */
    PortraitExhibition(const char* n,
                       Artwork* p1,
                       Artwork* p2,
                       Frame* f,
                       Lighting* l,
                       SecurityCamera* cam,
                       Curator* cu);

    /**
     * @brief Organize the portrait exhibition
     */
    void organize() override;
};

/**
 * @class LandscapeExhibition
 * @brief Class for landscape exhibitions
 * 
 * Represents a landscape exhibition with two landscape artworks, frame, lighting, and timer.
 */
class LandscapeExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* landscape1;        ///< First landscape artwork
    Artwork* landscape2;        ///< Second landscape artwork
    Frame* frame;              ///< Frame for display
    Lighting* lighting;        ///< Lighting system
    Timer* displayTimer;       ///< Timer for display duration
    Curator* curator;          ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new LandscapeExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param l1 pointer to the first landscape artwork
     * @param l2 pointer to the second landscape artwork
     * @param f pointer to the Frame object
     * @param l pointer to the Lighting object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    LandscapeExhibition(const char* n,
                       Artwork* l1,
                       Artwork* l2,
                       Frame* f,
                       Lighting* l,
                       Timer* t,
                       Curator* cu);

    /**
     * @brief Organize the landscape exhibition
     */
    void organize() override;
};

/**
 * @class RestorationExhibition
 * @brief Class for restoration exhibitions
 * 
 * Represents a restoration exhibition with artwork, restoration kit, cleaning kit, climate control, and timer.
 */
class RestorationExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* artwork;           ///< Artwork to restore
    RestorationKit* restorationKit; ///< Restoration kit
    CleaningKit* cleaningKit;  ///< Cleaning kit
    ClimateControl* climate;    ///< Climate control system
    Timer* restorationTimer;    ///< Timer for restoration process
    Curator* curator;           ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new RestorationExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param a pointer to the artwork to restore
     * @param rk pointer to the RestorationKit object
     * @param ck pointer to the CleaningKit object
     * @param cc pointer to the ClimateControl object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    RestorationExhibition(const char* n,
                           Artwork* a,
                           RestorationKit* rk,
                           CleaningKit* ck,
                           ClimateControl* cc,
                           Timer* t,
                           Curator* cu);

    /**
     * @brief Organize the restoration exhibition
     */
    void organize() override;
};

/**
 * @class TemporaryExhibition
 * @brief Class for temporary exhibitions
 * 
 * Represents a temporary exhibition with two temporary artworks, display stand, lighting, security camera, and timer.
 */
class TemporaryExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* temp1;             ///< First temporary artwork
    Artwork* temp2;            ///< Second temporary artwork
    DisplayStand* stand;       ///< Display stand
    Lighting* lighting;        ///< Lighting system
    SecurityCamera* camera;     ///< Security camera
    Timer* displayTimer;        ///< Timer for display duration
    Curator* curator;           ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new TemporaryExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param t1 pointer to the first temporary artwork
     * @param t2 pointer to the second temporary artwork
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param cam pointer to the SecurityCamera object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    TemporaryExhibition(const char* n,
                        Artwork* t1,
                        Artwork* t2,
                        DisplayStand* st,
                        Lighting* l,
                        SecurityCamera* cam,
                        Timer* t,
                        Curator* cu);

    /**
     * @brief Organize the temporary exhibition
     */
    void organize() override;
};

/**
 * @class PermanentExhibition
 * @brief Class for permanent exhibitions
 * 
 * Represents a permanent exhibition with two permanent artworks, frame, climate control, security system, and timer.
 */
class PermanentExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* permanent1;        ///< First permanent artwork
    Artwork* permanent2;       ///< Second permanent artwork
    Frame* frame;              ///< Frame for display
    ClimateControl* climate;    ///< Climate control system
    SecuritySystem* security;   ///< Security system
    Timer* displayTimer;        ///< Timer for display duration
    Curator* curator;           ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new PermanentExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param p1 pointer to the first permanent artwork
     * @param p2 pointer to the second permanent artwork
     * @param f pointer to the Frame object
     * @param cc pointer to the ClimateControl object
     * @param sec pointer to the SecuritySystem object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    PermanentExhibition(const char* n,
                       Artwork* p1,
                       Artwork* p2,
                       Frame* f,
                       ClimateControl* cc,
                       SecuritySystem* sec,
                       Timer* t,
                       Curator* cu);

    /**
     * @brief Organize the permanent exhibition
     */
    void organize() override;
};

/**
 * @class InteractiveExhibition
 * @brief Class for interactive exhibitions
 * 
 * Represents an interactive exhibition with two interactive artworks, display stand, lighting, security camera, and timer.
 */
class InteractiveExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* interactive1;      ///< First interactive artwork
    Artwork* interactive2;     ///< Second interactive artwork
    DisplayStand* stand;        ///< Display stand
    Lighting* lighting;        ///< Lighting system
    SecurityCamera* camera;     ///< Security camera
    Timer* displayTimer;       ///< Timer for display duration
    Curator* curator;           ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new InteractiveExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param i1 pointer to the first interactive artwork
     * @param i2 pointer to the second interactive artwork
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param cam pointer to the SecurityCamera object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    InteractiveExhibition(const char* n,
                         Artwork* i1,
                         Artwork* i2,
                         DisplayStand* st,
                         Lighting* l,
                         SecurityCamera* cam,
                         Timer* t,
                         Curator* cu);

    /**
     * @brief Organize the interactive exhibition
     */
    void organize() override;
};

/**
 * @class ThematicExhibition
 * @brief Class for thematic exhibitions
 * 
 * Represents a thematic exhibition with three thematic artworks, frame, display stand, lighting, and timer.
 */
class ThematicExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* theme1;            ///< First thematic artwork
    Artwork* theme2;           ///< Second thematic artwork
    Artwork* theme3;           ///< Third thematic artwork
    Frame* frame;              ///< Frame for display
    DisplayStand* stand;       ///< Display stand
    Lighting* lighting;        ///< Lighting system
    Timer* displayTimer;       ///< Timer for display duration
    Curator* curator;           ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new ThematicExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param th1 pointer to the first thematic artwork
     * @param th2 pointer to the second thematic artwork
     * @param th3 pointer to the third thematic artwork
     * @param f pointer to the Frame object
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    ThematicExhibition(const char* n,
                      Artwork* th1,
                      Artwork* th2,
                      Artwork* th3,
                      Frame* f,
                      DisplayStand* st,
                      Lighting* l,
                      Timer* t,
                      Curator* cu);

    /**
     * @brief Organize the thematic exhibition
     */
    void organize() override;
};

/**
 * @class RetrospectiveExhibition
 * @brief Class for retrospective exhibitions
 * 
 * Represents a retrospective exhibition with two retrospective artworks, frame, climate control, restoration kit, and timer.
 */
class RetrospectiveExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* retro1;            ///< First retrospective artwork
    Artwork* retro2;           ///< Second retrospective artwork
    Frame* frame;              ///< Frame for display
    ClimateControl* climate;    ///< Climate control system
    RestorationKit* restorationKit; ///< Restoration kit
    Timer* displayTimer;        ///< Timer for display duration
    Curator* curator;           ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new RetrospectiveExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param r1 pointer to the first retrospective artwork
     * @param r2 pointer to the second retrospective artwork
     * @param f pointer to the Frame object
     * @param cc pointer to the ClimateControl object
     * @param rk pointer to the RestorationKit object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    RetrospectiveExhibition(const char* n,
                            Artwork* r1,
                            Artwork* r2,
                            Frame* f,
                            ClimateControl* cc,
                            RestorationKit* rk,
                            Timer* t,
                            Curator* cu);

    /**
     * @brief Organize the retrospective exhibition
     */
    void organize() override;
};

/**
 * @class GroupExhibition
 * @brief Class for group exhibitions
 * 
 * Represents a group exhibition with three group artworks, display stand, lighting, frame, security camera, and timer.
 */
class GroupExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* group1;            ///< First group artwork
    Artwork* group2;            ///< Second group artwork
    Artwork* group3;            ///< Third group artwork
    DisplayStand* stand;        ///< Display stand
    Lighting* lighting;         ///< Lighting system
    Frame* frame;               ///< Frame for display
    SecurityCamera* camera;      ///< Security camera
    Timer* displayTimer;        ///< Timer for display duration
    Curator* curator;            ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new GroupExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param g1 pointer to the first group artwork
     * @param g2 pointer to the second group artwork
     * @param g3 pointer to the third group artwork
     * @param st pointer to the DisplayStand object
     * @param l pointer to the Lighting object
     * @param f pointer to the Frame object
     * @param cam pointer to the SecurityCamera object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    GroupExhibition(const char* n,
                   Artwork* g1,
                   Artwork* g2,
                   Artwork* g3,
                   DisplayStand* st,
                   Lighting* l,
                   Frame* f,
                   SecurityCamera* cam,
                   Timer* t,
                   Curator* cu);

    /**
     * @brief Organize the group exhibition
     */
    void organize() override;
};

/**
 * @class SoloExhibition
 * @brief Class for solo exhibitions
 * 
 * Represents a solo exhibition with two solo artworks, frame, lighting, display stand, and timer.
 */
class SoloExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* solo1;             ///< First solo artwork
    Artwork* solo2;             ///< Second solo artwork
    Frame* frame;               ///< Frame for display
    Lighting* lighting;         ///< Lighting system
    DisplayStand* stand;        ///< Display stand
    Timer* displayTimer;         ///< Timer for display duration
    Curator* curator;            ///< Curator organizing the exhibition

public:
    /**
     * @brief Construct a new SoloExhibition object
     * 
     * @param n constant pointer to the exhibition name
     * @param s1 pointer to the first solo artwork
     * @param s2 pointer to the second solo artwork
     * @param f pointer to the Frame object
     * @param l pointer to the Lighting object
     * @param st pointer to the DisplayStand object
     * @param t pointer to the Timer object
     * @param cu pointer to the Curator object
     */
    SoloExhibition(const char* n,
                   Artwork* s1,
                   Artwork* s2,
                   Frame* f,
                   Lighting* l,
                   DisplayStand* st,
                   Timer* t,
                   Curator* cu);

    /**
     * @brief Organize the solo exhibition
     */
    void organize() override;
};

/**
 * @class Curator
 * @brief Class for gallery curators
 * 
 * Manages organization of all types of exhibitions in the gallery.
 */
class Curator {
private:
    const char* name;    ///< Name of the curator

public:
    /**
     * @brief Construct a new Curator object
     * 
     * @param n constant pointer to the curator name
     */
    Curator(const char* n = "Curator");

    /**
     * @brief Organize a painting exhibition
     * 
     * @param exhibition pointer to the PaintingExhibition object
     */
    void organizePainting(PaintingExhibition* exhibition);

    /**
     * @brief Organize a sculpture exhibition
     * 
     * @param exhibition pointer to the SculptureExhibition object
     */
    void organizeSculpture(SculptureExhibition* exhibition);

    /**
     * @brief Organize a photography exhibition
     * 
     * @param exhibition pointer to the PhotographyExhibition object
     */
    void organizePhotography(PhotographyExhibition* exhibition);

    /**
     * @brief Organize a mixed media exhibition
     * 
     * @param exhibition pointer to the MixedMediaExhibition object
     */
    void organizeMixedMedia(MixedMediaExhibition* exhibition);

    /**
     * @brief Organize a digital art exhibition
     * 
     * @param exhibition pointer to the DigitalArtExhibition object
     */
    void organizeDigitalArt(DigitalArtExhibition* exhibition);

    /**
     * @brief Organize a vintage exhibition
     * 
     * @param exhibition pointer to the VintageExhibition object
     */
    void organizeVintage(VintageExhibition* exhibition);

    /**
     * @brief Organize a contemporary exhibition
     * 
     * @param exhibition pointer to the ContemporaryExhibition object
     */
    void organizeContemporary(ContemporaryExhibition* exhibition);

    /**
     * @brief Organize a minimalist exhibition
     * 
     * @param exhibition pointer to the MinimalistExhibition object
     */
    void organizeMinimalist(MinimalistExhibition* exhibition);

    /**
     * @brief Organize an abstract exhibition
     * 
     * @param exhibition pointer to the AbstractExhibition object
     */
    void organizeAbstract(AbstractExhibition* exhibition);

    /**
     * @brief Organize a portrait exhibition
     * 
     * @param exhibition pointer to the PortraitExhibition object
     */
    void organizePortrait(PortraitExhibition* exhibition);

    /**
     * @brief Organize a landscape exhibition
     * 
     * @param exhibition pointer to the LandscapeExhibition object
     */
    void organizeLandscape(LandscapeExhibition* exhibition);

    /**
     * @brief Organize a restoration exhibition
     * 
     * @param exhibition pointer to the RestorationExhibition object
     */
    void organizeRestoration(RestorationExhibition* exhibition);

    /**
     * @brief Organize a temporary exhibition
     * 
     * @param exhibition pointer to the TemporaryExhibition object
     */
    void organizeTemporary(TemporaryExhibition* exhibition);

    /**
     * @brief Organize a permanent exhibition
     * 
     * @param exhibition pointer to the PermanentExhibition object
     */
    void organizePermanent(PermanentExhibition* exhibition);

    /**
     * @brief Organize an interactive exhibition
     * 
     * @param exhibition pointer to the InteractiveExhibition object
     */
    void organizeInteractive(InteractiveExhibition* exhibition);

    /**
     * @brief Organize a thematic exhibition
     * 
     * @param exhibition pointer to the ThematicExhibition object
     */
    void organizeThematic(ThematicExhibition* exhibition);

    /**
     * @brief Organize a retrospective exhibition
     * 
     * @param exhibition pointer to the RetrospectiveExhibition object
     */
    void organizeRetrospective(RetrospectiveExhibition* exhibition);

    /**
     * @brief Organize a group exhibition
     * 
     * @param exhibition pointer to the GroupExhibition object
     */
    void organizeGroup(GroupExhibition* exhibition);

    /**
     * @brief Organize a solo exhibition
     * 
     * @param exhibition pointer to the SoloExhibition object
     */
    void organizeSolo(SoloExhibition* exhibition);
};

/**
 * @class Visitor
 * @brief Class for gallery visitors
 * 
 * Represents a visitor with access level, ticket status, and visit tracking.
 */
class Visitor {
private:
    const char* name;        ///< Name of the visitor
    int accessLevel;          ///< Access level of the visitor
    bool hasTicket;          ///< Whether the visitor has a ticket
    double visitDuration;    ///< Duration of the visit in minutes
    int favoriteCount;       ///< Number of favorite artworks
    bool guidedTour;         ///< Whether the visitor is on a guided tour

public:
    /**
     * @brief Construct a new Visitor object
     * 
     * @param n constant pointer to the visitor name
     * @param al integer value containing access level
     * @param ht boolean value indicating if has ticket
     */
    Visitor(const char* n = "Visitor", int al = 1, bool ht = false);

    /**
     * @brief Purchase a ticket
     */
    void purchaseTicket();

    /**
     * @brief Enter the gallery
     * 
     * @throws InvalidAccessException if visitor does not have a ticket
     */
    void enterGallery();

    /**
     * @brief View an artwork
     * 
     * @param artwork pointer to the Artwork object to view
     * @throws ArtworkNotFoundException if artwork is null
     */
    void viewArtwork(Artwork* artwork);

    /**
     * @brief Add an artwork to favorites
     */
    void addFavorite();

    /**
     * @brief Join a guided tour
     */
    void joinGuidedTour();

    /**
     * @brief Check if visitor can access restricted areas
     * 
     * @return true if access level is 3 or higher and has ticket
     * @return false otherwise
     */
    bool canAccessRestricted() const;

    /**
     * @brief Get the access level
     * 
     * @return int containing access level
     */
    int getAccessLevel() const;
};

/**
 * @class GalleryMenu
 * @brief Class for gallery menu system
 * 
 * Manages exhibitions and provides interactive menu for visitors.
 */
class GalleryMenu {
private:
    static const int MAX_EXHIBITIONS = 50;   ///< Maximum number of exhibitions
    Exhibition* exhibitions[MAX_EXHIBITIONS]; ///< Array of exhibition pointers
    int exhibitionCount;                      ///< Current number of exhibitions
    Visitor* currentVisitor;                  ///< Current visitor

public:
    /**
     * @brief Construct a new GalleryMenu object
     */
    GalleryMenu();

    /**
     * @brief Show the menu
     */
    void show() const;

    /**
     * @brief Add an exhibition to the menu
     * 
     * @param e pointer to the Exhibition object to add
     */
    void addExhibition(Exhibition* e);

    /**
     * @brief Set the current visitor
     * 
     * @param v pointer to the Visitor object
     */
    void setVisitor(Visitor* v);

    /**
     * @brief Run the menu system
     */
    void run();
};