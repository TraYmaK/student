#include "gallery.hpp"
#include <cstring>
Dimension::Dimension(const char* n, double c, bool m, int i)
    : name(n), cmPerUnit(c), metric(m), id(i), precision(0.1), standard(true) {}
double Dimension::toCentimeters(double amount) const {
    return amount * cmPerUnit;
}
bool Dimension::isMetric() const {
    return metric;
}
int Dimension::getId() const {
    return id;
}
void Dimension::setPrecision(double p) {
    precision = p;
}
bool Dimension::isStandard() const {
    return standard;
}
double Dimension::fromCentimeters(double cm) const {
    if (cmPerUnit <= 0.0) return 0.0;
    return cm / cmPerUnit;
}
double Dimension::convertTo(double amount, Dimension* targetDimension) const {
    if (!targetDimension) {
        return 0.0;
    }
    double cm = toCentimeters(amount);
    return targetDimension->fromCentimeters(cm);
}
bool Dimension::validateConversion(double amount) const {
    return amount >= 0.0 && cmPerUnit > 0.0;
}
Size::Size(double v, Dimension* d, bool appr, int i)
    : value(v), dimension(d), approximate(appr), id(i) {}
double Size::toCentimeters() const {
    if (!dimension) {
        throw StorageException("Dimension is not set for size");
    }
    return dimension->toCentimeters(value);
}
void Size::scale(double factor) {
    value *= factor;
}
bool Size::isZero() const {
    return value <= 0.0;
}
double Size::getValue() const {
    return value;
}
bool Size::isApproximate() const {
    return approximate;
}
double Size::convertToDimension(Dimension* targetDimension) const {
    if (!targetDimension || !dimension) {
        return 0.0;
    }
    double cm = toCentimeters();
    return targetDimension->fromCentimeters(cm);
}
double Size::calculateDifference(const Size& otherSize) const {
    double thisCm = toCentimeters();
    double otherCm = otherSize.toCentimeters();
    return thisCm > otherCm ? thisCm - otherCm : otherCm - thisCm;
}
Artwork::Artwork(const char* n, const Size& s, double v, bool fr)
    : name(n), size(s), value(v), fragile(fr), year(0), artist("Unknown"), authenticated(false), insuranceValue(0.0) {}
void Artwork::addSize(double v) {
    double oldCm = size.toCentimeters();
    double newCm = oldCm + v;
    if (oldCm > 0.0) {
        double factor = newCm / oldCm;
        size.scale(factor);
    }
}
void Artwork::useSize(double vCm) {
    double cm = size.toCentimeters();
    if (vCm > cm) {
        throw NotEnoughArtworkException("Not enough artwork size");
    }
    if (cm > 0.0) {
        double factor = (cm - vCm) / cm;
        size.scale(factor);
    }
}
bool Artwork::isFragile() const {
    return fragile;
}
void Artwork::authenticate() {
    authenticated = true;
}
void Artwork::setInsuranceValue(double val) {
    insuranceValue = val;
}
int Artwork::getYear() const {
    return year;
}
const char* Artwork::getArtist() const {
    return artist;
}
const char* Artwork::getName() const {
    return name;
}
double Artwork::getValue() const {
    return value;
}
bool Artwork::isAuthenticated() const {
    return authenticated;
}
double Artwork::calculateMaintenanceCost() const {
    double baseCost = 1000.0;
    if (authenticated) {
        baseCost *= 0.9;
    }
    int age = 2025 - year;
    return baseCost * (1.0 + age * 0.05);
}
bool Artwork::isAvailableForDisplay() const {
    return authenticated && !fragile;
}
double Artwork::calculateDepreciation(int currentYear) const {
    if (year <= 0 || currentYear < year) {
        return value;
    }
    int age = currentYear - year;
    double depreciationRate = 0.1;
    return value * (1.0 - depreciationRate * age);
}
bool Artwork::needsInspection() const {
    if (year == 0) {
        return false;
    }
    int age = 2025 - year;
    return age > 5;
}
double Artwork::calculateRestorationCost(double damageLevel) const {
    if (damageLevel <= 0.0) {
        return 0.0;
    }
    double baseCost = 500.0;
    return baseCost * damageLevel;
}
GalleryTool::GalleryTool(const char* n, bool c, bool a, int d)
    : name(n), clean(c), available(a), busy(false), durability(d) {}
void GalleryTool::useTool() {
    if (!available || !clean || durability <= 0) {
        throw ToolNotAvailableException("Tool not usable (unavailable, dirty, or broken)");
    }
    busy = true;
    durability--;
    if (durability <= 0) {
        durability = 0;
        available = false;
    }
    busy = false;
}
void GalleryTool::cleanTool() {
    clean = true;
}
void GalleryTool::breakTool() {
    available = false;
    durability = 0;
}
bool GalleryTool::isAvailable() const {
    return available && clean && durability > 0;
}
const char* GalleryTool::getName() const {
    return name;
}
Frame::Frame(const char* n, bool p, int w, int i)
    : GalleryTool(n, true, true, 100),
      protective(p), width(w), id(i) {}
void Frame::addProtection() {
    protective = true;
}
void Frame::removeProtection() {
    protective = false;
}
bool Frame::canProtect() const {
    return isAvailable() && protective;
}
int Frame::getWidth() const {
    return width;
}
double Frame::calculateComfortScore() const {
    double score = 5.0;
    if (protective) {
        score += 2.0;
    }
    if (width > 50) {
        score += 1.0;
    }
    if (isAvailable()) {
        score += 2.0;
    }
    return score > 10.0 ? 10.0 : score;
}
bool Frame::isSuitableForArtwork(double artworkWeight) const {
    if (artworkWeight <= 0.0) {
        return false;
    }
    return artworkWeight <= 150.0 && isAvailable();
}
void Frame::adjustPosition(int position) {
    if (position >= -45 && position <= 45) {
    }
}
Lighting::Lighting(const char* n, bool l, bool d, int i)
    : GalleryTool(n, true, true, 100),
      led(l), dimmed(d), id(i) {}
