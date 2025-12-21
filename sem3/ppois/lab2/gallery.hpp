#pragma once
#include <iostream>
#include <iomanip>
#include <stdexcept>
using namespace std;
class ArtworkNotFoundException : public runtime_error {
public:
    ArtworkNotFoundException(const char* msg) : runtime_error(msg) {}
};
class NotEnoughArtworkException : public runtime_error {
public:
    NotEnoughArtworkException(const char* msg) : runtime_error(msg) {}
};
class ToolNotAvailableException : public runtime_error {
public:
    ToolNotAvailableException(const char* msg) : runtime_error(msg) {}
};
class InvalidConditionException : public runtime_error {
public:
    InvalidConditionException(const char* msg) : runtime_error(msg) {}
};
class TimerNotSetException : public runtime_error {
public:
    TimerNotSetException(const char* msg) : runtime_error(msg) {}
};
class OverexposedArtworkException : public runtime_error {
public:
    OverexposedArtworkException(const char* msg) : runtime_error(msg) {}
};
class UnderexposedArtworkException : public runtime_error {
public:
    UnderexposedArtworkException(const char* msg) : runtime_error(msg) {}
};
class StorageException : public runtime_error {
public:
    StorageException(const char* msg) : runtime_error(msg) {}
};
class SecurityBreachException : public runtime_error {
public:
    SecurityBreachException(const char* msg) : runtime_error(msg) {}
};
class RestorationFailedException : public runtime_error {
public:
    RestorationFailedException(const char* msg) : runtime_error(msg) {}
};
class InsufficientSpaceException : public runtime_error {
public:
    InsufficientSpaceException(const char* msg) : runtime_error(msg) {}
};
class InvalidAccessException : public runtime_error {
public:
    InvalidAccessException(const char* msg) : runtime_error(msg) {}
};
class Dimension {
private:
    const char* name;
    double cmPerUnit;
    bool metric;
    int id;
    double precision;
    bool standard;
public:
    Dimension(const char* n = "unit", double c = 1.0, bool m = true, int i = 0);
    double toCentimeters(double amount) const;
    bool isMetric() const;
    int getId() const;
    void setPrecision(double p);
    bool isStandard() const;
};
class Size {
private:
    double value;
    Dimension* dimension;
    bool approximate;
    int id;
public:
    Size(double v = 0.0, Dimension* d = nullptr, bool appr = false, int i = 0);
    double toCentimeters() const;
    void scale(double factor);
    bool isZero() const;
};
class Artwork {
private:
    const char* name;
    Size size;
    double value;
    bool fragile;
    int year;
    const char* artist;
    bool authenticated;
    double insuranceValue;
public:
    Artwork(const char* n, const Size& s, double v, bool fr);
    void addSize(double v);
    void useSize(double vCm);
    bool isFragile() const;
    void authenticate();
    void setInsuranceValue(double val);
    int getYear() const;
    const char* getArtist() const;
};
class GalleryTool {
protected:
    const char* name;
    bool clean;
    bool available;
    bool busy;
    int durability;
public:
    GalleryTool(const char* n = "tool",
                bool c = true,
                bool a = true,
                int d = 100);
    void useTool();
    void cleanTool();
    void breakTool();
    bool isAvailable() const;
};
class Frame : public GalleryTool {
private:
    bool protective;
    int width;
    int id;
public:
    Frame(const char* n = "Frame",
          bool p = true,
          int w = 50,
          int i = 0);
    void addProtection();
    void removeProtection();
    bool canProtect() const;
};
class Lighting : public GalleryTool {
private:
    bool led;
    bool dimmed;
    int id;
public:
    Lighting(const char* n = "Lighting",
             bool l = true,
             bool d = false,
             int i = 0);
    void dim();
    void brighten();
    bool isSafeForArtwork() const;
};
class SecurityCamera : public GalleryTool {
private:
    double angle;
    bool recording;
    bool active;
public:
    SecurityCamera(const char* n = "Camera",
                   double a = 90.0,
                   bool r = false,
                   bool act = false);
    void startRecording();
    void stopRecording();
    bool isRecording() const;
};
class DisplayStand : public GalleryTool {
private:
    double capacity;
    bool hasCover;
    bool inUse;
public:
    DisplayStand(const char* n = "Stand",
                 double c = 2.0,
                 bool hc = true,
                 bool u = false);
    void startDisplay();
    void stopDisplay();
    bool canDisplay(double sqMeters) const;
};
class Timer {
private:
    int seconds;
    bool running;
    int elapsed;
    int id;
public:
    Timer(int s = 0, bool r = false, int e = 0, int i = 0);
    void start(int s);
    void tick(int delta);
    bool isFinished() const;
};
class RestorationKit : public GalleryTool {
private:
    bool equipped;
public:
    RestorationKit(const char* n = "RestorationKit",
                   bool eq = false);
    bool equip();
    bool unequip();
    void restore();
};
class CleaningKit : public GalleryTool {
private:
    int id;
public:
    CleaningKit(const char* n = "CleaningKit",
                int i = 0);
    void clean();
};
class ConditionProfile {
private:
    double startTemp;
    double targetTemp;
    int duration;
    bool gradual;
public:
    ConditionProfile(double s = 20.0, double t = 20.0,
                     int d = 600, bool g = true);
    double currentTemp(int elapsed) const;
    bool isReached(double current) const;
    void reset(double s, double t, int d);
};
class ClimateControl {
private:
    double temperature;
    bool on;
    bool doorClosed;
    Timer controlTimer;
    ConditionProfile profile;
    int elapsedSeconds;
    double humidity;
    double targetHumidity;
    bool alarmActive;
public:
    ClimateControl(double t = 0.0, bool o = false, bool d = true);
    void setConditions(double t, int warmupMinutes = 10);
    void turnOff();
    void closeDoor();
    void openDoor();
    void setTimerMinutes(int minutes);
    void tick(int secondsDelta);
    bool isOn() const;
    double getTemperature() const;
    bool isDoorClosed() const;
    void setHumidity(double h);
    void activateAlarm();
    bool isAlarmActive() const;
};
class SecuritySystem {
private:
    int zones;
    int activeZones;
    bool alarm;
    bool on;
public:
    SecuritySystem(int z = 4, int act = 0, bool a = true, bool o = false);
    void activateZone();
    void deactivateZone();
    int freeZones() const;
};
class Curator;
class Exhibition {
protected:
    const char* name;
public:
    Exhibition(const char* n);
    virtual ~Exhibition() {}
    virtual void organize() = 0;
    const char* getName() const {
        return name;
    }
};
class PaintingExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* painting1;
    Artwork* painting2;
    Frame* use_frame;
    Lighting* use_lighting;
    Timer* use_displayTimer;
    Curator* curator;