void Lighting::dim() {
    dimmed = true;
}
void Lighting::brighten() {
    dimmed = false;
}
bool Lighting::isSafeForArtwork() const {
    return led && !dimmed && isAvailable();
}
bool Lighting::isLED() const {
    return led;
}
double Lighting::calculatePowerConsumption() const {
    if (led) {
        return dimmed ? 5.0 : 10.0;
    }
    return dimmed ? 20.0 : 40.0;
}
void Lighting::setBrightness(int level) {
    if (level >= 0 && level <= 100) {
        dimmed = (level < 50);
    }
}
bool Lighting::meetsSafetyStandards() const {
    return led && isAvailable();
}
SecurityCamera::SecurityCamera(const char* n, double a, bool r, bool act)
    : GalleryTool(n, true, true, 100),
      angle(a), recording(r), active(act) {}
void SecurityCamera::startRecording() {
    if (!isAvailable()) {
        throw ToolNotAvailableException("Camera not available");
    }
    useTool();
    recording = true;
    active = true;
}
void SecurityCamera::stopRecording() {
    recording = false;
    active = false;
}
bool SecurityCamera::isRecording() const {
    return recording;
}
double SecurityCamera::getAngle() const {
    return angle;
}
void SecurityCamera::adjustAngle(double newAngle) {
    if (newAngle >= 0.0 && newAngle <= 360.0) {
        angle = newAngle;
    }
}
double SecurityCamera::calculateCoverageArea(double distance) const {
    if (distance <= 0.0) {
        return 0.0;
    }
    double radius = distance * tan(angle * 3.14159 / 180.0 / 2.0);
    return 3.14159 * radius * radius;
}
bool SecurityCamera::canMonitorArea(double areaSize) const {
    if (areaSize <= 0.0 || !active) {
        return false;
    }
    double maxArea = calculateCoverageArea(50.0);
    return areaSize <= maxArea;
}
DisplayStand::DisplayStand(const char* n, double c, bool hc, bool u)
    : GalleryTool(n, true, true, 100),
      capacity(c), hasCover(hc), inUse(u) {}
void DisplayStand::startDisplay() {
    if (!isAvailable()) {
        throw ToolNotAvailableException("Display stand not available");
    }
    useTool();
    inUse = true;
}
void DisplayStand::stopDisplay() {
    inUse = false;
}
bool DisplayStand::canDisplay(double sqMeters) const {
    return sqMeters <= capacity;
}
bool DisplayStand::checkHasCover() const {
    return hasCover;
}
double DisplayStand::calculateDisplayTime(int visitorCount) const {
    if (visitorCount <= 0) {
        return 0.0;
    }
    return visitorCount * 0.5;
}
bool DisplayStand::canHandleVisitorLoad(int visitorCount) const {
    double requiredCapacity = visitorCount * 0.5;
    return capacity >= requiredCapacity;
}
double DisplayStand::calculateUtilization() const {
    if (capacity <= 0.0) {
        return 0.0;
    }
    return inUse ? 75.0 : 25.0;
}
Timer::Timer(int s, bool r, int e, int i)
    : seconds(s), running(r), elapsed(e), id(i) {}
void Timer::start(int s) {
    if (s <= 0) {
        throw TimerNotSetException("Timer seconds must be > 0");
    }
    seconds = s;
    elapsed = 0;
    running = true;
}
void Timer::tick(int delta) {
    if (delta <= 0) return;
    if (!running) return;
    elapsed += delta;
    if (elapsed >= seconds) {
        elapsed = seconds;
        running = false;
    }
}
bool Timer::isFinished() const {
    return !running && elapsed >= seconds && seconds > 0;
}
int Timer::getElapsed() const {
    return elapsed;
}
bool Timer::isRunning() const {
    return running;
}
int Timer::calculateRemainingTime() const {
    if (seconds <= 0) {
        return 0;
    }
    return seconds - elapsed;
}
void Timer::reset() {
    elapsed = 0;
    running = false;
}
void Timer::pause() {
    running = false;
}
void Timer::resume() {
    if (seconds > 0 && elapsed < seconds) {
        running = true;
    }
}
RestorationKit::RestorationKit(const char* n, bool eq)
    : GalleryTool(n, true, true, 100),
      equipped(eq) {}
bool RestorationKit::equip() {
    equipped = true;
    cout << "Restoration kit equipped.\n";
    return equipped;
}
bool RestorationKit::unequip() {
    equipped = false;
    cout << "Restoration kit unequipped.\n";
    return equipped;
}
void RestorationKit::restore() {
    if (!equipped) {
        cout << "Restoration kit is not equipped. Cannot start restoration.\n";
        return;
    }
    if (!isAvailable()) {
        throw ToolNotAvailableException("Restoration kit is not available");
    }
    useTool();
    cout << "Starting restoration process...\n";
    cout << "Cleaning and repairing artwork...\n";
    cout << "Restoration process completed.\n";
}
bool RestorationKit::isEquipped() const {
    return equipped;
}
double RestorationKit::calculateRestorationEfficiency() const {
    if (!isEquipped()) {
        return 0.0;
    }
    return isAvailable() ? 85.0 : 50.0;
}
bool RestorationKit::hasRequiredTools(int toolCount) const {
    return isEquipped() && toolCount > 0 && toolCount <= 10;
}
CleaningKit::CleaningKit(const char* n, int i)
    : GalleryTool(n, true, true, 100),
      id(i) {}
void CleaningKit::clean() {
    if (!isAvailable()) {
        throw ToolNotAvailableException("Cleaning kit is not available");
    }
    useTool();
    cout << "Using cleaning kit... clean-clean-clean...\n";
}
int CleaningKit::getId() const {
    return id;
}
ConditionProfile::ConditionProfile(double s, double t, int d, bool g)
    : startTemp(s), targetTemp(t), duration(d), gradual(g) {}
double ConditionProfile::currentTemp(int elapsed) const {
    if (!gradual || elapsed >= duration || duration <= 0) {
        return targetTemp;
    }
    double ratio = (double)elapsed / duration;
    return startTemp + (targetTemp - startTemp) * ratio;
}
bool ConditionProfile::isReached(double current) const {
    return current >= targetTemp;
}
void ConditionProfile::reset(double s, double t, int d) {
    startTemp = s;
    targetTemp = t;
    duration = d;
}
int ConditionProfile::getDuration() const {
    return duration;
}
double ConditionProfile::calculateTemperatureChangeRate() const {
    if (duration <= 0) {
        return 0.0;
    }
    return (targetTemp - startTemp) / duration;
}
bool ConditionProfile::isValidProfile() const {
    return duration > 0 && startTemp >= -50.0 && targetTemp <= 50.0;
}
int ConditionProfile::getEstimatedCompletionTime(int currentElapsed) const {
    if (currentElapsed < 0) {
        return duration;
    }
    int remaining = duration - currentElapsed;
    return remaining > 0 ? remaining : 0;
}
ClimateControl::ClimateControl(double t, bool o, bool d)
    : temperature(t),
      on(o),
      doorClosed(d),
      controlTimer(),
      profile(),
      elapsedSeconds(0),
      humidity(50.0),
      targetHumidity(50.0),
      alarmActive(false) {}
void ClimateControl::setConditions(double t, int warmupMinutes) {
    if (t <= 0.0 || t > 30.0) {
        throw InvalidConditionException("Invalid climate temperature");
    }
    if (!doorClosed) {
        throw InvalidConditionException("Climate control door is open");
    }
    if (warmupMinutes <= 0) {
        throw InvalidConditionException("Warmup minutes must be > 0");
    }
    profile.reset(temperature, t, warmupMinutes * 60);
    elapsedSeconds = 0;
    on = true;
    temperature = t;
}
void ClimateControl::turnOff() {
    on = false;
    temperature = 0.0;
}
void ClimateControl::closeDoor() {
    doorClosed = true;
}
void ClimateControl::openDoor() {
    doorClosed = false;
}
void ClimateControl::setTimerMinutes(int minutes) {
    if (minutes <= 0) {
        throw TimerNotSetException("Timer minutes must be > 0");
    }
    controlTimer.start(minutes * 60);
}
void ClimateControl::tick(int secondsDelta) {
    if (secondsDelta <= 0) return;
    if (on) {
        elapsedSeconds += secondsDelta;
        temperature = profile.currentTemp(elapsedSeconds);
    }
    controlTimer.tick(secondsDelta);
    if (controlTimer.isFinished() && on) {
        turnOff();
    }
}
bool ClimateControl::isOn() const {
    return on;
}
double ClimateControl::getTemperature() const {
    return temperature;
}
bool ClimateControl::isDoorClosed() const {
    return doorClosed;
}
void ClimateControl::setHumidity(double h) {
    humidity = h;
}
void ClimateControl::activateAlarm() {
    alarmActive = true;
}
bool ClimateControl::isAlarmActive() const {
    return alarmActive;
}
double ClimateControl::getHumidity() const {
    return humidity;
}
double ClimateControl::getTargetHumidity() const {
    return targetHumidity;
}
double ClimateControl::calculateEnergyConsumption() const {
    if (!on) {
        return 0.0;
    }
    return 2.5 * (temperature / 20.0);
}
bool ClimateControl::isOptimalClimate() const {
    return temperature >= 20.0 && temperature <= 24.0 && humidity >= 40.0 && humidity <= 60.0;
}
void ClimateControl::adjustTemperatureGradually(double targetTemp, int duration) {
    if (duration > 0 && targetTemp > 0.0 && targetTemp <= 30.0) {
        profile.reset(temperature, targetTemp, duration * 60);
        elapsedSeconds = 0;
        on = true;
    }
}
int ClimateControl::calculateMaintenanceSchedule() const {
    return 30;
}
SecuritySystem::SecuritySystem(int z, int act, bool a, bool o)
    : zones(z), activeZones(act), alarm(a), on(o) {}
void SecuritySystem::activateZone() {
    if (activeZones < zones) {
        activeZones++;
        on = true;
    }
}
void SecuritySystem::deactivateZone() {
    if (activeZones > 0) {
        activeZones--;
        if (activeZones == 0) {
            on = false;
        }
    }
}
int SecuritySystem::freeZones() const {
    return zones - activeZones;
}
bool SecuritySystem::isOn() const {
    return on;
}
void SecuritySystem::activateAllZones() {
    activeZones = zones;
}
void SecuritySystem::deactivateAllZones() {
    activeZones = 0;
}
bool SecuritySystem::checkSecurityBreach() const {
    return activeZones < zones && on;
}
double SecuritySystem::calculateSecurityCoverage() const {
    if (zones == 0) {
        return 0.0;
    }
    return (activeZones * 100.0) / zones;
}
Exhibition::Exhibition(const char* n) : name(n) {}
PaintingExhibition::PaintingExhibition(const char* n,
                                       Artwork* p1,
                                       Artwork* p2,
                                       Frame* f,
                                       Lighting* l,
                                       Timer* t,
                                       Curator* cu)
    : Exhibition(n),
      painting1(p1), painting2(p2),
      use_frame(f), use_lighting(l),
      use_displayTimer(t), curator(cu) {}
void PaintingExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for painting exhibition");
    }
    curator->organizePainting(this);
}
SculptureExhibition::SculptureExhibition(const char* n,
                                          Artwork* s,
                                          DisplayStand* st,
                                          Lighting* l,
                                          Frame* f,
                                          Curator* cu)
    : Exhibition(n), sculpture(s), stand(st), lighting(l), frame(f), curator(cu) {}
void SculptureExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for sculpture exhibition");
    }
    curator->organizeSculpture(this);
}
PhotographyExhibition::PhotographyExhibition(const char* n,
                                             Artwork* ph1,
                                             Artwork* ph2,
                                             Frame* fr,
                                             Lighting* l,
                                             SecurityCamera* cam,
                                             Timer* t,
                                             Curator* cu)
    : Exhibition(n), photo1(ph1), photo2(ph2),
      frame(fr), lighting(l), camera(cam), displayTimer(t), curator(cu) {}
void PhotographyExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for photography exhibition");
    }
    curator->organizePhotography(this);
}
MixedMediaExhibition::MixedMediaExhibition(const char* n,
                                           Artwork* a1,
                                           Artwork* a2,
                                           Artwork* a3,
                                           DisplayStand* st,
                                           Lighting* l,
                                           Frame* f,
                                           Timer* t,
                                           RestorationKit* rk,
                                           Curator* cu)
    : Exhibition(n), artwork1(a1), artwork2(a2), artwork3(a3),
      stand(st), lighting(l), frame(f), displayTimer(t),
      restorationKit(rk), curator(cu) {}
void MixedMediaExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for mixed media exhibition");
    }
    curator->organizeMixedMedia(this);
}
DigitalArtExhibition::DigitalArtExhibition(const char* n,
                                           Artwork* d1,
                                           Artwork* d2,
                                           DisplayStand* st,
                                           Lighting* l,
                                           SecurityCamera* cam,
                                           Timer* t,
                                           Curator* cu)
    : Exhibition(n), digital1(d1), digital2(d2),
      stand(st), lighting(l), camera(cam), displayTimer(t), curator(cu) {}
void DigitalArtExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for digital art exhibition");
    }
    curator->organizeDigitalArt(this);
}
VintageExhibition::VintageExhibition(const char* n,
                                      Artwork* v1,
                                      Artwork* v2,
                                      Frame* f,
                                      ClimateControl* cc,
                                      RestorationKit* rk,
                                      Curator* cu)
    : Exhibition(n), vintage1(v1), vintage2(v2),
      frame(f), climate(cc), restorationKit(rk), curator(cu) {}
void VintageExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for vintage exhibition");
    }
    curator->organizeVintage(this);
}
ContemporaryExhibition::ContemporaryExhibition(const char* n,
                                                Artwork* c1,
                                                Artwork* c2,
                                                DisplayStand* st,
                                                Lighting* l,
                                                SecurityCamera* cam,
                                                Timer* t,
                                                Curator* cu)
    : Exhibition(n), contemporary1(c1), contemporary2(c2),
      stand(st), lighting(l), camera(cam), displayTimer(t), curator(cu) {}
void ContemporaryExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for contemporary exhibition");
    }
    curator->organizeContemporary(this);
}
MinimalistExhibition::MinimalistExhibition(const char* n,
                                            Artwork* m,
                                            Frame* f,
                                            Lighting* l,
                                            Curator* cu)
    : Exhibition(n), minimalist(m), frame(f), lighting(l), curator(cu) {}
void MinimalistExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for minimalist exhibition");
    }
    curator->organizeMinimalist(this);
}
AbstractExhibition::AbstractExhibition(const char* n,
                                       Artwork* a1,
                                       Artwork* a2,
                                       DisplayStand* st,
                                       Lighting* l,
                                       Frame* f,
                                       Timer* t,
                                       Curator* cu)
    : Exhibition(n), abstract1(a1), abstract2(a2),
      stand(st), lighting(l), frame(f), displayTimer(t), curator(cu) {}
void AbstractExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for abstract exhibition");
    }
    curator->organizeAbstract(this);
}
PortraitExhibition::PortraitExhibition(const char* n,
                                       Artwork* p1,
                                       Artwork* p2,
                                       Frame* f,
                                       Lighting* l,
                                       SecurityCamera* cam,
                                       Curator* cu)
    : Exhibition(n), portrait1(p1), portrait2(p2),
      frame(f), lighting(l), camera(cam), curator(cu) {}
void PortraitExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for portrait exhibition");
    }
    curator->organizePortrait(this);
}
LandscapeExhibition::LandscapeExhibition(const char* n,
                                         Artwork* l1,
                                         Artwork* l2,
                                         Frame* f,
                                         Lighting* l,
                                         Timer* t,
                                         Curator* cu)
    : Exhibition(n), landscape1(l1), landscape2(l2),
      frame(f), lighting(l), displayTimer(t), curator(cu) {}
void LandscapeExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for landscape exhibition");
    }
    curator->organizeLandscape(this);
}
RestorationExhibition::RestorationExhibition(const char* n,
                                              Artwork* a,
                                              RestorationKit* rk,
                                              CleaningKit* ck,
                                              ClimateControl* cc,
                                              Timer* t,
                                              Curator* cu)
    : Exhibition(n), artwork(a),
      restorationKit(rk), cleaningKit(ck), climate(cc),
      restorationTimer(t), curator(cu) {}
void RestorationExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for restoration exhibition");
    }
    curator->organizeRestoration(this);
}
TemporaryExhibition::TemporaryExhibition(const char* n,
                                          Artwork* t1,
                                          Artwork* t2,
                                          DisplayStand* st,
                                          Lighting* l,
                                          SecurityCamera* cam,
                                          Timer* t,
                                          Curator* cu)
    : Exhibition(n), temp1(t1), temp2(t2),
      stand(st), lighting(l), camera(cam), displayTimer(t), curator(cu) {}
void TemporaryExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for temporary exhibition");
    }
    curator->organizeTemporary(this);
}
PermanentExhibition::PermanentExhibition(const char* n,
                                          Artwork* p1,
                                          Artwork* p2,
                                          Frame* f,
                                          ClimateControl* cc,
                                          SecuritySystem* sec,
                                          Timer* t,
                                          Curator* cu)
    : Exhibition(n), permanent1(p1), permanent2(p2),
      frame(f), climate(cc), security(sec), displayTimer(t), curator(cu) {}
void PermanentExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for permanent exhibition");
    }
    curator->organizePermanent(this);
}
InteractiveExhibition::InteractiveExhibition(const char* n,
                                             Artwork* i1,
                                             Artwork* i2,
                                             DisplayStand* st,
                                             Lighting* l,
                                             SecurityCamera* cam,
                                             Timer* t,
                                             Curator* cu)
    : Exhibition(n), interactive1(i1), interactive2(i2),
      stand(st), lighting(l), camera(cam), displayTimer(t), curator(cu) {}
void InteractiveExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for interactive exhibition");
    }
    curator->organizeInteractive(this);
}
ThematicExhibition::ThematicExhibition(const char* n,
                                       Artwork* th1,
                                       Artwork* th2,
                                       Artwork* th3,
                                       Frame* f,
                                       DisplayStand* st,
                                       Lighting* l,
                                       Timer* t,
                                       Curator* cu)
    : Exhibition(n), theme1(th1), theme2(th2), theme3(th3),
      frame(f), stand(st), lighting(l), displayTimer(t), curator(cu) {}
void ThematicExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for thematic exhibition");
    }
    curator->organizeThematic(this);
}
RetrospectiveExhibition::RetrospectiveExhibition(const char* n,
                                                   Artwork* r1,
                                                   Artwork* r2,
                                                   Frame* f,
                                                   ClimateControl* cc,
                                                   RestorationKit* rk,
                                                   Timer* t,
                                                   Curator* cu)
    : Exhibition(n), retro1(r1), retro2(r2),
      frame(f), climate(cc), restorationKit(rk), displayTimer(t), curator(cu) {}
void RetrospectiveExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for retrospective exhibition");
    }
    curator->organizeRetrospective(this);
}
GroupExhibition::GroupExhibition(const char* n,
                                 Artwork* g1,
                                 Artwork* g2,
                                 Artwork* g3,
                                 DisplayStand* st,
                                 Lighting* l,
                                 Frame* f,
                                 SecurityCamera* cam,
                                 Timer* t,
                                 Curator* cu)
    : Exhibition(n), group1(g1), group2(g2), group3(g3),
      stand(st), lighting(l), frame(f), camera(cam), displayTimer(t), curator(cu) {}
void GroupExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for group exhibition");
    }
    curator->organizeGroup(this);
}
SoloExhibition::SoloExhibition(const char* n,
                                Artwork* s1,
                                Artwork* s2,
                                Frame* f,
                                Lighting* l,
                                DisplayStand* st,
                                Timer* t,
                                Curator* cu)
    : Exhibition(n), solo1(s1), solo2(s2),
      frame(f), lighting(l), stand(st), displayTimer(t), curator(cu) {}
void SoloExhibition::organize() {
    if (!curator) {
        throw ToolNotAvailableException("No curator for solo exhibition");
    }
    curator->organizeSolo(this);
}
Curator::Curator(const char* n)
    : name(n) {}