public:
    PaintingExhibition(const char* n,
                      Artwork* p1,
                      Artwork* p2,
                      Frame* f,
                      Lighting* l,
                      Timer* t,
                      Curator* cu);
    void organize() override;
};
class SculptureExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* sculpture;
    DisplayStand* stand;
    Lighting* lighting;
    Frame* frame;
    Curator* curator;
public:
    SculptureExhibition(const char* n,
                       Artwork* s,
                       DisplayStand* st,
                       Lighting* l,
                       Frame* f,
                       Curator* cu);
    void organize() override;
};
class PhotographyExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* photo1;
    Artwork* photo2;
    Frame* frame;
    Lighting* lighting;
    SecurityCamera* camera;
    Timer* displayTimer;
    Curator* curator;
public:
    PhotographyExhibition(const char* n,
                         Artwork* ph1,
                         Artwork* ph2,
                         Frame* fr,
                         Lighting* l,
                         SecurityCamera* cam,
                         Timer* t,
                         Curator* cu);
    void organize() override;
};
class MixedMediaExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* artwork1;
    Artwork* artwork2;
    Artwork* artwork3;
    DisplayStand* stand;
    Lighting* lighting;
    Frame* frame;
    Timer* displayTimer;
    RestorationKit* restorationKit;
    Curator* curator;
public:
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
    void organize() override;
};
class DigitalArtExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* digital1;
    Artwork* digital2;
    DisplayStand* stand;
    Lighting* lighting;
    SecurityCamera* camera;
    Timer* displayTimer;
    Curator* curator;