void Curator::organizePainting(PaintingExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->use_frame) throw ToolNotAvailableException("No frame for painting exhibition");
    if (!exhibition->use_lighting) throw ToolNotAvailableException("No lighting for painting exhibition");
    if (!exhibition->use_displayTimer) throw TimerNotSetException("No timer for painting exhibition");
    cout << "Preparing paintings for display...\n";
    exhibition->painting1->useSize(50.0);
    exhibition->painting2->useSize(50.0);
    cout << "Mounting paintings in frames...\n";
    exhibition->use_frame->useTool();
    cout << "Setting up lighting...\n";
    exhibition->use_lighting->brighten();
    const int totalSeconds = 60 * 60;
    exhibition->use_displayTimer->start(totalSeconds);
    int step = 10 * 60;
    int minutesPassed = 0;
    while (!exhibition->use_displayTimer->isFinished()) {
        exhibition->use_displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Painting exhibition organized successfully!\n";
}
void Curator::organizeSculpture(SculptureExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->stand || !exhibition->stand->canDisplay(1.5)) {
        throw InsufficientSpaceException("Display stand too small for sculpture");
    }
    cout << "Preparing sculpture for display...\n";
    exhibition->sculpture->useSize(100.0);
    cout << "Placing sculpture on display stand...\n";
    exhibition->stand->startDisplay();
    cout << "Setting up lighting for sculpture...\n";
    exhibition->lighting->brighten();
    cout << "Sculpture exhibition organized successfully!\n";
}
void Curator::organizePhotography(PhotographyExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->frame || !exhibition->lighting) {
        throw ToolNotAvailableException("No frame or lighting for photography exhibition");
    }
    if (!exhibition->camera) {
        throw ToolNotAvailableException("No security camera for photography exhibition");
    }
    if (!exhibition->displayTimer) {
        throw TimerNotSetException("No timer for photography exhibition");
    }
    cout << "Preparing photographs for display...\n";
    exhibition->photo1->useSize(30.0);
    exhibition->photo2->useSize(30.0);
    cout << "Mounting photographs in frames...\n";
    exhibition->frame->useTool();
    cout << "Setting up security camera...\n";
    exhibition->camera->startRecording();
    cout << "Setting up lighting...\n";
    exhibition->lighting->brighten();
    const int totalSeconds = 45 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 5 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Photography exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Photography exhibition organized successfully!\n";
}
void Curator::organizeMixedMedia(MixedMediaExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->artwork1 || !exhibition->artwork2 || !exhibition->artwork3) {
        throw ArtworkNotFoundException("Missing artworks for mixed media exhibition");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand for mixed media");
    if (!exhibition->restorationKit) throw ToolNotAvailableException("No restoration kit for mixed media");
    cout << "Preparing mixed media artworks...\n";
    exhibition->artwork1->useSize(40.0);
    exhibition->artwork2->useSize(40.0);
    exhibition->artwork3->useSize(40.0);
    cout << "Equipping restoration kit...\n";
    exhibition->restorationKit->equip();
    cout << "Placing artworks on display stand...\n";
    exhibition->stand->startDisplay();
    cout << "Setting up lighting...\n";
    exhibition->lighting->brighten();
    const int totalSeconds = 90 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 10 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Mixed media exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Mixed media exhibition organized successfully!\n";
}
void Curator::organizeDigitalArt(DigitalArtExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->digital1 || !exhibition->digital2) {
        throw ArtworkNotFoundException("Missing digital artworks");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand for digital art");
    if (!exhibition->camera) throw ToolNotAvailableException("No security camera for digital art");
    cout << "Preparing digital artworks...\n";
    exhibition->digital1->useSize(60.0);
    exhibition->digital2->useSize(60.0);
    cout << "Setting up digital displays...\n";
    exhibition->stand->startDisplay();
    cout << "Activating security camera...\n";
    exhibition->camera->startRecording();
    cout << "Setting up LED lighting...\n";
    exhibition->lighting->brighten();
    const int totalSeconds = 120 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 15 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Digital art exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Digital art exhibition organized successfully!\n";
}
void Curator::organizeVintage(VintageExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->climate) {
        throw ToolNotAvailableException("No climate control for vintage exhibition");
    }
    cout << "Preparing vintage artworks...\n";
    exhibition->vintage1->useSize(80.0);
    exhibition->vintage2->useSize(80.0);
    exhibition->climate->closeDoor();
    exhibition->climate->setConditions(18.0);
    exhibition->climate->setTimerMinutes(240);
    cout << "Maintaining optimal climate conditions for vintage artworks...\n";
    int controlledSeconds = 0;
    while (true) {
        exhibition->climate->tick(600);
        controlledSeconds += 600;
        cout << "Climate controlled for " << controlledSeconds / 60 << " minutes...\n";
        if (!exhibition->climate->isOn()) break;
    }
    int controlledMinutes = controlledSeconds / 60;
    int expectedMinutes = 240;
    if (controlledMinutes > expectedMinutes + 10) {
        throw OverexposedArtworkException("Vintage artworks overexposed to conditions");
    }
    if (controlledMinutes < expectedMinutes - 10) {
        throw UnderexposedArtworkException("Vintage artworks underexposed");
    }
    cout << "Vintage exhibition organized successfully!\n";
}
void Curator::organizeContemporary(ContemporaryExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->contemporary1 || !exhibition->contemporary2) {
        throw ArtworkNotFoundException("Missing contemporary artworks");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand");
    if (!exhibition->camera) throw ToolNotAvailableException("No security camera");
    cout << "Preparing contemporary artworks...\n";
    exhibition->contemporary1->useSize(70.0);
    exhibition->contemporary2->useSize(70.0);
    cout << "Setting up contemporary display...\n";
    exhibition->stand->startDisplay();
    exhibition->camera->startRecording();
    exhibition->lighting->brighten();
    const int totalSeconds = 180 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 20 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Contemporary exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Contemporary exhibition organized successfully!\n";
}
void Curator::organizeMinimalist(MinimalistExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->frame || !exhibition->frame->canProtect()) {
        throw ToolNotAvailableException("Frame not available for minimalist exhibition");
    }
    if (!exhibition->lighting) {
        throw ToolNotAvailableException("No lighting for minimalist exhibition");
    }
    cout << "Preparing minimalist artwork...\n";
    exhibition->minimalist->useSize(50.0);
    cout << "Mounting in protective frame...\n";
    exhibition->frame->addProtection();
    cout << "Setting up subtle lighting...\n";
    exhibition->lighting->dim();
    cout << "Minimalist exhibition organized successfully!\n";
}
void Curator::organizeAbstract(AbstractExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->abstract1 || !exhibition->abstract2) {
        throw ArtworkNotFoundException("Missing abstract artworks");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand");
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing abstract artworks...\n";
    exhibition->abstract1->useSize(60.0);
    exhibition->abstract2->useSize(60.0);
    exhibition->stand->startDisplay();
    exhibition->lighting->brighten();
    const int totalSeconds = 150 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 15 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Abstract exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Abstract exhibition organized successfully!\n";
}
void Curator::organizePortrait(PortraitExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->portrait1 || !exhibition->portrait2) {
        throw ArtworkNotFoundException("Missing portrait artworks");
    }
    if (!exhibition->frame) throw ToolNotAvailableException("No frame");
    if (!exhibition->camera) throw ToolNotAvailableException("No security camera");
    cout << "Preparing portraits...\n";
    exhibition->portrait1->useSize(55.0);
    exhibition->portrait2->useSize(55.0);
    exhibition->frame->useTool();
    exhibition->lighting->brighten();
    exhibition->camera->startRecording();
    cout << "Portrait exhibition organized successfully!\n";
}
void Curator::organizeLandscape(LandscapeExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->landscape1 || !exhibition->landscape2) {
        throw ArtworkNotFoundException("Missing landscape artworks");
    }
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing landscapes...\n";
    exhibition->landscape1->useSize(75.0);
    exhibition->landscape2->useSize(75.0);
    exhibition->frame->useTool();
    exhibition->lighting->brighten();
    const int totalSeconds = 200 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 20 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Landscape exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Landscape exhibition organized successfully!\n";
}
void Curator::organizeRestoration(RestorationExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->artwork) throw ArtworkNotFoundException("No artwork for restoration");
    if (!exhibition->restorationKit) throw ToolNotAvailableException("No restoration kit");
    if (!exhibition->cleaningKit) throw ToolNotAvailableException("No cleaning kit");
    if (!exhibition->restorationTimer) throw TimerNotSetException("No timer");
    cout << "Preparing artwork for restoration...\n";
    exhibition->artwork->useSize(100.0);
    cout << "Setting up climate control...\n";
    exhibition->climate->setConditions(20.0);
    cout << "Equipping restoration kit...\n";
    exhibition->restorationKit->equip();
    cout << "Starting restoration process...\n";
    exhibition->restorationKit->restore();
    cout << "Cleaning artwork...\n";
    exhibition->cleaningKit->clean();
    const int totalSeconds = 120 * 60;
    exhibition->restorationTimer->start(totalSeconds);
    int step = 10 * 60;
    int minutesPassed = 0;
    while (!exhibition->restorationTimer->isFinished()) {
        exhibition->restorationTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Restoration process running for " << minutesPassed << " minutes...\n";
    }
    cout << "Restoration exhibition organized successfully!\n";
}
void Curator::organizeTemporary(TemporaryExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->temp1 || !exhibition->temp2) {
        throw ArtworkNotFoundException("Missing temporary artworks");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand");
    if (!exhibition->camera) throw ToolNotAvailableException("No security camera");
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing temporary artworks...\n";
    exhibition->temp1->useSize(45.0);
    exhibition->temp2->useSize(45.0);
    exhibition->stand->startDisplay();
    exhibition->lighting->brighten();
    exhibition->camera->startRecording();
    const int totalSeconds = 60 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 5 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Temporary exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Temporary exhibition organized successfully!\n";
}
void Curator::organizePermanent(PermanentExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->permanent1 || !exhibition->permanent2) {
        throw ArtworkNotFoundException("Missing permanent artworks");
    }
    if (!exhibition->climate) throw ToolNotAvailableException("No climate control");
    if (!exhibition->security) throw ToolNotAvailableException("No security system");
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing permanent collection...\n";
    exhibition->permanent1->useSize(90.0);
    exhibition->permanent2->useSize(90.0);
    cout << "Setting up climate control...\n";
    exhibition->climate->setConditions(20.0);
    cout << "Activating security system...\n";
    exhibition->security->activateZone();
    exhibition->security->activateZone();
    exhibition->frame->useTool();
    const int totalSeconds = 300 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 30 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Permanent exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Permanent exhibition organized successfully!\n";
}
void Curator::organizeInteractive(InteractiveExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->interactive1 || !exhibition->interactive2) {
        throw ArtworkNotFoundException("Missing interactive artworks");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand");
    if (!exhibition->camera) throw ToolNotAvailableException("No security camera");
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing interactive artworks...\n";
    exhibition->interactive1->useSize(65.0);
    exhibition->interactive2->useSize(65.0);
    exhibition->stand->startDisplay();
    exhibition->lighting->brighten();
    exhibition->camera->startRecording();
    const int totalSeconds = 240 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 20 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Interactive exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Interactive exhibition organized successfully!\n";
}
void Curator::organizeThematic(ThematicExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->theme1 || !exhibition->theme2 || !exhibition->theme3) {
        throw ArtworkNotFoundException("Missing thematic artworks");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand");
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing thematic artworks...\n";
    exhibition->theme1->useSize(50.0);
    exhibition->theme2->useSize(50.0);
    exhibition->theme3->useSize(50.0);
    exhibition->stand->startDisplay();
    exhibition->frame->useTool();
    exhibition->lighting->brighten();
    const int totalSeconds = 180 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 15 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Thematic exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Thematic exhibition organized successfully!\n";
}
void Curator::organizeRetrospective(RetrospectiveExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->retro1 || !exhibition->retro2) {
        throw ArtworkNotFoundException("Missing retrospective artworks");
    }
    if (!exhibition->climate) throw ToolNotAvailableException("No climate control");
    if (!exhibition->restorationKit) throw ToolNotAvailableException("No restoration kit");
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing retrospective artworks...\n";
    exhibition->retro1->useSize(85.0);
    exhibition->retro2->useSize(85.0);
    cout << "Setting up climate control...\n";
    exhibition->climate->setConditions(19.0);
    cout << "Equipping restoration kit...\n";
    exhibition->restorationKit->equip();
    exhibition->frame->useTool();
    const int totalSeconds = 360 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 30 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Retrospective exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Retrospective exhibition organized successfully!\n";
}
void Curator::organizeGroup(GroupExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->group1 || !exhibition->group2 || !exhibition->group3) {
        throw ArtworkNotFoundException("Missing group artworks");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand");
    if (!exhibition->camera) throw ToolNotAvailableException("No security camera");
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing group artworks...\n";
    exhibition->group1->useSize(55.0);
    exhibition->group2->useSize(55.0);
    exhibition->group3->useSize(55.0);
    exhibition->stand->startDisplay();
    exhibition->frame->useTool();
    exhibition->lighting->brighten();
    exhibition->camera->startRecording();
    const int totalSeconds = 210 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 15 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Group exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Group exhibition organized successfully!\n";
}
void Curator::organizeSolo(SoloExhibition* exhibition) {
    cout << "\n=== Organizing exhibition: " << exhibition->getName() << " ===\n";
    if (!exhibition->solo1 || !exhibition->solo2) {
        throw ArtworkNotFoundException("Missing solo artworks");
    }
    if (!exhibition->stand) throw ToolNotAvailableException("No display stand");
    if (!exhibition->displayTimer) throw TimerNotSetException("No timer");
    cout << "Preparing solo artworks...\n";
    exhibition->solo1->useSize(70.0);
    exhibition->solo2->useSize(70.0);
    exhibition->stand->startDisplay();
    exhibition->frame->useTool();
    exhibition->lighting->brighten();
    const int totalSeconds = 270 * 60;
    exhibition->displayTimer->start(totalSeconds);
    int step = 25 * 60;
    int minutesPassed = 0;
    while (!exhibition->displayTimer->isFinished()) {
        exhibition->displayTimer->tick(step);
        minutesPassed += step / 60;
        cout << "Solo exhibition running for " << minutesPassed << " minutes...\n";
    }
    cout << "Solo exhibition organized successfully!\n";
}
Visitor::Visitor(const char* n, int al, bool ht)
    : name(n), accessLevel(al), hasTicket(ht), visitDuration(0.0), favoriteCount(0), guidedTour(false) {}
void Visitor::purchaseTicket() {
    hasTicket = true;
    cout << "Visitor " << name << " purchased ticket.\n";
}
void Visitor::enterGallery() {
    if (!hasTicket) {
        throw InvalidAccessException("Visitor needs ticket to enter");
    }
    cout << "Visitor " << name << " entered gallery.\n";
}
void Visitor::viewArtwork(Artwork* artwork) {
    if (!artwork) {
        throw ArtworkNotFoundException("Artwork not found");
    }
    cout << "Visitor " << name << " viewing " << artwork->getArtist() << " artwork.\n";
    visitDuration += 5.0;
}
void Visitor::addFavorite() {
    favoriteCount++;
    cout << "Visitor " << name << " added favorite. Total: " << favoriteCount << "\n";
}
void Visitor::joinGuidedTour() {
    guidedTour = true;
    cout << "Visitor " << name << " joined guided tour.\n";
}
bool Visitor::canAccessRestricted() const {
    return accessLevel >= 3 && hasTicket;
}
int Visitor::getAccessLevel() const {
    return accessLevel;
}
const char* Visitor::getName() const {
    return name;
}
double Visitor::getVisitDuration() const {
    return visitDuration;
}
double Visitor::calculateTotalBaggageWeight(double checkedWeight, double carryOnWeight) const {
    return checkedWeight + carryOnWeight;
}
bool Visitor::hasValidVisa(const char* destinationCountry) const {
    if (!destinationCountry) {
        return false;
    }
    return accessLevel >= 2;
}
double Visitor::calculateLoyaltyDiscount(double basePrice) const {
    if (favoriteCount > 10) {
        return basePrice * 0.15;
    } else if (favoriteCount > 5) {
        return basePrice * 0.10;
    }
    return 0.0;
}
bool Visitor::requestSpecialMeal(const char* mealType) {
    if (!mealType) {
        return false;
    }
    return hasTicket;
}
bool Visitor::checkInForExhibition(const char* exhibitionNumber) {
    if (!exhibitionNumber || !hasTicket) {
        return false;
    }
    return true;
}
bool Visitor::cancelReservation(int reservationId) {
    if (reservationId < 0) {
        return false;
    }
    return hasTicket;
}
double Visitor::calculateTotalTravelCost(double ticketPrice, double baggageFee, double mealFee) const {
    return ticketPrice + baggageFee + mealFee;
}
GalleryMenu::GalleryMenu() : exhibitionCount(0), currentVisitor(nullptr) {
    for (int i = 0; i < MAX_EXHIBITIONS; i++) {
        exhibitions[i] = nullptr;
    }
}
void GalleryMenu::show() const {
    cout << "\n==== GALLERY MENU ====\n";
    for (int i = 0; i < exhibitionCount; i++) {
        cout << (i + 1) << ") " << exhibitions[i]->getName() << "\n";
    }
    cout << "0) Exit\n";
}
void GalleryMenu::addExhibition(Exhibition* e) {
    if (e && exhibitionCount < MAX_EXHIBITIONS) {
        exhibitions[exhibitionCount] = e;
        exhibitionCount++;
    }
}
void GalleryMenu::setVisitor(Visitor* v) {
    currentVisitor = v;
}
void GalleryMenu::run() {
    while (true) {
        show();
        cout << "Select exhibition number: ";
        int choice = 0;
        if (!(cin >> choice)) {
            cout << "Invalid input, exiting.\n";
            return;
        }
        if (choice == 0) {
            cout << "Exiting gallery menu.\n";
            return;
        }
        if (choice < 1 || choice > exhibitionCount) {
            cout << "No such option.\n";
            continue;
        }
        exhibitions[choice - 1]->organize();
        cout << "Exhibition \"" << exhibitions[choice - 1]->getName()
             << "\" successfully organized.\n";
    }
}
int GalleryMenu::getExhibitionCount() const {
    return exhibitionCount;
}
Visitor* GalleryMenu::getCurrentVisitor() const {
    return currentVisitor;
}
int GalleryMenu::searchExhibitions(const char* searchTerm) const {
    int count = 0;
    for (int i = 0; i < exhibitionCount; i++) {
        if (exhibitions[i] && strstr(exhibitions[i]->getName(), searchTerm)) {
            count++;
        }
    }
    return count;
}
int GalleryMenu::filterAvailableExhibitions() const {
    int count = 0;
    for (int i = 0; i < exhibitionCount; i++) {
        if (exhibitions[i] && !exhibitions[i]->isFullyBooked()) {
            count++;
        }
    }
    return count;
}
void GalleryMenu::sortExhibitionsByName() {
    for (int i = 0; i < exhibitionCount - 1; i++) {
        for (int j = i + 1; j < exhibitionCount; j++) {
            if (exhibitions[i] && exhibitions[j] && strcmp(exhibitions[i]->getName(), exhibitions[j]->getName()) > 0) {
                Exhibition* temp = exhibitions[i];
                exhibitions[i] = exhibitions[j];
                exhibitions[j] = temp;
            }
        }
    }
}
Exhibition* GalleryMenu::getExhibitionByIndex(int index) const {
    if (index >= 0 && index < exhibitionCount) {
        return exhibitions[index];
    }
    return nullptr;
}
bool Exhibition::isFullyBooked() const {
    return false;
}
double Exhibition::calculateExhibitionDuration() const {
    return 2.5;
}
bool Exhibition::isOnTime() const {
    return true;
}
int Exhibition::getAvailableSeats() const {
    return 150;
}
TicketSystem::TicketSystem(const char* name, int max)
    : systemName(name), totalBookings(0), totalRevenue(0.0), isActive(true), maxBookings(max),
      currentBookingVisitor(nullptr), currentBookingExhibition(nullptr) {}