public:
    DigitalArtExhibition(const char* n,
                         Artwork* d1,
                         Artwork* d2,
                         DisplayStand* st,
                         Lighting* l,
                         SecurityCamera* cam,
                         Timer* t,
                         Curator* cu);
    void organize() override;
};
class VintageExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* vintage1;
    Artwork* vintage2;
    Frame* frame;
    ClimateControl* climate;
    RestorationKit* restorationKit;
    Curator* curator;
public:
    VintageExhibition(const char* n,
                      Artwork* v1,
                      Artwork* v2,
                      Frame* f,
                      ClimateControl* cc,
                      RestorationKit* rk,
                      Curator* cu);
    void organize() override;
};
class ContemporaryExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* contemporary1;
    Artwork* contemporary2;
    DisplayStand* stand;
    Lighting* lighting;
    SecurityCamera* camera;
    Timer* displayTimer;
    Curator* curator;
public:
    ContemporaryExhibition(const char* n,
                           Artwork* c1,
                           Artwork* c2,
                           DisplayStand* st,
                           Lighting* l,
                           SecurityCamera* cam,
                           Timer* t,
                           Curator* cu);
    void organize() override;
};
class MinimalistExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* minimalist;
    Frame* frame;
    Lighting* lighting;
    Curator* curator;
public:
    MinimalistExhibition(const char* n,
                         Artwork* m,
                         Frame* f,
                         Lighting* l,
                         Curator* cu);
    void organize() override;
};
class AbstractExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* abstract1;
    Artwork* abstract2;
    DisplayStand* stand;
    Lighting* lighting;
    Frame* frame;
    Timer* displayTimer;
    Curator* curator;
public:
    AbstractExhibition(const char* n,
                       Artwork* a1,
                       Artwork* a2,
                       DisplayStand* st,
                       Lighting* l,
                       Frame* f,
                       Timer* t,
                       Curator* cu);
    void organize() override;
};
class PortraitExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* portrait1;
    Artwork* portrait2;
    Frame* frame;
    Lighting* lighting;
    SecurityCamera* camera;
    Curator* curator;
public:
    PortraitExhibition(const char* n,
                       Artwork* p1,
                       Artwork* p2,
                       Frame* f,
                       Lighting* l,
                       SecurityCamera* cam,
                       Curator* cu);
    void organize() override;
};
class LandscapeExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* landscape1;
    Artwork* landscape2;
    Frame* frame;
    Lighting* lighting;
    Timer* displayTimer;
    Curator* curator;
public:
    LandscapeExhibition(const char* n,
                       Artwork* l1,
                       Artwork* l2,
                       Frame* f,
                       Lighting* l,
                       Timer* t,
                       Curator* cu);
    void organize() override;
};
class RestorationExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* artwork;
    RestorationKit* restorationKit;
    CleaningKit* cleaningKit;
    ClimateControl* climate;
    Timer* restorationTimer;
    Curator* curator;
public:
    RestorationExhibition(const char* n,
                           Artwork* a,
                           RestorationKit* rk,
                           CleaningKit* ck,
                           ClimateControl* cc,
                           Timer* t,
                           Curator* cu);
    void organize() override;
};
class TemporaryExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* temp1;
    Artwork* temp2;
    DisplayStand* stand;
    Lighting* lighting;
    SecurityCamera* camera;
    Timer* displayTimer;
    Curator* curator;
public:
    TemporaryExhibition(const char* n,
                        Artwork* t1,
                        Artwork* t2,
                        DisplayStand* st,
                        Lighting* l,
                        SecurityCamera* cam,
                        Timer* t,
                        Curator* cu);
    void organize() override;
};
class PermanentExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* permanent1;
    Artwork* permanent2;
    Frame* frame;
    ClimateControl* climate;
    SecuritySystem* security;
    Timer* displayTimer;
    Curator* curator;
public:
    PermanentExhibition(const char* n,
                       Artwork* p1,
                       Artwork* p2,
                       Frame* f,
                       ClimateControl* cc,
                       SecuritySystem* sec,
                       Timer* t,
                       Curator* cu);
    void organize() override;
};
class InteractiveExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* interactive1;
    Artwork* interactive2;
    DisplayStand* stand;
    Lighting* lighting;
    SecurityCamera* camera;
    Timer* displayTimer;
    Curator* curator;