bool TicketSystem::createBooking(Visitor* visitor, Exhibition* exhibition) {
    if (!isActive || totalBookings >= maxBookings || !visitor || !exhibition) {
        return false;
    }
    if (exhibition->isFullyBooked()) {
        return false;
    }
    currentBookingVisitor = visitor;
    currentBookingExhibition = exhibition;
    totalBookings++;
    return true;
}
bool TicketSystem::cancelBooking(int bookingId) {
    if (bookingId < 0 || bookingId >= totalBookings) {
        return false;
    }
    totalBookings--;
    return true;
}
double TicketSystem::calculateTicketPrice(Exhibition* exhibition, Visitor* visitor) {
    if (!exhibition || !visitor) {
        return 0.0;
    }
    double basePrice = 500.0;
    double discount = visitor->calculateLoyaltyDiscount(basePrice);
    return basePrice - discount;
}
bool TicketSystem::processPayment(double amount, const char* cardNumber) {
    if (amount <= 0.0 || !cardNumber || strlen(cardNumber) < 13) {
        return false;
    }
    totalRevenue += amount;
    return true;
}
bool TicketSystem::transferMoney(const char* fromCard, const char* toCard, double amount) {
    if (!fromCard || !toCard || amount <= 0.0) {
        return false;
    }
    if (strlen(fromCard) < 13 || strlen(toCard) < 13) {
        return false;
    }
    return true;
}
bool TicketSystem::verifyPassword(const char* accountId, const char* password) {
    if (!accountId || !password) {
        return false;
    }
    return strlen(password) >= 6;
}
bool TicketSystem::checkExhibitionAvailability(Exhibition* exhibition) {
    if (!exhibition) {
        return false;
    }
    return !exhibition->isFullyBooked();
}
bool TicketSystem::reserveSeat(Visitor* visitor, int seatNumber) {
    if (!visitor || seatNumber < 1 || seatNumber > 500) {
        return false;
    }
    return true;
}
bool TicketSystem::cancelSeatReservation(int seatNumber) {
    if (seatNumber < 1 || seatNumber > 500) {
        return false;
    }
    return true;
}
double TicketSystem::calculateBaggageFee(double weight) {
    if (weight <= 0.0) {
        return 0.0;
    }
    if (weight <= 23.0) {
        return 0.0;
    }
    double excess = weight - 23.0;
    return excess * 10.0;
}
bool TicketSystem::checkVisaRequirement(Visitor* visitor, Exhibition* exhibition) {
    if (!visitor || !exhibition) {
        return false;
    }
    return visitor->hasValidVisa("International");
}
double TicketSystem::getTotalRevenue() const {
    return totalRevenue;
}
int TicketSystem::getTotalBookings() const {
    return totalBookings;
}
bool TicketSystem::isSystemActive() const {
    return isActive;
}