public:
    InteractiveExhibition(const char* n,
                         Artwork* i1,
                         Artwork* i2,
                         DisplayStand* st,
                         Lighting* l,
                         SecurityCamera* cam,
                         Timer* t,
                         Curator* cu);
    void organize() override;
};
class ThematicExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* theme1;
    Artwork* theme2;
    Artwork* theme3;
    Frame* frame;
    DisplayStand* stand;
    Lighting* lighting;
    Timer* displayTimer;
    Curator* curator;
public:
    ThematicExhibition(const char* n,
                      Artwork* th1,
                      Artwork* th2,
                      Artwork* th3,
                      Frame* f,
                      DisplayStand* st,
                      Lighting* l,
                      Timer* t,
                      Curator* cu);
    void organize() override;
};
class RetrospectiveExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* retro1;
    Artwork* retro2;
    Frame* frame;
    ClimateControl* climate;
    RestorationKit* restorationKit;
    Timer* displayTimer;
    Curator* curator;
public:
    RetrospectiveExhibition(const char* n,
                            Artwork* r1,
                            Artwork* r2,
                            Frame* f,
                            ClimateControl* cc,
                            RestorationKit* rk,
                            Timer* t,
                            Curator* cu);
    void organize() override;
};
class GroupExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* group1;
    Artwork* group2;
    Artwork* group3;
    DisplayStand* stand;
    Lighting* lighting;
    Frame* frame;
    SecurityCamera* camera;
    Timer* displayTimer;
    Curator* curator;
public:
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
    void organize() override;
};
class SoloExhibition : public Exhibition {
    friend class Curator;
private:
    Artwork* solo1;
    Artwork* solo2;
    Frame* frame;
    Lighting* lighting;
    DisplayStand* stand;
    Timer* displayTimer;
    Curator* curator;
public:
    SoloExhibition(const char* n,
                   Artwork* s1,
                   Artwork* s2,
                   Frame* f,
                   Lighting* l,
                   DisplayStand* st,
                   Timer* t,
                   Curator* cu);
    void organize() override;
};
class Curator {
private:
    const char* name;
public:
    Curator(const char* n = "Curator");
    void organizePainting(PaintingExhibition* exhibition);
    void organizeSculpture(SculptureExhibition* exhibition);
    void organizePhotography(PhotographyExhibition* exhibition);
    void organizeMixedMedia(MixedMediaExhibition* exhibition);
    void organizeDigitalArt(DigitalArtExhibition* exhibition);
    void organizeVintage(VintageExhibition* exhibition);
    void organizeContemporary(ContemporaryExhibition* exhibition);
    void organizeMinimalist(MinimalistExhibition* exhibition);
    void organizeAbstract(AbstractExhibition* exhibition);
    void organizePortrait(PortraitExhibition* exhibition);
    void organizeLandscape(LandscapeExhibition* exhibition);
    void organizeRestoration(RestorationExhibition* exhibition);
    void organizeTemporary(TemporaryExhibition* exhibition);
    void organizePermanent(PermanentExhibition* exhibition);
    void organizeInteractive(InteractiveExhibition* exhibition);
    void organizeThematic(ThematicExhibition* exhibition);
    void organizeRetrospective(RetrospectiveExhibition* exhibition);
    void organizeGroup(GroupExhibition* exhibition);
    void organizeSolo(SoloExhibition* exhibition);
};
class Visitor {
private:
    const char* name;
    int accessLevel;
    bool hasTicket;
    double visitDuration;
    int favoriteCount;
    bool guidedTour;
public:
    Visitor(const char* n = "Visitor", int al = 1, bool ht = false);
    void purchaseTicket();
    void enterGallery();
    void viewArtwork(Artwork* artwork);
    void addFavorite();
    void joinGuidedTour();
    bool canAccessRestricted() const;
    int getAccessLevel() const;
};
class GalleryMenu {
private:
    static const int MAX_EXHIBITIONS = 50;
    Exhibition* exhibitions[MAX_EXHIBITIONS];
    int exhibitionCount;
    Visitor* currentVisitor;
public:
    GalleryMenu();
    void show() const;
    void addExhibition(Exhibition* e);
    void setVisitor(Visitor* v);
    void run();
};