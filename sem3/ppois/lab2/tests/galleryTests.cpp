#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include "../gallery.hpp"

using namespace std;

static Dimension cmUnit("cm", 1.0, true, 1);

static Artwork makeArtwork(const char* name, double cm, bool fragile = false) {
    Size s(cm, &cmUnit, false, 1);
    return Artwork(name, s, 0.0, fragile);
}

TEST(Dimension, DefaultCtor) {
    Dimension d;
    EXPECT_NEAR(10.0, d.toCentimeters(10.0), 1e-9);
    EXPECT_TRUE(d.isMetric());
    EXPECT_EQ(0, d.getId());
}

TEST(Dimension, CustomCtor) {
    Dimension inch("in", 2.54, false, 7);
    EXPECT_NEAR(25.4, inch.toCentimeters(10.0), 1e-9);
    EXPECT_FALSE(inch.isMetric());
    EXPECT_EQ(7, inch.getId());
}

TEST(Dimension, IsMetricFlag) {
    Dimension metric("cm", 1.0, true, 2);
    Dimension imperial("in", 2.54, false, 3);
    EXPECT_TRUE(metric.isMetric());
    EXPECT_FALSE(imperial.isMetric());
}

TEST(Size, ToCentimeters_WithDimension) {
    Size s(200.0, &cmUnit);
    EXPECT_NEAR(200.0, s.toCentimeters(), 1e-9);
}

TEST(Size, ToCentimeters_NoDimension_Throws) {
    Size s(100.0, nullptr);
    EXPECT_THROW(s.toCentimeters(), StorageException);
}

TEST(Size, Scale_Up) {
    Size s(100.0, &cmUnit);
    s.scale(2.0);
    EXPECT_NEAR(200.0, s.toCentimeters(), 1e-9);
}

TEST(Size, Scale_DownToZero_IsZero) {
    Size s(100.0, &cmUnit);
    s.scale(0.0);
    EXPECT_TRUE(s.isZero());
}

TEST(Artwork, Fragile_True) {
    Size s(100.0, &cmUnit);
    Artwork painting("painting", s, 200.0, true);
    EXPECT_TRUE(painting.isFragile());
}

TEST(Artwork, Fragile_False) {
    Size s(100.0, &cmUnit);
    Artwork sculpture("sculpture", s, 0.0, false);
    EXPECT_FALSE(sculpture.isFragile());
}

TEST(Artwork, AddSize_Increase) {
    Size s(100.0, &cmUnit);
    Artwork a("artwork", s, 0.0, false);
    a.addSize(50.0);
    EXPECT_FALSE(a.isFragile());
}

TEST(Artwork, AddSize_FromZero_NoCrash) {
    Size s(0.0, &cmUnit);
    Artwork a("artwork", s, 0.0, false);
    a.addSize(100.0);
}

TEST(Artwork, UseSize_Enough) {
    Size s(200.0, &cmUnit);
    Artwork a("artwork", s, 0.0, false);
    a.useSize(100.0);
    EXPECT_FALSE(a.isFragile());
}

TEST(Artwork, UseSize_NotEnough_Throws) {
    Size s(50.0, &cmUnit);
    Artwork a("artwork", s, 0.0, false);
    EXPECT_THROW(a.useSize(100.0), NotEnoughArtworkException);
}

TEST(GalleryTool, DefaultAvailable) {
    GalleryTool t;
    EXPECT_TRUE(t.isAvailable());
}

TEST(GalleryTool, UseTool_DecreasesDurability) {
    GalleryTool t("tool", true, true, 2);
    EXPECT_TRUE(t.isAvailable());
    t.useTool();
    EXPECT_TRUE(t.isAvailable());
    t.useTool();
    EXPECT_FALSE(t.isAvailable());
}

TEST(GalleryTool, UseTool_Dirty_Throws) {
    GalleryTool t("tool", false, true, 10);
    EXPECT_THROW(t.useTool(), ToolNotAvailableException);
}

TEST(GalleryTool, UseTool_NotAvailable_Throws) {
    GalleryTool t("tool", true, false, 10);
    EXPECT_THROW(t.useTool(), ToolNotAvailableException);
}

TEST(GalleryTool, CleanTool_MakesAvailable) {
    GalleryTool t("tool", false, true, 10);
    EXPECT_FALSE(t.isAvailable());
    t.cleanTool();
    EXPECT_TRUE(t.isAvailable());
}

TEST(Frame, CanProtect_True) {
    Frame f("Frame", true, 50, 1);
    EXPECT_TRUE(f.canProtect());
}

TEST(Frame, NoProtection_CannotProtect) {
    Frame f("Frame", false, 50, 1);
    EXPECT_FALSE(f.canProtect());
}

TEST(Frame, AddProtection_MakesProtective) {
    Frame f("Frame", false, 50, 1);
    f.addProtection();
    EXPECT_TRUE(f.canProtect());
}

TEST(Lighting, IsSafeForArtwork_True) {
    Lighting l("Lighting", true, false, 1);
    EXPECT_TRUE(l.isSafeForArtwork());
}

TEST(Lighting, Dimmed_NotSafe) {
    Lighting l("Lighting", true, true, 1);
    EXPECT_FALSE(l.isSafeForArtwork());
    l.brighten();
    EXPECT_TRUE(l.isSafeForArtwork());
}

TEST(Lighting, NotLED_NotSafe) {
    Lighting l("Lighting", false, false, 1);
    EXPECT_FALSE(l.isSafeForArtwork());
}

TEST(SecurityCamera, StartRecording_Ok) {
    SecurityCamera cam("Camera", 90.0, false, false);
    cam.startRecording();
    EXPECT_TRUE(cam.isRecording());
}

TEST(SecurityCamera, StopRecording_Ok) {
    SecurityCamera cam("Camera", 90.0, true, true);
    cam.stopRecording();
    EXPECT_FALSE(cam.isRecording());
}

TEST(SecurityCamera, NotAvailable_Throws) {
    SecurityCamera cam("Camera", 90.0, false, false);
    GalleryTool* base = &cam;
    base->breakTool();
    EXPECT_THROW(cam.startRecording(), ToolNotAvailableException);
}

TEST(DisplayStand, CanDisplay_True) {
    DisplayStand stand("Stand", 5.0, true, false);
    EXPECT_TRUE(stand.canDisplay(3.0));
    EXPECT_FALSE(stand.canDisplay(6.0));
}

TEST(DisplayStand, StartDisplay_Ok) {
    DisplayStand stand("Stand", 5.0, true, false);
    stand.startDisplay();
}

TEST(DisplayStand, NotAvailable_Throws) {
    DisplayStand stand("Stand", 5.0, true, false);
    GalleryTool* base = &stand;
    base->breakTool();
    EXPECT_THROW(stand.startDisplay(), ToolNotAvailableException);
}

TEST(Timer, StartAndFinish) {
    Timer t;
    t.start(10);
    EXPECT_FALSE(t.isFinished());
    t.tick(5);
    EXPECT_FALSE(t.isFinished());
    t.tick(5);
    EXPECT_TRUE(t.isFinished());
}

TEST(Timer, Start_Invalid_Throws) {
    Timer t;
    EXPECT_THROW(t.start(0), TimerNotSetException);
    EXPECT_THROW(t.start(-5), TimerNotSetException);
}

TEST(Timer, Tick_NegativeOrZeroIgnored) {
    Timer t;
    t.start(10);
    t.tick(0);
    t.tick(-5);
    EXPECT_FALSE(t.isFinished());
}

TEST(Timer, Tick_NotRunning_NoEffect) {
    Timer t;
    t.start(10);
    t.tick(10);
    EXPECT_TRUE(t.isFinished());
    t.tick(10);
    EXPECT_TRUE(t.isFinished());
}

TEST(RestorationKit, EquipUnplugSequence) {
    RestorationKit rk("RestorationKit", false);
    EXPECT_FALSE(rk.unequip());
    EXPECT_TRUE(rk.equip());
    EXPECT_FALSE(rk.unequip());
}

TEST(RestorationKit, Restore_NotEquipped_NoCrash) {
    RestorationKit rk("RestorationKit", false);
    rk.restore();
}

TEST(RestorationKit, Restore_NotAvailable_Throws) {
    RestorationKit rk("RestorationKit", true);
    GalleryTool* base = &rk;
    base->breakTool();
    EXPECT_THROW(rk.restore(), ToolNotAvailableException);
}

TEST(CleaningKit, Clean_Ok) {
    CleaningKit ck("CleaningKit", 1);
    ck.clean();
}

TEST(CleaningKit, Clean_NotAvailable_Throws) {
    CleaningKit ck("CleaningKit", 1);
    GalleryTool* base = &ck;
    base->breakTool();
    EXPECT_THROW(ck.clean(), ToolNotAvailableException);
}

TEST(ConditionProfile, Gradual) {
    ConditionProfile cp(20.0, 18.0, 10, true);
    double mid = cp.currentTemp(5);
    EXPECT_GT(mid, 18.0);
    EXPECT_LT(mid, 20.0);
}

TEST(ConditionProfile, Gradual_AfterDuration) {
    ConditionProfile cp(20.0, 18.0, 10, true);
    double end = cp.currentTemp(20);
    EXPECT_NEAR(18.0, end, 1e-9);
}

TEST(ConditionProfile, Jump) {
    ConditionProfile cp(20.0, 18.0, 10, false);
    EXPECT_NEAR(18.0, cp.currentTemp(0), 1e-9);
    EXPECT_TRUE(cp.isReached(18.0));
    EXPECT_TRUE(cp.isReached(19.0));
}

TEST(ConditionProfile, Reset) {
    ConditionProfile cp;
    cp.reset(30.0, 19.0, 5);
    double mid = cp.currentTemp(3);
    EXPECT_GT(mid, 19.0);
    EXPECT_LE(mid, 30.0);
}

TEST(ClimateControl, SetConditions_Ok) {
    ClimateControl cc(0.0, false, true);
    cc.setConditions(20.0, 10);
    EXPECT_TRUE(cc.isOn());
    cc.tick(10 * 60); // Wait for warmup period
    EXPECT_NEAR(20.0, cc.getTemperature(), 1e-9);
}

TEST(ClimateControl, SetConditions_InvalidLow_Throws) {
    ClimateControl cc(0.0, false, true);
    EXPECT_THROW(cc.setConditions(0.0), InvalidConditionException);
    EXPECT_THROW(cc.setConditions(-10.0), InvalidConditionException);
}

TEST(ClimateControl, SetConditions_InvalidHigh_Throws) {
    ClimateControl cc(0.0, false, true);
    EXPECT_THROW(cc.setConditions(31.0), InvalidConditionException);
}

TEST(ClimateControl, SetConditions_DoorOpen_Throws) {
    ClimateControl cc(0.0, false, false);
    EXPECT_THROW(cc.setConditions(20.0), InvalidConditionException);
}

TEST(ClimateControl, SetTimerMinutes_Invalid_Throws) {
    ClimateControl cc(0.0, false, true);
    EXPECT_THROW(cc.setTimerMinutes(0), TimerNotSetException);
}

TEST(ClimateControl, Tick_TurnsOff) {
    ClimateControl cc(0.0, false, true);
    cc.setConditions(20.0);
    cc.setTimerMinutes(1);
    EXPECT_TRUE(cc.isOn());
    cc.tick(30);
    EXPECT_TRUE(cc.isOn());
    cc.tick(30);
    EXPECT_FALSE(cc.isOn());
}

TEST(SecuritySystem, Zones_OnOff) {
    SecuritySystem s(4, 0, true, false);
    EXPECT_EQ(4, s.freeZones());
    s.activateZone();
    EXPECT_EQ(3, s.freeZones());
    s.activateZone();
    s.activateZone();
    s.activateZone();
    EXPECT_EQ(0, s.freeZones());
    s.deactivateZone();
    EXPECT_EQ(1, s.freeZones());
}

TEST(SecuritySystem, Deactivate_NoActive_NoCrash) {
    SecuritySystem s(4, 0, true, false);
    EXPECT_EQ(4, s.freeZones());
    s.deactivateZone();
    EXPECT_EQ(4, s.freeZones());
}

TEST(PaintingExhibition, Organize_Success) {
    Artwork p1 = makeArtwork("painting1", 1000.0);
    Artwork p2 = makeArtwork("painting2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    PaintingExhibition ex("Painting", &p1, &p2, &f, &l, &t, &c);
    ex.organize();
}

TEST(SculptureExhibition, Organize_Success) {
    Artwork s = makeArtwork("sculpture", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Curator c("Curator");
    SculptureExhibition ex("Sculpture", &s, &stand, &l, &f, &c);
    ex.organize();
}

TEST(PhotographyExhibition, Organize_Success) {
    Artwork ph1 = makeArtwork("photo1", 1000.0);
    Artwork ph2 = makeArtwork("photo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    PhotographyExhibition ex("Photography", &ph1, &ph2, &f, &l, &cam, &t, &c);
    ex.organize();
}

TEST(MixedMediaExhibition, Organize_Success) {
    Artwork a1 = makeArtwork("artwork1", 1000.0);
    Artwork a2 = makeArtwork("artwork2", 1000.0);
    Artwork a3 = makeArtwork("artwork3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    MixedMediaExhibition ex("Mixed Media", &a1, &a2, &a3, &stand, &l, &f, &t, &rk, &c);
    ex.organize();
}

TEST(DigitalArtExhibition, Organize_Success) {
    Artwork d1 = makeArtwork("digital1", 1000.0);
    Artwork d2 = makeArtwork("digital2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    DigitalArtExhibition ex("Digital Art", &d1, &d2, &stand, &l, &cam, &t, &c);
    ex.organize();
}

TEST(VintageExhibition, Organize_Success) {
    Artwork v1 = makeArtwork("vintage1", 1000.0);
    Artwork v2 = makeArtwork("vintage2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(0.0, false, true);
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    VintageExhibition ex("Vintage", &v1, &v2, &f, &cc, &rk, &c);
    ex.organize();
}

TEST(ContemporaryExhibition, Organize_Success) {
    Artwork c1 = makeArtwork("contemporary1", 1000.0);
    Artwork c2 = makeArtwork("contemporary2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator cu("Curator");
    ContemporaryExhibition ex("Contemporary", &c1, &c2, &stand, &l, &cam, &t, &cu);
    ex.organize();
}

TEST(MinimalistExhibition, Organize_Success) {
    Artwork m = makeArtwork("minimalist", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    MinimalistExhibition ex("Minimalist", &m, &f, &l, &c);
    ex.organize();
}

TEST(AbstractExhibition, Organize_Success) {
    Artwork a1 = makeArtwork("abstract1", 1000.0);
    Artwork a2 = makeArtwork("abstract2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    Curator c("Curator");
    AbstractExhibition ex("Abstract", &a1, &a2, &stand, &l, &f, &t, &c);
    ex.organize();
}

TEST(PortraitExhibition, Organize_Success) {
    Artwork p1 = makeArtwork("portrait1", 1000.0);
    Artwork p2 = makeArtwork("portrait2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    PortraitExhibition ex("Portrait", &p1, &p2, &f, &l, &cam, &c);
    ex.organize();
}

TEST(LandscapeExhibition, Organize_Success) {
    Artwork l1 = makeArtwork("landscape1", 1000.0);
    Artwork l2 = makeArtwork("landscape2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    LandscapeExhibition ex("Landscape", &l1, &l2, &f, &l, &t, &c);
    ex.organize();
}

TEST(RestorationExhibition, Organize_Success) {
    Artwork a = makeArtwork("artwork", 1000.0);
    RestorationKit rk("RestorationKit", false);
    CleaningKit ck("CleaningKit", 1);
    ClimateControl cc(0.0, false, true);
    Timer t;
    Curator c("Curator");
    RestorationExhibition ex("Restoration", &a, &rk, &ck, &cc, &t, &c);
    ex.organize();
}

TEST(TemporaryExhibition, Organize_Success) {
    Artwork t1 = makeArtwork("temp1", 1000.0);
    Artwork t2 = makeArtwork("temp2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    TemporaryExhibition ex("Temporary", &t1, &t2, &stand, &l, &cam, &t, &c);
    ex.organize();
}

TEST(PermanentExhibition, Organize_Success) {
    Artwork p1 = makeArtwork("permanent1", 1000.0);
    Artwork p2 = makeArtwork("permanent2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(0.0, false, true);
    SecuritySystem sec(4, 0, true, false);
    Timer t;
    Curator c("Curator");
    PermanentExhibition ex("Permanent", &p1, &p2, &f, &cc, &sec, &t, &c);
    ex.organize();
}

TEST(InteractiveExhibition, Organize_Success) {
    Artwork i1 = makeArtwork("interactive1", 1000.0);
    Artwork i2 = makeArtwork("interactive2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    InteractiveExhibition ex("Interactive", &i1, &i2, &stand, &l, &cam, &t, &c);
    ex.organize();
}

TEST(ThematicExhibition, Organize_Success) {
    Artwork th1 = makeArtwork("theme1", 1000.0);
    Artwork th2 = makeArtwork("theme2", 1000.0);
    Artwork th3 = makeArtwork("theme3", 1000.0);
    Frame f("Frame", true, 50, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    ThematicExhibition ex("Thematic", &th1, &th2, &th3, &f, &stand, &l, &t, &c);
    ex.organize();
}

TEST(RetrospectiveExhibition, Organize_Success) {
    Artwork r1 = makeArtwork("retro1", 1000.0);
    Artwork r2 = makeArtwork("retro2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(0.0, false, true);
    RestorationKit rk("RestorationKit", false);
    Timer t;
    Curator c("Curator");
    RetrospectiveExhibition ex("Retrospective", &r1, &r2, &f, &cc, &rk, &t, &c);
    ex.organize();
}

TEST(GroupExhibition, Organize_Success) {
    Artwork g1 = makeArtwork("group1", 1000.0);
    Artwork g2 = makeArtwork("group2", 1000.0);
    Artwork g3 = makeArtwork("group3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    GroupExhibition ex("Group", &g1, &g2, &g3, &stand, &l, &f, &cam, &t, &c);
    ex.organize();
}

TEST(SoloExhibition, Organize_Success) {
    Artwork s1 = makeArtwork("solo1", 1000.0);
    Artwork s2 = makeArtwork("solo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Timer t;
    Curator c("Curator");
    SoloExhibition ex("Solo", &s1, &s2, &f, &l, &stand, &t, &c);
    ex.organize();
}

TEST(PaintingExhibition, NoFrame_Throws) {
    Artwork p1 = makeArtwork("painting1", 1000.0);
    Artwork p2 = makeArtwork("painting2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    PaintingExhibition ex("Painting", &p1, &p2, nullptr, &l, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(PaintingExhibition, NoTimer_Throws) {
    Artwork p1 = makeArtwork("painting1", 1000.0);
    Artwork p2 = makeArtwork("painting2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    PaintingExhibition ex("Painting", &p1, &p2, &f, &l, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(SculptureExhibition, NoStand_Throws) {
    Artwork s = makeArtwork("sculpture", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Curator c("Curator");
    SculptureExhibition ex("Sculpture", &s, nullptr, &l, &f, &c);
    EXPECT_THROW(ex.organize(), InsufficientSpaceException);
}

TEST(PhotographyExhibition, NoCamera_Throws) {
    Artwork ph1 = makeArtwork("photo1", 1000.0);
    Artwork ph2 = makeArtwork("photo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    PhotographyExhibition ex("Photography", &ph1, &ph2, &f, &l, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(MixedMediaExhibition, NoArtwork_Throws) {
    Artwork a2 = makeArtwork("artwork2", 1000.0);
    Artwork a3 = makeArtwork("artwork3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    MixedMediaExhibition ex("Mixed Media", nullptr, &a2, &a3, &stand, &l, &f, &t, &rk, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(DigitalArtExhibition, NoStand_Throws) {
    Artwork d1 = makeArtwork("digital1", 1000.0);
    Artwork d2 = makeArtwork("digital2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    DigitalArtExhibition ex("Digital Art", &d1, &d2, nullptr, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(VintageExhibition, NoClimate_Throws) {
    Artwork v1 = makeArtwork("vintage1", 1000.0);
    Artwork v2 = makeArtwork("vintage2", 1000.0);
    Frame f("Frame", true, 50, 1);
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    VintageExhibition ex("Vintage", &v1, &v2, &f, nullptr, &rk, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(MinimalistExhibition, NoFrame_Throws) {
    Artwork m = makeArtwork("minimalist", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    MinimalistExhibition ex("Minimalist", &m, nullptr, &l, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(AbstractExhibition, NoTimer_Throws) {
    Artwork a1 = makeArtwork("abstract1", 1000.0);
    Artwork a2 = makeArtwork("abstract2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Curator c("Curator");
    AbstractExhibition ex("Abstract", &a1, &a2, &stand, &l, &f, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(PortraitExhibition, NoArtwork_Throws) {
    Artwork p2 = makeArtwork("portrait2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    PortraitExhibition ex("Portrait", nullptr, &p2, &f, &l, &cam, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(LandscapeExhibition, NoTimer_Throws) {
    Artwork l1 = makeArtwork("landscape1", 1000.0);
    Artwork l2 = makeArtwork("landscape2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    LandscapeExhibition ex("Landscape", &l1, &l2, &f, &l, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(RestorationExhibition, NoArtwork_Throws) {
    RestorationKit rk("RestorationKit", false);
    CleaningKit ck("CleaningKit", 1);
    ClimateControl cc(0.0, false, true);
    Timer t;
    Curator c("Curator");
    RestorationExhibition ex("Restoration", nullptr, &rk, &ck, &cc, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(TemporaryExhibition, NoStand_Throws) {
    Artwork t1 = makeArtwork("temp1", 1000.0);
    Artwork t2 = makeArtwork("temp2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    TemporaryExhibition ex("Temporary", &t1, &t2, nullptr, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(PermanentExhibition, NoClimate_Throws) {
    Artwork p1 = makeArtwork("permanent1", 1000.0);
    Artwork p2 = makeArtwork("permanent2", 1000.0);
    Frame f("Frame", true, 50, 1);
    SecuritySystem sec(4, 0, true, false);
    Timer t;
    Curator c("Curator");
    PermanentExhibition ex("Permanent", &p1, &p2, &f, nullptr, &sec, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(InteractiveExhibition, NoCamera_Throws) {
    Artwork i1 = makeArtwork("interactive1", 1000.0);
    Artwork i2 = makeArtwork("interactive2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    InteractiveExhibition ex("Interactive", &i1, &i2, &stand, &l, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(ThematicExhibition, NoStand_Throws) {
    Artwork th1 = makeArtwork("theme1", 1000.0);
    Artwork th2 = makeArtwork("theme2", 1000.0);
    Artwork th3 = makeArtwork("theme3", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    ThematicExhibition ex("Thematic", &th1, &th2, &th3, &f, nullptr, &l, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(RetrospectiveExhibition, NoClimate_Throws) {
    Artwork r1 = makeArtwork("retro1", 1000.0);
    Artwork r2 = makeArtwork("retro2", 1000.0);
    Frame f("Frame", true, 50, 1);
    RestorationKit rk("RestorationKit", false);
    Timer t;
    Curator c("Curator");
    RetrospectiveExhibition ex("Retrospective", &r1, &r2, &f, nullptr, &rk, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(GroupExhibition, NoArtwork_Throws) {
    Artwork g2 = makeArtwork("group2", 1000.0);
    Artwork g3 = makeArtwork("group3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    GroupExhibition ex("Group", nullptr, &g2, &g3, &stand, &l, &f, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(SoloExhibition, NoStand_Throws) {
    Artwork s1 = makeArtwork("solo1", 1000.0);
    Artwork s2 = makeArtwork("solo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    SoloExhibition ex("Solo", &s1, &s2, &f, &l, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

class DummyExhibition : public Exhibition {
public:
    bool organized;
    DummyExhibition(const char* n) : Exhibition(n), organized(false) {}
    void organize() override { organized = true; }
};

TEST(GalleryMenu, Run_ValidChoiceThenExit) {
    DummyExhibition d("Dummy");
    GalleryMenu m;
    m.addExhibition(&d);
    istringstream input("1\n0\n");
    streambuf* oldCin = cin.rdbuf(input.rdbuf());
    m.run();
    cin.rdbuf(oldCin);
    EXPECT_TRUE(d.organized);
}

TEST(GalleryMenu, Run_InvalidChoiceThenExit) {
    DummyExhibition d("Dummy");
    GalleryMenu m;
    m.addExhibition(&d);
    istringstream input("5\n1\n0\n");
    streambuf* oldCin = cin.rdbuf(input.rdbuf());
    m.run();
    cin.rdbuf(oldCin);
    EXPECT_TRUE(d.organized);
}

TEST(GalleryMenu, Run_InvalidInput_Break) {
    DummyExhibition d("Dummy");
    GalleryMenu m;
    m.addExhibition(&d);
    istringstream input("abc\n");
    streambuf* oldCin = cin.rdbuf(input.rdbuf());
    m.run();
    cin.rdbuf(oldCin);
    EXPECT_FALSE(d.organized);
}

TEST(Visitor, PurchaseTicket_Ok) {
    Visitor v("Visitor", 1, false);
    v.purchaseTicket();
    v.enterGallery();
}

TEST(Visitor, EnterGallery_NoTicket_Throws) {
    Visitor v("Visitor", 1, false);
    EXPECT_THROW(v.enterGallery(), InvalidAccessException);
}

TEST(Visitor, ViewArtwork_Ok) {
    Visitor v("Visitor", 1, true);
    Artwork a = makeArtwork("artwork", 100.0);
    v.viewArtwork(&a);
}

TEST(Visitor, ViewArtwork_NotFound_Throws) {
    Visitor v("Visitor", 1, true);
    EXPECT_THROW(v.viewArtwork(nullptr), ArtworkNotFoundException);
}

TEST(Visitor, CanAccessRestricted_True) {
    Visitor v("Visitor", 3, true);
    EXPECT_TRUE(v.canAccessRestricted());
}

TEST(Visitor, CanAccessRestricted_False) {
    Visitor v("Visitor", 2, true);
    EXPECT_FALSE(v.canAccessRestricted());
}

TEST(Visitor, GetName) {
    Visitor p("John Doe", 2, true);
    EXPECT_STREQ("John Doe", p.getName());
}

TEST(Visitor, GetVisitDuration) {
    Visitor p("Test", 2, true);
    EXPECT_NEAR(0.0, p.getVisitDuration(), 1e-9);
    Artwork a = makeArtwork("Test", 100.0);
    p.viewArtwork(&a);
    EXPECT_NEAR(5.0, p.getVisitDuration(), 1e-9);
}

TEST(Visitor, CalculateTotalBaggageWeight) {
    Visitor v("Test", 2, true);
    EXPECT_NEAR(50.0, v.calculateTotalBaggageWeight(30.0, 20.0), 1e-9);
}

TEST(Visitor, HasValidVisa) {
    Visitor v("Test", 2, true);
    EXPECT_TRUE(v.hasValidVisa("International"));
    Visitor v2("Test", 1, true);
    EXPECT_FALSE(v2.hasValidVisa("International"));
}

TEST(Visitor, CalculateLoyaltyDiscount) {
    Visitor v("Test", 2, true);
    for (int i = 0; i < 6; i++) {
        v.addFavorite();
    }
    EXPECT_GT(v.calculateLoyaltyDiscount(100.0), 0.0);
}

TEST(Visitor, RequestSpecialMeal) {
    Visitor v("Test", 2, true);
    EXPECT_TRUE(v.requestSpecialMeal("Vegetarian"));
    Visitor v2("Test", 2, false);
    EXPECT_FALSE(v2.requestSpecialMeal("Vegetarian"));
}

TEST(Visitor, CheckInForExhibition) {
    Visitor v("Test", 2, true);
    EXPECT_TRUE(v.checkInForExhibition("EXH001"));
    Visitor v2("Test", 2, false);
    EXPECT_FALSE(v2.checkInForExhibition("EXH001"));
}

TEST(Visitor, CancelReservation) {
    Visitor v("Test", 2, true);
    EXPECT_TRUE(v.cancelReservation(1));
    EXPECT_FALSE(v.cancelReservation(-1));
}

TEST(Visitor, CalculateTotalTravelCost) {
    Visitor v("Test", 2, true);
    EXPECT_NEAR(150.0, v.calculateTotalTravelCost(100.0, 30.0, 20.0), 1e-9);
}

TEST(GalleryMenu, GetExhibitionCount) {
    GalleryMenu menu;
    EXPECT_EQ(0, menu.getExhibitionCount());
    
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    
    PaintingExhibition ex("Test Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    menu.addExhibition(&ex);
    EXPECT_EQ(1, menu.getExhibitionCount());
}

TEST(GalleryMenu, GetCurrentVisitor) {
    GalleryMenu menu;
    EXPECT_EQ(nullptr, menu.getCurrentVisitor());
    
    Visitor v("Test", 2, true);
    menu.setVisitor(&v);
    EXPECT_EQ(&v, menu.getCurrentVisitor());
}

TEST(GalleryMenu, SearchExhibitions) {
    GalleryMenu menu;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    
    PaintingExhibition ex1("Test Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    PaintingExhibition ex2("Another Test", &a1, &a2, &frame, &lighting, &timer, &c);
    menu.addExhibition(&ex1);
    menu.addExhibition(&ex2);
    EXPECT_EQ(2, menu.searchExhibitions("Test"));
}

TEST(GalleryMenu, FilterAvailableExhibitions) {
    GalleryMenu menu;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    
    PaintingExhibition ex("Test Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    menu.addExhibition(&ex);
    EXPECT_GE(menu.filterAvailableExhibitions(), 0);
}

TEST(GalleryMenu, SortExhibitionsByName) {
    GalleryMenu menu;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    
    PaintingExhibition ex1("Zebra Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    PaintingExhibition ex2("Alpha Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    menu.addExhibition(&ex1);
    menu.addExhibition(&ex2);
    menu.sortExhibitionsByName();
    EXPECT_STREQ("Alpha Exhibition", menu.getExhibitionByIndex(0)->getName());
}

TEST(GalleryMenu, GetExhibitionByIndex) {
    GalleryMenu menu;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    
    PaintingExhibition ex("Test Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    menu.addExhibition(&ex);
    EXPECT_EQ(&ex, menu.getExhibitionByIndex(0));
    EXPECT_EQ(nullptr, menu.getExhibitionByIndex(10));
}

TEST(TicketSystem, CreateBooking) {
    TicketSystem system;
    Visitor v("Test", 2, true);
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_TRUE(system.createBooking(&v, &ex));
}

TEST(TicketSystem, TransferMoney) {
    TicketSystem system;
    EXPECT_TRUE(system.transferMoney("1234567890123", "9876543210987", 100.0));
    EXPECT_FALSE(system.transferMoney("123", "456", 100.0));
}

TEST(TicketSystem, VerifyPassword) {
    TicketSystem system;
    EXPECT_TRUE(system.verifyPassword("account1", "password123"));
    EXPECT_FALSE(system.verifyPassword("account1", "123"));
}

TEST(TicketSystem, ProcessPayment) {
    TicketSystem system;
    EXPECT_TRUE(system.processPayment(100.0, "1234567890123"));
    EXPECT_FALSE(system.processPayment(-10.0, "1234567890123"));
}

TEST(TicketSystem, CalculateTicketPrice) {
    TicketSystem system;
    Visitor v("Test", 2, true);
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_GT(system.calculateTicketPrice(&ex, &v), 0.0);
}

TEST(Artwork, GetName) {
    Size s(100.0, &cmUnit);
    Artwork a("Test Artwork", s, 1000.0, false);
    EXPECT_STREQ("Test Artwork", a.getName());
}

TEST(Artwork, GetValue) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 5000.0, false);
    EXPECT_NEAR(5000.0, a.getValue(), 1e-9);
}

TEST(Artwork, IsAuthenticated) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_FALSE(a.isAuthenticated());
    a.authenticate();
    EXPECT_TRUE(a.isAuthenticated());
}

TEST(Artwork, CalculateMaintenanceCost) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_GT(a.calculateMaintenanceCost(), 0.0);
}

TEST(Artwork, IsAvailableForDisplay) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_FALSE(a.isAvailableForDisplay());
    a.authenticate();
    EXPECT_TRUE(a.isAvailableForDisplay());
}

TEST(Artwork, CalculateDepreciation) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_GT(a.calculateDepreciation(2025), 0.0);
}

TEST(Artwork, NeedsInspection) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_FALSE(a.needsInspection());
}

TEST(Artwork, CalculateRestorationCost) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_GT(a.calculateRestorationCost(0.5), 0.0);
}

TEST(Size, GetValue) {
    Size s(150.0, &cmUnit, false, 1);
    EXPECT_NEAR(150.0, s.getValue(), 1e-9);
}

TEST(Size, IsApproximate) {
    Size exact(100.0, &cmUnit, false, 1);
    Size approx(100.0, &cmUnit, true, 2);
    EXPECT_FALSE(exact.isApproximate());
    EXPECT_TRUE(approx.isApproximate());
}

TEST(Size, ConvertToDimension) {
    Dimension cm("cm", 1.0, true, 1);
    Dimension inch("in", 2.54, false, 2);
    Size s(100.0, &cm, false, 1);
    EXPECT_GT(s.convertToDimension(&inch), 0.0);
}

TEST(Size, CalculateDifference) {
    Size s1(100.0, &cmUnit, false, 1);
    Size s2(150.0, &cmUnit, false, 2);
    EXPECT_GT(s1.calculateDifference(s2), 0.0);
}

TEST(Dimension, FromCentimeters) {
    Dimension cm("cm", 1.0, true, 1);
    Dimension inch("in", 2.54, false, 2);
    
    EXPECT_NEAR(10.0, cm.fromCentimeters(10.0), 1e-9);
    EXPECT_NEAR(10.0, inch.fromCentimeters(25.4), 1e-9);
}

TEST(Dimension, ConvertTo) {
    Dimension cm("cm", 1.0, true, 1);
    Dimension inch("in", 2.54, false, 2);
    EXPECT_GT(cm.convertTo(10.0, &inch), 0.0);
}

TEST(Dimension, ValidateConversion) {
    Dimension d("cm", 1.0, true, 1);
    EXPECT_TRUE(d.validateConversion(10.0));
    EXPECT_FALSE(d.validateConversion(-10.0));
}

TEST(Frame, GetWidth) {
    Frame frame("Test", true, 75, 1);
    EXPECT_EQ(75, frame.getWidth());
}

TEST(Frame, CalculateComfortScore) {
    Frame frame("Test", true, 50, 1);
    EXPECT_GT(frame.calculateComfortScore(), 0.0);
}

TEST(Frame, IsSuitableForArtwork) {
    Frame frame("Test", true, 50, 1);
    EXPECT_TRUE(frame.isSuitableForArtwork(100.0));
    EXPECT_FALSE(frame.isSuitableForArtwork(200.0));
}

TEST(Lighting, IsLED) {
    Lighting led("LED", true, false, 1);
    Lighting nonLED("NonLED", false, false, 2);
    EXPECT_TRUE(led.isLED());
    EXPECT_FALSE(nonLED.isLED());
}

TEST(Lighting, CalculatePowerConsumption) {
    Lighting led("LED", true, false, 1);
    EXPECT_GT(led.calculatePowerConsumption(), 0.0);
}

TEST(Lighting, SetBrightness) {
    Lighting l("Lighting", true, false, 1);
    l.setBrightness(30);
    EXPECT_FALSE(l.isSafeForArtwork());
}

TEST(Lighting, MeetsSafetyStandards) {
    Lighting led("LED", true, false, 1);
    EXPECT_TRUE(led.meetsSafetyStandards());
}

TEST(SecurityCamera, GetAngle) {
    SecurityCamera cam("Test", 120.0, false, false);
    EXPECT_NEAR(120.0, cam.getAngle(), 1e-9);
}

TEST(SecurityCamera, AdjustAngle) {
    SecurityCamera cam("Test", 90.0, false, false);
    cam.adjustAngle(120.0);
    EXPECT_NEAR(120.0, cam.getAngle(), 1e-9);
}

TEST(SecurityCamera, CalculateCoverageArea) {
    SecurityCamera cam("Test", 90.0, false, false);
    EXPECT_GT(cam.calculateCoverageArea(10.0), 0.0);
}

TEST(SecurityCamera, CanMonitorArea) {
    SecurityCamera cam("Test", 90.0, false, true);
    EXPECT_TRUE(cam.canMonitorArea(10.0));
}

TEST(DisplayStand, CheckHasCover) {
    DisplayStand standWithCover("Stand1", 5.0, true, false);
    DisplayStand standWithoutCover("Stand2", 3.0, false, false);
    EXPECT_TRUE(standWithCover.checkHasCover());
    EXPECT_FALSE(standWithoutCover.checkHasCover());
}

TEST(DisplayStand, CalculateDisplayTime) {
    DisplayStand stand("Stand", 5.0, true, false);
    EXPECT_GT(stand.calculateDisplayTime(10), 0.0);
}

TEST(DisplayStand, CanHandleVisitorLoad) {
    DisplayStand stand("Stand", 5.0, true, false);
    EXPECT_TRUE(stand.canHandleVisitorLoad(5));
}

TEST(DisplayStand, CalculateUtilization) {
    DisplayStand stand("Stand", 5.0, true, false);
    EXPECT_GT(stand.calculateUtilization(), 0.0);
}

TEST(Timer, GetElapsed) {
    Timer timer(100, false, 0, 1);
    timer.start(300);
    timer.tick(50);
    EXPECT_EQ(50, timer.getElapsed());
}

TEST(Timer, IsRunning) {
    Timer timer(0, false, 0, 1);
    EXPECT_FALSE(timer.isRunning());
    timer.start(100);
    EXPECT_TRUE(timer.isRunning());
}

TEST(Timer, CalculateRemainingTime) {
    Timer timer(100, false, 0, 1);
    timer.start(100);
    timer.tick(30);
    EXPECT_EQ(70, timer.calculateRemainingTime());
}

TEST(Timer, Reset) {
    Timer timer(100, false, 0, 1);
    timer.start(100);
    timer.tick(50);
    timer.reset();
    EXPECT_EQ(0, timer.getElapsed());
}

TEST(Timer, Pause) {
    Timer timer(0, false, 0, 1);
    timer.start(100);
    timer.pause();
    EXPECT_FALSE(timer.isRunning());
}

TEST(Timer, Resume) {
    Timer timer(0, false, 0, 1);
    timer.start(100);
    timer.pause();
    timer.resume();
    EXPECT_TRUE(timer.isRunning());
}

TEST(RestorationKit, IsEquipped) {
    RestorationKit kit("Test", false);
    EXPECT_FALSE(kit.isEquipped());
    kit.equip();
    EXPECT_TRUE(kit.isEquipped());
}

TEST(RestorationKit, CalculateRestorationEfficiency) {
    RestorationKit kit("Test", true);
    EXPECT_GT(kit.calculateRestorationEfficiency(), 0.0);
}

TEST(RestorationKit, HasRequiredTools) {
    RestorationKit kit("Test", true);
    EXPECT_TRUE(kit.hasRequiredTools(5));
}

TEST(CleaningKit, GetId) {
    CleaningKit kit("Test", 5);
    EXPECT_EQ(5, kit.getId());
}

TEST(ConditionProfile, GetDuration) {
    ConditionProfile profile(25.0, 30.0, 120, true);
    EXPECT_EQ(120, profile.getDuration());
}

TEST(ConditionProfile, CalculateTemperatureChangeRate) {
    ConditionProfile profile(20.0, 25.0, 100, true);
    EXPECT_GT(profile.calculateTemperatureChangeRate(), 0.0);
}

TEST(ConditionProfile, IsValidProfile) {
    ConditionProfile profile(20.0, 25.0, 100, true);
    EXPECT_TRUE(profile.isValidProfile());
}

TEST(ConditionProfile, GetEstimatedCompletionTime) {
    ConditionProfile profile(20.0, 25.0, 100, true);
    EXPECT_GT(profile.getEstimatedCompletionTime(50), 0);
}

TEST(ClimateControl, GetHumidity) {
    ClimateControl climate(50.0, false, true);
    EXPECT_NEAR(50.0, climate.getHumidity(), 1e-9);
}

TEST(ClimateControl, GetTargetHumidity) {
    ClimateControl climate(50.0, false, true);
    climate.setConditions(20.0, 10);
    EXPECT_NEAR(50.0, climate.getHumidity(), 1e-9);
}

TEST(ClimateControl, CalculateEnergyConsumption) {
    ClimateControl climate(0.0, false, true);
    climate.setConditions(20.0, 10);
    EXPECT_GT(climate.calculateEnergyConsumption(), 0.0);
}

TEST(ClimateControl, IsOptimalClimate) {
    ClimateControl climate(0.0, false, true);
    climate.setConditions(22.0, 10);
    climate.setHumidity(50.0);
    EXPECT_TRUE(climate.isOptimalClimate());
}

TEST(ClimateControl, AdjustTemperatureGradually) {
    ClimateControl climate(0.0, false, true);
    climate.adjustTemperatureGradually(22.0, 10);
    EXPECT_TRUE(climate.isOn());
}

TEST(ClimateControl, CalculateMaintenanceSchedule) {
    ClimateControl climate(0.0, false, true);
    EXPECT_GT(climate.calculateMaintenanceSchedule(), 0);
}

TEST(SecuritySystem, IsOn) {
    SecuritySystem security(6, 0, true, true);
    EXPECT_TRUE(security.isOn());
    SecuritySystem securityOff(6, 0, true, false);
    EXPECT_FALSE(securityOff.isOn());
}

TEST(SecuritySystem, ActivateAllZones) {
    SecuritySystem security(4, 0, true, false);
    security.activateAllZones();
    EXPECT_EQ(0, security.freeZones());
}

TEST(SecuritySystem, DeactivateAllZones) {
    SecuritySystem security(4, 2, true, true);
    security.deactivateAllZones();
    EXPECT_EQ(4, security.freeZones());
}

TEST(SecuritySystem, CheckSecurityBreach) {
    SecuritySystem security(4, 2, true, true);
    EXPECT_TRUE(security.checkSecurityBreach());
}

TEST(SecuritySystem, CalculateSecurityCoverage) {
    SecuritySystem security(4, 2, true, true);
    EXPECT_GT(security.calculateSecurityCoverage(), 0.0);
}

TEST(Exhibition, IsFullyBooked) {
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_FALSE(ex.isFullyBooked());
}

TEST(Exhibition, CalculateExhibitionDuration) {
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_GT(ex.calculateExhibitionDuration(), 0.0);
}

TEST(Exhibition, IsOnTime) {
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_TRUE(ex.isOnTime());
}

TEST(Exhibition, GetAvailableSeats) {
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_GT(ex.getAvailableSeats(), 0);
}

TEST(GalleryTool, GetName) {
    Frame frame("Test Frame", true, 50, 1);
    EXPECT_STREQ("Test Frame", frame.getName());
}

TEST(Dimension, SetPrecision) {
    Dimension d("cm", 1.0, true, 1);
    d.setPrecision(0.5);
}

TEST(Artwork, SetInsuranceValue) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    a.setInsuranceValue(2000.0);
}

TEST(Artwork, CalculateRestorationCost_Zero) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_NEAR(0.0, a.calculateRestorationCost(0.0), 1e-9);
}

TEST(Artwork, CalculateRestorationCost_Negative) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_NEAR(0.0, a.calculateRestorationCost(-1.0), 1e-9);
}

TEST(GalleryTool, UseTool_Unavailable) {
    GalleryTool tool("Test", true, false, 10);
    EXPECT_THROW(tool.useTool(), ToolNotAvailableException);
}

TEST(GalleryTool, UseTool_Dirty) {
    GalleryTool tool("Test", false, true, 10);
    EXPECT_THROW(tool.useTool(), ToolNotAvailableException);
}

TEST(GalleryTool, UseTool_Broken) {
    GalleryTool tool("Test", true, true, 0);
    EXPECT_THROW(tool.useTool(), ToolNotAvailableException);
}

TEST(GalleryTool, UseTool_DurabilityDecrease) {
    GalleryTool tool("Test", true, true, 1);
    tool.useTool();
    EXPECT_FALSE(tool.isAvailable());
}

TEST(GalleryTool, BreakTool) {
    GalleryTool tool("Test", true, true, 10);
    tool.breakTool();
    EXPECT_FALSE(tool.isAvailable());
}

TEST(Frame, RemoveProtection) {
    Frame frame("Test", true, 50, 1);
    frame.removeProtection();
    EXPECT_FALSE(frame.canProtect());
}

TEST(Frame, CalculateComfortScore_NoProtection) {
    Frame frame("Test", false, 30, 1);
    double score = frame.calculateComfortScore();
    EXPECT_GT(score, 0.0);
}

TEST(Frame, CalculateComfortScore_Wide) {
    Frame frame("Test", true, 60, 1);
    double score = frame.calculateComfortScore();
    EXPECT_GT(score, 0.0);
}

TEST(Frame, IsSuitableForArtwork_InvalidWeight) {
    Frame frame("Test", true, 50, 1);
    EXPECT_FALSE(frame.isSuitableForArtwork(0.0));
    EXPECT_FALSE(frame.isSuitableForArtwork(-10.0));
}

TEST(Frame, IsSuitableForArtwork_TooHeavy) {
    Frame frame("Test", true, 50, 1);
    EXPECT_FALSE(frame.isSuitableForArtwork(200.0));
}

TEST(Frame, AdjustPosition_Valid) {
    Frame frame("Test", true, 50, 1);
    frame.adjustPosition(30);
    frame.adjustPosition(-30);
    frame.adjustPosition(0);
}

TEST(Frame, AdjustPosition_Invalid) {
    Frame frame("Test", true, 50, 1);
    frame.adjustPosition(50);
    frame.adjustPosition(-50);
}

TEST(Lighting, Brighten) {
    Lighting light("Test", true, true, 1);
    light.brighten();
}

TEST(Lighting, CalculatePowerConsumption_NonLED) {
    Lighting light("Test", false, false, 1);
    EXPECT_NEAR(40.0, light.calculatePowerConsumption(), 1e-9);
}

TEST(Lighting, CalculatePowerConsumption_NonLED_Dimmed) {
    Lighting light("Test", false, true, 1);
    EXPECT_NEAR(20.0, light.calculatePowerConsumption(), 1e-9);
}

TEST(Lighting, SetBrightness_Valid) {
    Lighting light("Test", true, false, 1);
    light.setBrightness(75);
    light.setBrightness(25);
}

TEST(Lighting, SetBrightness_Invalid) {
    Lighting light("Test", true, false, 1);
    light.setBrightness(150);
    light.setBrightness(-10);
}

TEST(SecurityCamera, StopRecording) {
    SecurityCamera cam("Test", 90.0, true, true);
    cam.stopRecording();
    EXPECT_FALSE(cam.isRecording());
}

TEST(SecurityCamera, CalculateCoverageArea_Zero) {
    SecurityCamera cam("Test", 90.0, false, false);
    EXPECT_NEAR(0.0, cam.calculateCoverageArea(0.0), 1e-9);
}

TEST(SecurityCamera, CalculateCoverageArea_Negative) {
    SecurityCamera cam("Test", 90.0, false, false);
    EXPECT_NEAR(0.0, cam.calculateCoverageArea(-10.0), 1e-9);
}

TEST(SecurityCamera, CanMonitorArea_Zero) {
    SecurityCamera cam("Test", 90.0, false, true);
    EXPECT_FALSE(cam.canMonitorArea(0.0));
}

TEST(SecurityCamera, CanMonitorArea_Negative) {
    SecurityCamera cam("Test", 90.0, false, true);
    EXPECT_FALSE(cam.canMonitorArea(-10.0));
}

TEST(SecurityCamera, CanMonitorArea_Inactive) {
    SecurityCamera cam("Test", 90.0, false, false);
    EXPECT_FALSE(cam.canMonitorArea(10.0));
}

TEST(SecurityCamera, AdjustAngle_Invalid) {
    SecurityCamera cam("Test", 90.0, false, false);
    cam.adjustAngle(400.0);
    cam.adjustAngle(-10.0);
}

TEST(DisplayStand, StopDisplay) {
    DisplayStand stand("Test", 5.0, true, true);
    stand.stopDisplay();
}

TEST(DisplayStand, CanDisplay_Invalid) {
    DisplayStand stand("Test", 5.0, true, false);
    EXPECT_TRUE(stand.canDisplay(0.0));
    EXPECT_TRUE(stand.canDisplay(-10.0));
}

TEST(DisplayStand, CanDisplay_TooLarge) {
    DisplayStand stand("Test", 5.0, true, false);
    EXPECT_FALSE(stand.canDisplay(10.0));
}


TEST(Timer, IsFinished) {
    Timer timer(0, false, 0, 1);
    timer.start(100);
    timer.tick(100);
    EXPECT_TRUE(timer.isFinished());
}

TEST(Timer, IsFinished_NotFinished) {
    Timer timer(0, false, 0, 1);
    timer.start(100);
    timer.tick(50);
    EXPECT_FALSE(timer.isFinished());
}


TEST(RestorationKit, Unequip) {
    RestorationKit kit("Test", true);
    kit.equip();
    kit.unequip();
    EXPECT_FALSE(kit.isEquipped());
}


TEST(ConditionProfile, IsReached_False) {
    ConditionProfile profile(20.0, 25.0, 100, true);
    EXPECT_FALSE(profile.isReached(15.0));
}

TEST(ClimateControl, TurnOff) {
    ClimateControl climate(20.0, true, true);
    climate.turnOff();
    EXPECT_FALSE(climate.isOn());
    EXPECT_NEAR(0.0, climate.getTemperature(), 1e-9);
}

TEST(ClimateControl, CloseDoor) {
    ClimateControl climate(20.0, true, false);
    climate.closeDoor();
    EXPECT_TRUE(climate.isDoorClosed());
}

TEST(ClimateControl, OpenDoor) {
    ClimateControl climate(20.0, true, true);
    climate.openDoor();
    EXPECT_FALSE(climate.isDoorClosed());
}

TEST(ClimateControl, SetTimerMinutes) {
    ClimateControl climate(20.0, true, true);
    climate.setTimerMinutes(30);
}

TEST(ClimateControl, SetTimerMinutes_Invalid) {
    ClimateControl climate(20.0, true, true);
    EXPECT_THROW(climate.setTimerMinutes(0), TimerNotSetException);
    EXPECT_THROW(climate.setTimerMinutes(-10), TimerNotSetException);
}

TEST(ClimateControl, Tick) {
    ClimateControl climate(0.0, false, true);
    climate.setConditions(20.0, 10);
    climate.tick(60);
    EXPECT_GT(climate.getTemperature(), 0.0);
}

TEST(ClimateControl, Tick_TimerFinished) {
    ClimateControl climate(0.0, false, true);
    climate.setConditions(20.0, 1);
    climate.setTimerMinutes(1);
    climate.tick(70);
    EXPECT_FALSE(climate.isOn());
}


TEST(ClimateControl, ActivateAlarm) {
    ClimateControl climate(20.0, true, true);
    climate.activateAlarm();
    EXPECT_TRUE(climate.isAlarmActive());
}

TEST(ClimateControl, IsAlarmActive) {
    ClimateControl climate(20.0, true, true);
    EXPECT_FALSE(climate.isAlarmActive());
    climate.activateAlarm();
    EXPECT_TRUE(climate.isAlarmActive());
}

TEST(ClimateControl, AdjustTemperatureGradually_Invalid) {
    ClimateControl climate(20.0, true, true);
    climate.adjustTemperatureGradually(0.0, 5);
    climate.adjustTemperatureGradually(35.0, 5);
    climate.adjustTemperatureGradually(25.0, 0);
}

TEST(SecuritySystem, DeactivateZone) {
    SecuritySystem security(10, 5, false, true);
    security.deactivateZone();
}

TEST(Artwork, UseSize_ZeroSize) {
    Size s(0.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    a.useSize(0.0);
}

TEST(Artwork, CalculateMaintenanceCost_Authenticated) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    a.authenticate();
    double cost = a.calculateMaintenanceCost();
    EXPECT_GT(cost, 0.0);
}

TEST(Artwork, CalculateMaintenanceCost_NotAuthenticated) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    double cost = a.calculateMaintenanceCost();
    EXPECT_GT(cost, 0.0);
}

TEST(Artwork, IsAvailableForDisplay_NotAuthenticated) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    EXPECT_FALSE(a.isAvailableForDisplay());
}

TEST(Artwork, IsAvailableForDisplay_Fragile) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, true);
    a.authenticate();
    EXPECT_FALSE(a.isAvailableForDisplay());
}

TEST(Artwork, CalculateDepreciation_Valid) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    double dep = a.calculateDepreciation(2025);
    EXPECT_GT(dep, 0.0);
}

TEST(GalleryTool, IsAvailable_AllConditions) {
    GalleryTool tool("Test", true, true, 10);
    EXPECT_TRUE(tool.isAvailable());
}

TEST(GalleryTool, IsAvailable_NotClean) {
    GalleryTool tool("Test", false, true, 10);
    EXPECT_FALSE(tool.isAvailable());
}

TEST(GalleryTool, IsAvailable_NotAvailable) {
    GalleryTool tool("Test", true, false, 10);
    EXPECT_FALSE(tool.isAvailable());
}

TEST(GalleryTool, IsAvailable_ZeroDurability) {
    GalleryTool tool("Test", true, true, 0);
    EXPECT_FALSE(tool.isAvailable());
}

TEST(Frame, CanProtect_NotAvailable) {
    Frame frame("Test", true, 50, 1);
    GalleryTool* base = &frame;
    base->breakTool();
    EXPECT_FALSE(frame.canProtect());
}

TEST(Lighting, MeetsSafetyStandards_NotLED) {
    Lighting light("Test", false, false, 1);
    EXPECT_FALSE(light.meetsSafetyStandards());
}

TEST(Lighting, MeetsSafetyStandards_NotAvailable) {
    Lighting light("Test", true, false, 1);
    GalleryTool* base = &light;
    base->breakTool();
    EXPECT_FALSE(light.meetsSafetyStandards());
}

TEST(SecurityCamera, AdjustAngle_Valid) {
    SecurityCamera cam("Test", 90.0, false, false);
    cam.adjustAngle(180.0);
    EXPECT_NEAR(180.0, cam.getAngle(), 1e-9);
}

TEST(DisplayStand, StartDisplay_NotAvailable) {
    DisplayStand stand("Test", 5.0, true, false);
    GalleryTool* base = &stand;
    base->breakTool();
    EXPECT_THROW(stand.startDisplay(), ToolNotAvailableException);
}

TEST(Timer, Tick_NotRunning) {
    Timer timer(0, false, 0, 1);
    timer.tick(10);
    EXPECT_EQ(0, timer.getElapsed());
}

TEST(Timer, CalculateRemainingTime_NotStarted) {
    Timer timer(0, false, 0, 1);
    EXPECT_EQ(0, timer.calculateRemainingTime());
}

TEST(RestorationKit, HasRequiredTools_NotEnough) {
    RestorationKit kit("Test", true);
    EXPECT_FALSE(kit.hasRequiredTools(100));
}

TEST(CleaningKit, Clean_Available) {
    CleaningKit kit("Test", 1);
    kit.clean();
}

TEST(ConditionProfile, CurrentTemp_BeforeStart) {
    ConditionProfile profile(20.0, 25.0, 100, true);
    EXPECT_NEAR(20.0, profile.currentTemp(0), 1e-9);
}

TEST(ConditionProfile, IsReached_True) {
    ConditionProfile profile(20.0, 25.0, 100, true);
    EXPECT_TRUE(profile.isReached(25.0));
    EXPECT_TRUE(profile.isReached(26.0));
}

TEST(ClimateControl, Tick_NotOn) {
    ClimateControl climate(20.0, false, true);
    climate.tick(60);
    EXPECT_NEAR(20.0, climate.getTemperature(), 1e-9);
}

TEST(ClimateControl, Tick_ZeroDelta) {
    ClimateControl climate(0.0, false, true);
    climate.setConditions(20.0, 10);
    climate.tick(0);
    EXPECT_NEAR(20.0, climate.getTemperature(), 1e-9);
}

TEST(ClimateControl, AdjustTemperatureGradually_Valid) {
    ClimateControl climate(20.0, true, true);
    climate.adjustTemperatureGradually(25.0, 5);
    EXPECT_TRUE(climate.isOn());
}

TEST(SecuritySystem, ActivateZone) {
    SecuritySystem security(10, 5, false, true);
    security.activateZone();
}

TEST(SecuritySystem, ActivateZone_MaxZones) {
    SecuritySystem security(10, 10, false, true);
    security.activateZone();
}

TEST(PaintingExhibition, Organize_NoCurator_Throws) {
    Artwork p1 = makeArtwork("painting1", 100.0);
    Artwork p2 = makeArtwork("painting2", 100.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    PaintingExhibition ex("Painting", &p1, &p2, &f, &l, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(SculptureExhibition, Organize_NoCurator_Throws) {
    Artwork s = makeArtwork("sculpture", 100.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SculptureExhibition ex("Sculpture", &s, &stand, &l, &f, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(PhotographyExhibition, Organize_NoCurator_Throws) {
    Artwork ph1 = makeArtwork("photo1", 100.0);
    Artwork ph2 = makeArtwork("photo2", 100.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    PhotographyExhibition ex("Photography", &ph1, &ph2, &f, &l, &cam, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(MixedMediaExhibition, Organize_NoCurator_Throws) {
    Artwork a1 = makeArtwork("art1", 100.0);
    Artwork a2 = makeArtwork("art2", 100.0);
    Artwork a3 = makeArtwork("art3", 100.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    RestorationKit rk("RestorationKit", false);
    MixedMediaExhibition ex("MixedMedia", &a1, &a2, &a3, &stand, &l, &f, &t, &rk, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(DigitalArtExhibition, Organize_NoCurator_Throws) {
    Artwork d1 = makeArtwork("digital1", 100.0);
    Artwork d2 = makeArtwork("digital2", 100.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    DigitalArtExhibition ex("DigitalArt", &d1, &d2, &stand, &l, &cam, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(VintageExhibition, Organize_NoCurator_Throws) {
    Artwork v1 = makeArtwork("vintage1", 100.0);
    Artwork v2 = makeArtwork("vintage2", 100.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    RestorationKit rk("RestorationKit", false);
    VintageExhibition ex("Vintage", &v1, &v2, &f, &cc, &rk, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(ContemporaryExhibition, Organize_NoCurator_Throws) {
    Artwork c1 = makeArtwork("contemporary1", 100.0);
    Artwork c2 = makeArtwork("contemporary2", 100.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    ContemporaryExhibition ex("Contemporary", &c1, &c2, &stand, &l, &cam, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(MinimalistExhibition, Organize_NoCurator_Throws) {
    Artwork m = makeArtwork("minimalist", 100.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    MinimalistExhibition ex("Minimalist", &m, &f, &l, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(AbstractExhibition, Organize_NoCurator_Throws) {
    Artwork ab1 = makeArtwork("abstract1", 100.0);
    Artwork ab2 = makeArtwork("abstract2", 100.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    AbstractExhibition ex("Abstract", &ab1, &ab2, &stand, &l, &f, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(PortraitExhibition, Organize_NoCurator_Throws) {
    Artwork p1 = makeArtwork("portrait1", 100.0);
    Artwork p2 = makeArtwork("portrait2", 100.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    PortraitExhibition ex("Portrait", &p1, &p2, &f, &l, &cam, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(LandscapeExhibition, Organize_NoCurator_Throws) {
    Artwork l1 = makeArtwork("landscape1", 100.0);
    Artwork l2 = makeArtwork("landscape2", 100.0);
    Frame f("Frame", true, 50, 1);
    Lighting light("Lighting", true, false, 1);
    Timer t;
    LandscapeExhibition ex("Landscape", &l1, &l2, &f, &light, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(RestorationExhibition, Organize_NoCurator_Throws) {
    Artwork r = makeArtwork("restoration", 100.0);
    RestorationKit rk("RestorationKit", false);
    CleaningKit ck("CleaningKit", 1);
    ClimateControl cc(20.0, true, true);
    Timer t;
    RestorationExhibition ex("Restoration", &r, &rk, &ck, &cc, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(TemporaryExhibition, Organize_NoCurator_Throws) {
    Artwork t1 = makeArtwork("temporary1", 100.0);
    Artwork t2 = makeArtwork("temporary2", 100.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer timer;
    TemporaryExhibition ex("Temporary", &t1, &t2, &stand, &l, &cam, &timer, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(PermanentExhibition, Organize_NoCurator_Throws) {
    Artwork p1 = makeArtwork("permanent1", 100.0);
    Artwork p2 = makeArtwork("permanent2", 100.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    SecuritySystem sec(10, 5, false, true);
    Timer t;
    PermanentExhibition ex("Permanent", &p1, &p2, &f, &cc, &sec, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(InteractiveExhibition, Organize_NoCurator_Throws) {
    Artwork i1 = makeArtwork("interactive1", 100.0);
    Artwork i2 = makeArtwork("interactive2", 100.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    InteractiveExhibition ex("Interactive", &i1, &i2, &stand, &l, &cam, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(ThematicExhibition, Organize_NoCurator_Throws) {
    Artwork th1 = makeArtwork("theme1", 100.0);
    Artwork th2 = makeArtwork("theme2", 100.0);
    Artwork th3 = makeArtwork("theme3", 100.0);
    Frame f("Frame", true, 50, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    ThematicExhibition ex("Thematic", &th1, &th2, &th3, &f, &stand, &l, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(RetrospectiveExhibition, Organize_NoCurator_Throws) {
    Artwork r1 = makeArtwork("retro1", 100.0);
    Artwork r2 = makeArtwork("retro2", 100.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    RestorationKit rk("RestorationKit", false);
    Timer t;
    RetrospectiveExhibition ex("Retrospective", &r1, &r2, &f, &cc, &rk, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(GroupExhibition, Organize_NoCurator_Throws) {
    Artwork g1 = makeArtwork("group1", 100.0);
    Artwork g2 = makeArtwork("group2", 100.0);
    Artwork g3 = makeArtwork("group3", 100.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    GroupExhibition ex("Group", &g1, &g2, &g3, &stand, &l, &f, &cam, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(SoloExhibition, Organize_NoCurator_Throws) {
    Artwork s1 = makeArtwork("solo1", 100.0);
    Artwork s2 = makeArtwork("solo2", 100.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Timer t;
    SoloExhibition ex("Solo", &s1, &s2, &f, &l, &stand, &t, nullptr);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(SecuritySystem, CalculateSecurityCoverage_ZeroZones) {
    SecuritySystem security(0, 0, false, true);
    EXPECT_NEAR(0.0, security.calculateSecurityCoverage(), 1e-9);
}

TEST(ClimateControl, SetConditions_WarmupZero_Throws) {
    ClimateControl climate(0.0, false, true);
    EXPECT_THROW(climate.setConditions(20.0, 0), InvalidConditionException);
}

TEST(ClimateControl, SetConditions_WarmupNegative_Throws) {
    ClimateControl climate(0.0, false, true);
    EXPECT_THROW(climate.setConditions(20.0, -10), InvalidConditionException);
}

TEST(ClimateControl, AdjustTemperatureGradually_DurationZero) {
    ClimateControl climate(20.0, true, true);
    climate.adjustTemperatureGradually(25.0, 0);
}

TEST(Frame, CalculateComfortScore_MaxScore) {
    Frame frame("Test", true, 100, 1);
    double score = frame.calculateComfortScore();
    EXPECT_LE(score, 10.0);
}

TEST(Lighting, CalculatePowerConsumption_LED_Dimmed) {
    Lighting light("Test", true, true, 1);
    EXPECT_NEAR(5.0, light.calculatePowerConsumption(), 1e-9);
}

TEST(Lighting, CalculatePowerConsumption_LED_Bright) {
    Lighting light("Test", true, false, 1);
    EXPECT_NEAR(10.0, light.calculatePowerConsumption(), 1e-9);
}

TEST(DisplayStand, CalculateDisplayTime_ZeroVisitors) {
    DisplayStand stand("Test", 5.0, true, false);
    EXPECT_NEAR(0.0, stand.calculateDisplayTime(0), 1e-9);
}

TEST(DisplayStand, CalculateDisplayTime_NegativeVisitors) {
    DisplayStand stand("Test", 5.0, true, false);
    EXPECT_NEAR(0.0, stand.calculateDisplayTime(-10), 1e-9);
}

TEST(DisplayStand, CanHandleVisitorLoad_Zero) {
    DisplayStand stand("Test", 5.0, true, false);
    EXPECT_TRUE(stand.canHandleVisitorLoad(0));
}

TEST(DisplayStand, CanHandleVisitorLoad_TooMany) {
    DisplayStand stand("Test", 5.0, true, false);
    EXPECT_FALSE(stand.canHandleVisitorLoad(100));
}

TEST(Timer, Resume_NotPaused) {
    Timer timer(0, false, 0, 1);
    timer.start(100);
    timer.resume();
    EXPECT_TRUE(timer.isRunning());
}

TEST(ConditionProfile, CurrentTemp_AfterDuration) {
    ConditionProfile profile(20.0, 25.0, 100, true);
    EXPECT_NEAR(25.0, profile.currentTemp(200), 1e-9);
}

TEST(ConditionProfile, CurrentTemp_JumpMode) {
    ConditionProfile profile(20.0, 25.0, 100, false);
    EXPECT_NEAR(25.0, profile.currentTemp(0), 1e-9);
    EXPECT_NEAR(25.0, profile.currentTemp(50), 1e-9);
}

TEST(ClimateControl, Tick_TimerNotFinished) {
    ClimateControl climate(0.0, false, true);
    climate.setConditions(20.0, 1);
    climate.setTimerMinutes(10);
    climate.tick(60);
    EXPECT_TRUE(climate.isOn());
}

TEST(Artwork, CalculateMaintenanceCost_WithYear) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    double cost = a.calculateMaintenanceCost();
    EXPECT_GT(cost, 0.0);
}

TEST(Artwork, CalculateDepreciation_CurrentYear) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    double dep = a.calculateDepreciation(2025);
    EXPECT_GT(dep, 0.0);
}

TEST(Size, Scale_Negative) {
    Size s(100.0, &cmUnit);
    s.scale(-1.0);
    EXPECT_LT(s.toCentimeters(), 0.0);
}

TEST(Dimension, FromCentimeters_Negative) {
    Dimension d("cm", 1.0, true, 1);
    EXPECT_NEAR(-10.0, d.fromCentimeters(-10.0), 1e-9);
}

TEST(Artwork, AddSize_Negative) {
    Size s(100.0, &cmUnit);
    Artwork a("Test", s, 1000.0, false);
    a.addSize(-50.0);
}

TEST(GalleryTool, UseTool_MultipleTimes) {
    GalleryTool tool("Test", true, true, 3);
    tool.useTool();
    EXPECT_TRUE(tool.isAvailable());
    tool.useTool();
    EXPECT_TRUE(tool.isAvailable());
    tool.useTool();
    EXPECT_FALSE(tool.isAvailable());
}

TEST(Frame, IsSuitableForArtwork_ExactWeight) {
    Frame frame("Test", true, 50, 1);
    EXPECT_TRUE(frame.isSuitableForArtwork(150.0));
}

TEST(SecurityCamera, CalculateCoverageArea_LargeDistance) {
    SecurityCamera cam("Test", 90.0, false, false);
    double area = cam.calculateCoverageArea(100.0);
    EXPECT_GT(area, 0.0);
}

TEST(SecurityCamera, CanMonitorArea_ExactSize) {
    SecurityCamera cam("Test", 90.0, false, true);
    double maxArea = cam.calculateCoverageArea(50.0);
    EXPECT_TRUE(cam.canMonitorArea(maxArea));
}

TEST(DisplayStand, CalculateUtilization_InUse) {
    DisplayStand stand("Test", 5.0, true, true);
    stand.startDisplay();
    double util = stand.calculateUtilization();
    EXPECT_GT(util, 0.0);
}

TEST(RestorationKit, CalculateRestorationEfficiency_NotEquipped) {
    RestorationKit kit("Test", false);
    EXPECT_NEAR(0.0, kit.calculateRestorationEfficiency(), 1e-9);
}

TEST(CleaningKit, Clean_MultipleTimes) {
    CleaningKit kit("Test", 1);
    kit.clean();
    kit.clean();
}

TEST(ConditionProfile, Reset_NewValues) {
    ConditionProfile profile(20.0, 25.0, 100, true);
    profile.reset(30.0, 15.0, 200);
    EXPECT_NEAR(30.0, profile.currentTemp(0), 1e-9);
}

TEST(ClimateControl, GetHumidity_Default) {
    ClimateControl climate(20.0, true, true);
    EXPECT_NEAR(50.0, climate.getHumidity(), 1e-9);
}

TEST(ClimateControl, SetHumidity_Change) {
    ClimateControl climate(20.0, true, true);
    climate.setHumidity(60.0);
    EXPECT_NEAR(60.0, climate.getHumidity(), 1e-9);
}

TEST(SecuritySystem, FreeZones_Calculation) {
    SecuritySystem security(10, 3, false, true);
    EXPECT_EQ(7, security.freeZones());
}

TEST(SecuritySystem, FreeZones_AllActive) {
    SecuritySystem security(10, 10, false, true);
    EXPECT_EQ(0, security.freeZones());
}

TEST(SecuritySystem, CheckSecurityBreach_AllZonesActive) {
    SecuritySystem security(10, 10, false, true);
    EXPECT_FALSE(security.checkSecurityBreach());
}

TEST(SecuritySystem, CheckSecurityBreach_Off) {
    SecuritySystem security(10, 5, false, false);
    EXPECT_FALSE(security.checkSecurityBreach());
}

TEST(SecuritySystem, CalculateSecurityCoverage_Partial) {
    SecuritySystem security(10, 7, false, true);
    EXPECT_NEAR(70.0, security.calculateSecurityCoverage(), 1e-9);
}

TEST(SecuritySystem, CalculateSecurityCoverage_Full) {
    SecuritySystem security(10, 10, false, true);
    EXPECT_NEAR(100.0, security.calculateSecurityCoverage(), 1e-9);
}


TEST(TicketSystem, CreateBooking_MaxBookings) {
    TicketSystem system("Test", 1);
    Visitor v("Test", 2, true);
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    system.createBooking(&v, &ex);
    EXPECT_FALSE(system.createBooking(&v, &ex));
}

TEST(TicketSystem, CreateBooking_NullVisitor) {
    TicketSystem system;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_FALSE(system.createBooking(nullptr, &ex));
}

TEST(TicketSystem, CreateBooking_NullExhibition) {
    TicketSystem system;
    Visitor v("Test", 2, true);
    EXPECT_FALSE(system.createBooking(&v, nullptr));
}

TEST(TicketSystem, CancelBooking_InvalidId) {
    TicketSystem system;
    EXPECT_FALSE(system.cancelBooking(-1));
    EXPECT_FALSE(system.cancelBooking(100));
}

TEST(TicketSystem, CancelBooking_ValidId) {
    TicketSystem system;
    Visitor v("Test", 2, true);
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    system.createBooking(&v, &ex);
    EXPECT_TRUE(system.cancelBooking(0));
}

TEST(TicketSystem, CalculateTicketPrice_NullExhibition) {
    TicketSystem system;
    Visitor v("Test", 2, true);
    EXPECT_NEAR(0.0, system.calculateTicketPrice(nullptr, &v), 1e-9);
}

TEST(TicketSystem, CalculateTicketPrice_NullVisitor) {
    TicketSystem system;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_NEAR(0.0, system.calculateTicketPrice(&ex, nullptr), 1e-9);
}

TEST(TicketSystem, ProcessPayment_InvalidCard) {
    TicketSystem system;
    EXPECT_FALSE(system.processPayment(100.0, "123"));
    EXPECT_FALSE(system.processPayment(100.0, "123456789012"));
}

TEST(TicketSystem, ProcessPayment_ValidCard) {
    TicketSystem system;
    EXPECT_TRUE(system.processPayment(100.0, "1234567890123"));
    EXPECT_NEAR(100.0, system.getTotalRevenue(), 1e-9);
}

TEST(TicketSystem, TransferMoney_InvalidCards) {
    TicketSystem system;
    EXPECT_FALSE(system.transferMoney("123", "456", 100.0));
    EXPECT_FALSE(system.transferMoney("1234567890123", "456", 100.0));
    EXPECT_FALSE(system.transferMoney("123", "1234567890123", 100.0));
}

TEST(TicketSystem, TransferMoney_NullCards) {
    TicketSystem system;
    EXPECT_FALSE(system.transferMoney(nullptr, "1234567890123", 100.0));
    EXPECT_FALSE(system.transferMoney("1234567890123", nullptr, 100.0));
}

TEST(TicketSystem, TransferMoney_InvalidAmount) {
    TicketSystem system;
    EXPECT_FALSE(system.transferMoney("1234567890123", "9876543210987", 0.0));
    EXPECT_FALSE(system.transferMoney("1234567890123", "9876543210987", -10.0));
}

TEST(TicketSystem, VerifyPassword_NullInputs) {
    TicketSystem system;
    EXPECT_FALSE(system.verifyPassword(nullptr, "password"));
    EXPECT_FALSE(system.verifyPassword("account", nullptr));
}

TEST(TicketSystem, VerifyPassword_ShortPassword) {
    TicketSystem system;
    EXPECT_FALSE(system.verifyPassword("account1", "12345"));
}

TEST(TicketSystem, VerifyPassword_ValidPassword) {
    TicketSystem system;
    EXPECT_TRUE(system.verifyPassword("account1", "123456"));
    EXPECT_TRUE(system.verifyPassword("account1", "password123"));
}

TEST(TicketSystem, CheckExhibitionAvailability_Null) {
    TicketSystem system;
    EXPECT_FALSE(system.checkExhibitionAvailability(nullptr));
}

TEST(TicketSystem, CheckExhibitionAvailability_Available) {
    TicketSystem system;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_TRUE(system.checkExhibitionAvailability(&ex));
}

TEST(TicketSystem, ReserveSeat_InvalidSeat) {
    TicketSystem system;
    Visitor v("Test", 2, true);
    EXPECT_FALSE(system.reserveSeat(&v, 0));
    EXPECT_FALSE(system.reserveSeat(&v, 501));
    EXPECT_FALSE(system.reserveSeat(nullptr, 10));
}

TEST(TicketSystem, ReserveSeat_ValidSeat) {
    TicketSystem system;
    Visitor v("Test", 2, true);
    EXPECT_TRUE(system.reserveSeat(&v, 1));
    EXPECT_TRUE(system.reserveSeat(&v, 250));
    EXPECT_TRUE(system.reserveSeat(&v, 500));
}

TEST(TicketSystem, CancelSeatReservation_InvalidSeat) {
    TicketSystem system;
    EXPECT_FALSE(system.cancelSeatReservation(0));
    EXPECT_FALSE(system.cancelSeatReservation(501));
}

TEST(TicketSystem, CancelSeatReservation_ValidSeat) {
    TicketSystem system;
    EXPECT_TRUE(system.cancelSeatReservation(1));
    EXPECT_TRUE(system.cancelSeatReservation(250));
    EXPECT_TRUE(system.cancelSeatReservation(500));
}

TEST(TicketSystem, CalculateBaggageFee_ZeroWeight) {
    TicketSystem system;
    EXPECT_NEAR(0.0, system.calculateBaggageFee(0.0), 1e-9);
}

TEST(TicketSystem, CalculateBaggageFee_NegativeWeight) {
    TicketSystem system;
    EXPECT_NEAR(0.0, system.calculateBaggageFee(-10.0), 1e-9);
}

TEST(TicketSystem, CalculateBaggageFee_UnderLimit) {
    TicketSystem system;
    EXPECT_NEAR(0.0, system.calculateBaggageFee(23.0), 1e-9);
}

TEST(TicketSystem, CalculateBaggageFee_OverLimit) {
    TicketSystem system;
    EXPECT_NEAR(20.0, system.calculateBaggageFee(25.0), 1e-9);
    EXPECT_NEAR(50.0, system.calculateBaggageFee(28.0), 1e-9);
}

TEST(TicketSystem, CheckVisaRequirement_NullInputs) {
    TicketSystem system;
    Visitor v("Test", 2, true);
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_FALSE(system.checkVisaRequirement(nullptr, &ex));
    EXPECT_FALSE(system.checkVisaRequirement(&v, nullptr));
}

TEST(TicketSystem, GetTotalRevenue) {
    TicketSystem system;
    EXPECT_NEAR(0.0, system.getTotalRevenue(), 1e-9);
    system.processPayment(100.0, "1234567890123");
    EXPECT_NEAR(100.0, system.getTotalRevenue(), 1e-9);
    system.processPayment(50.0, "1234567890123");
    EXPECT_NEAR(150.0, system.getTotalRevenue(), 1e-9);
}

TEST(TicketSystem, GetTotalBookings) {
    TicketSystem system;
    EXPECT_EQ(0, system.getTotalBookings());
    Visitor v("Test", 2, true);
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test", &a1, &a2, &frame, &lighting, &timer, &c);
    system.createBooking(&v, &ex);
    EXPECT_EQ(1, system.getTotalBookings());
}

TEST(TicketSystem, IsSystemActive) {
    TicketSystem system;
    EXPECT_TRUE(system.isSystemActive());
}

TEST(GalleryMenu, SearchExhibitions_NoMatch) {
    GalleryMenu menu;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    menu.addExhibition(&ex);
    EXPECT_EQ(0, menu.searchExhibitions("NonExistent"));
}

TEST(GalleryMenu, SearchExhibitions_Match) {
    GalleryMenu menu;
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    menu.addExhibition(&ex);
    EXPECT_EQ(1, menu.searchExhibitions("Test"));
}

TEST(GalleryMenu, GetExhibitionByIndex_Negative) {
    GalleryMenu menu;
    EXPECT_EQ(nullptr, menu.getExhibitionByIndex(-1));
}

TEST(Exhibition, GetName) {
    Artwork a1 = makeArtwork("A1", 100.0);
    Artwork a2 = makeArtwork("A2", 100.0);
    Frame frame("Frame", true, 50, 1);
    Lighting lighting("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    PaintingExhibition ex("Test Exhibition", &a1, &a2, &frame, &lighting, &timer, &c);
    EXPECT_STREQ("Test Exhibition", ex.getName());
}

TEST(Curator, OrganizeSculpture_StandTooSmall) {
    Artwork s = makeArtwork("sculpture", 1000.0);
    DisplayStand stand("Stand", 1.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Curator c("Curator");
    SculptureExhibition ex("Sculpture", &s, &stand, &l, &f, &c);
    EXPECT_THROW(ex.organize(), InsufficientSpaceException);
}

TEST(Curator, OrganizeSculpture_NoStand) {
    Artwork s = makeArtwork("sculpture", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Curator c("Curator");
    SculptureExhibition ex("Sculpture", &s, nullptr, &l, &f, &c);
    EXPECT_THROW(ex.organize(), InsufficientSpaceException);
}

TEST(Curator, OrganizePhotography_NoFrame) {
    Artwork ph1 = makeArtwork("photo1", 1000.0);
    Artwork ph2 = makeArtwork("photo2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    PhotographyExhibition ex("Photography", &ph1, &ph2, nullptr, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizePhotography_NoLighting) {
    Artwork ph1 = makeArtwork("photo1", 1000.0);
    Artwork ph2 = makeArtwork("photo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    PhotographyExhibition ex("Photography", &ph1, &ph2, &f, nullptr, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizePhotography_NoCamera) {
    Artwork ph1 = makeArtwork("photo1", 1000.0);
    Artwork ph2 = makeArtwork("photo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    PhotographyExhibition ex("Photography", &ph1, &ph2, &f, &l, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizePhotography_NoTimer) {
    Artwork ph1 = makeArtwork("photo1", 1000.0);
    Artwork ph2 = makeArtwork("photo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    PhotographyExhibition ex("Photography", &ph1, &ph2, &f, &l, &cam, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizeMixedMedia_NoArtwork1) {
    Artwork a2 = makeArtwork("art2", 1000.0);
    Artwork a3 = makeArtwork("art3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    MixedMediaExhibition ex("MixedMedia", nullptr, &a2, &a3, &stand, &l, &f, &t, &rk, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeMixedMedia_NoArtwork2) {
    Artwork a1 = makeArtwork("art1", 1000.0);
    Artwork a3 = makeArtwork("art3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    MixedMediaExhibition ex("MixedMedia", &a1, nullptr, &a3, &stand, &l, &f, &t, &rk, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeMixedMedia_NoArtwork3) {
    Artwork a1 = makeArtwork("art1", 1000.0);
    Artwork a2 = makeArtwork("art2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    MixedMediaExhibition ex("MixedMedia", &a1, &a2, nullptr, &stand, &l, &f, &t, &rk, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeMixedMedia_NoStand) {
    Artwork a1 = makeArtwork("art1", 1000.0);
    Artwork a2 = makeArtwork("art2", 1000.0);
    Artwork a3 = makeArtwork("art3", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    MixedMediaExhibition ex("MixedMedia", &a1, &a2, &a3, nullptr, &l, &f, &t, &rk, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeMixedMedia_NoRestorationKit) {
    Artwork a1 = makeArtwork("art1", 1000.0);
    Artwork a2 = makeArtwork("art2", 1000.0);
    Artwork a3 = makeArtwork("art3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    Curator c("Curator");
    MixedMediaExhibition ex("MixedMedia", &a1, &a2, &a3, &stand, &l, &f, &t, nullptr, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeDigitalArt_NoArtwork1) {
    Artwork d2 = makeArtwork("digital2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    DigitalArtExhibition ex("DigitalArt", nullptr, &d2, &stand, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeDigitalArt_NoArtwork2) {
    Artwork d1 = makeArtwork("digital1", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    DigitalArtExhibition ex("DigitalArt", &d1, nullptr, &stand, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeDigitalArt_NoStand) {
    Artwork d1 = makeArtwork("digital1", 1000.0);
    Artwork d2 = makeArtwork("digital2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    DigitalArtExhibition ex("DigitalArt", &d1, &d2, nullptr, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeDigitalArt_NoCamera) {
    Artwork d1 = makeArtwork("digital1", 1000.0);
    Artwork d2 = makeArtwork("digital2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    DigitalArtExhibition ex("DigitalArt", &d1, &d2, &stand, &l, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeVintage_NoClimate) {
    Artwork v1 = makeArtwork("vintage1", 1000.0);
    Artwork v2 = makeArtwork("vintage2", 1000.0);
    Frame f("Frame", true, 50, 1);
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    VintageExhibition ex("Vintage", &v1, &v2, &f, nullptr, &rk, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeContemporary_NoArtwork1) {
    Artwork c2 = makeArtwork("contemporary2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator cu("Curator");
    ContemporaryExhibition ex("Contemporary", nullptr, &c2, &stand, &l, &cam, &t, &cu);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeContemporary_NoArtwork2) {
    Artwork c1 = makeArtwork("contemporary1", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator cu("Curator");
    ContemporaryExhibition ex("Contemporary", &c1, nullptr, &stand, &l, &cam, &t, &cu);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeContemporary_NoStand) {
    Artwork c1 = makeArtwork("contemporary1", 1000.0);
    Artwork c2 = makeArtwork("contemporary2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator cu("Curator");
    ContemporaryExhibition ex("Contemporary", &c1, &c2, nullptr, &l, &cam, &t, &cu);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeContemporary_NoCamera) {
    Artwork c1 = makeArtwork("contemporary1", 1000.0);
    Artwork c2 = makeArtwork("contemporary2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator cu("Curator");
    ContemporaryExhibition ex("Contemporary", &c1, &c2, &stand, &l, nullptr, &t, &cu);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeMinimalist_NoFrame) {
    Artwork m = makeArtwork("minimalist", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    MinimalistExhibition ex("Minimalist", &m, nullptr, &l, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeMinimalist_FrameNoProtection) {
    Artwork m = makeArtwork("minimalist", 1000.0);
    Frame f("Frame", false, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    MinimalistExhibition ex("Minimalist", &m, &f, &l, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeMinimalist_NoLighting) {
    Artwork m = makeArtwork("minimalist", 1000.0);
    Frame f("Frame", true, 50, 1);
    Curator c("Curator");
    MinimalistExhibition ex("Minimalist", &m, &f, nullptr, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeAbstract_NoArtwork1) {
    Artwork ab2 = makeArtwork("abstract2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    Curator c("Curator");
    AbstractExhibition ex("Abstract", nullptr, &ab2, &stand, &l, &f, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeAbstract_NoArtwork2) {
    Artwork ab1 = makeArtwork("abstract1", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    Curator c("Curator");
    AbstractExhibition ex("Abstract", &ab1, nullptr, &stand, &l, &f, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeAbstract_NoStand) {
    Artwork ab1 = makeArtwork("abstract1", 1000.0);
    Artwork ab2 = makeArtwork("abstract2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    Curator c("Curator");
    AbstractExhibition ex("Abstract", &ab1, &ab2, nullptr, &l, &f, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeAbstract_NoTimer) {
    Artwork ab1 = makeArtwork("abstract1", 1000.0);
    Artwork ab2 = makeArtwork("abstract2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Curator c("Curator");
    AbstractExhibition ex("Abstract", &ab1, &ab2, &stand, &l, &f, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizePortrait_NoArtwork1) {
    Artwork p2 = makeArtwork("portrait2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    PortraitExhibition ex("Portrait", nullptr, &p2, &f, &l, &cam, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizePortrait_NoArtwork2) {
    Artwork p1 = makeArtwork("portrait1", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    PortraitExhibition ex("Portrait", &p1, nullptr, &f, &l, &cam, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizePortrait_NoFrame) {
    Artwork p1 = makeArtwork("portrait1", 1000.0);
    Artwork p2 = makeArtwork("portrait2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    PortraitExhibition ex("Portrait", &p1, &p2, nullptr, &l, &cam, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizePortrait_NoCamera) {
    Artwork p1 = makeArtwork("portrait1", 1000.0);
    Artwork p2 = makeArtwork("portrait2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    PortraitExhibition ex("Portrait", &p1, &p2, &f, &l, nullptr, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeLandscape_NoArtwork1) {
    Artwork l2 = makeArtwork("landscape2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting light("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    LandscapeExhibition ex("Landscape", nullptr, &l2, &f, &light, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeLandscape_NoArtwork2) {
    Artwork l1 = makeArtwork("landscape1", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting light("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    LandscapeExhibition ex("Landscape", &l1, nullptr, &f, &light, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeLandscape_NoTimer) {
    Artwork l1 = makeArtwork("landscape1", 1000.0);
    Artwork l2 = makeArtwork("landscape2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting light("Lighting", true, false, 1);
    Curator c("Curator");
    LandscapeExhibition ex("Landscape", &l1, &l2, &f, &light, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizeRestoration_NoArtwork) {
    RestorationKit rk("RestorationKit", false);
    CleaningKit ck("CleaningKit", 1);
    ClimateControl cc(20.0, true, true);
    Timer t;
    Curator cu("Curator");
    RestorationExhibition ex("Restoration", nullptr, &rk, &ck, &cc, &t, &cu);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeRestoration_NoRestorationKit) {
    Artwork r = makeArtwork("restoration", 1000.0);
    CleaningKit ck("CleaningKit", 1);
    ClimateControl cc(20.0, true, true);
    Timer t;
    Curator cu("Curator");
    RestorationExhibition ex("Restoration", &r, nullptr, &ck, &cc, &t, &cu);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeRestoration_NoCleaningKit) {
    Artwork r = makeArtwork("restoration", 1000.0);
    RestorationKit rk("RestorationKit", false);
    ClimateControl cc(20.0, true, true);
    Timer t;
    Curator cu("Curator");
    RestorationExhibition ex("Restoration", &r, &rk, nullptr, &cc, &t, &cu);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeRestoration_NoTimer) {
    Artwork r = makeArtwork("restoration", 1000.0);
    RestorationKit rk("RestorationKit", false);
    CleaningKit ck("CleaningKit", 1);
    ClimateControl cc(20.0, true, true);
    Curator cu("Curator");
    RestorationExhibition ex("Restoration", &r, &rk, &ck, &cc, nullptr, &cu);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizeTemporary_NoArtwork1) {
    Artwork t2 = makeArtwork("temporary2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer timer;
    Curator c("Curator");
    TemporaryExhibition ex("Temporary", nullptr, &t2, &stand, &l, &cam, &timer, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeTemporary_NoArtwork2) {
    Artwork t1 = makeArtwork("temporary1", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer timer;
    Curator c("Curator");
    TemporaryExhibition ex("Temporary", &t1, nullptr, &stand, &l, &cam, &timer, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeTemporary_NoStand) {
    Artwork t1 = makeArtwork("temporary1", 1000.0);
    Artwork t2 = makeArtwork("temporary2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer timer;
    Curator c("Curator");
    TemporaryExhibition ex("Temporary", &t1, &t2, nullptr, &l, &cam, &timer, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeTemporary_NoCamera) {
    Artwork t1 = makeArtwork("temporary1", 1000.0);
    Artwork t2 = makeArtwork("temporary2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer timer;
    Curator c("Curator");
    TemporaryExhibition ex("Temporary", &t1, &t2, &stand, &l, nullptr, &timer, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeTemporary_NoTimer) {
    Artwork t1 = makeArtwork("temporary1", 1000.0);
    Artwork t2 = makeArtwork("temporary2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    TemporaryExhibition ex("Temporary", &t1, &t2, &stand, &l, &cam, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizePermanent_NoArtwork1) {
    Artwork p2 = makeArtwork("permanent2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    SecuritySystem sec(10, 5, false, true);
    Timer t;
    Curator c("Curator");
    PermanentExhibition ex("Permanent", nullptr, &p2, &f, &cc, &sec, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizePermanent_NoArtwork2) {
    Artwork p1 = makeArtwork("permanent1", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    SecuritySystem sec(10, 5, false, true);
    Timer t;
    Curator c("Curator");
    PermanentExhibition ex("Permanent", &p1, nullptr, &f, &cc, &sec, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizePermanent_NoClimate) {
    Artwork p1 = makeArtwork("permanent1", 1000.0);
    Artwork p2 = makeArtwork("permanent2", 1000.0);
    Frame f("Frame", true, 50, 1);
    SecuritySystem sec(10, 5, false, true);
    Timer t;
    Curator c("Curator");
    PermanentExhibition ex("Permanent", &p1, &p2, &f, nullptr, &sec, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizePermanent_NoSecurity) {
    Artwork p1 = makeArtwork("permanent1", 1000.0);
    Artwork p2 = makeArtwork("permanent2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    Timer t;
    Curator c("Curator");
    PermanentExhibition ex("Permanent", &p1, &p2, &f, &cc, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizePermanent_NoTimer) {
    Artwork p1 = makeArtwork("permanent1", 1000.0);
    Artwork p2 = makeArtwork("permanent2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    SecuritySystem sec(10, 5, false, true);
    Curator c("Curator");
    PermanentExhibition ex("Permanent", &p1, &p2, &f, &cc, &sec, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizeInteractive_NoArtwork1) {
    Artwork i2 = makeArtwork("interactive2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    InteractiveExhibition ex("Interactive", nullptr, &i2, &stand, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeInteractive_NoArtwork2) {
    Artwork i1 = makeArtwork("interactive1", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    InteractiveExhibition ex("Interactive", &i1, nullptr, &stand, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeInteractive_NoStand) {
    Artwork i1 = makeArtwork("interactive1", 1000.0);
    Artwork i2 = makeArtwork("interactive2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    InteractiveExhibition ex("Interactive", &i1, &i2, nullptr, &l, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeInteractive_NoCamera) {
    Artwork i1 = makeArtwork("interactive1", 1000.0);
    Artwork i2 = makeArtwork("interactive2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    InteractiveExhibition ex("Interactive", &i1, &i2, &stand, &l, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeInteractive_NoTimer) {
    Artwork i1 = makeArtwork("interactive1", 1000.0);
    Artwork i2 = makeArtwork("interactive2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    InteractiveExhibition ex("Interactive", &i1, &i2, &stand, &l, &cam, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizeThematic_NoArtwork1) {
    Artwork th2 = makeArtwork("theme2", 1000.0);
    Artwork th3 = makeArtwork("theme3", 1000.0);
    Frame f("Frame", true, 50, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    ThematicExhibition ex("Thematic", nullptr, &th2, &th3, &f, &stand, &l, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeThematic_NoArtwork2) {
    Artwork th1 = makeArtwork("theme1", 1000.0);
    Artwork th3 = makeArtwork("theme3", 1000.0);
    Frame f("Frame", true, 50, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    ThematicExhibition ex("Thematic", &th1, nullptr, &th3, &f, &stand, &l, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeThematic_NoArtwork3) {
    Artwork th1 = makeArtwork("theme1", 1000.0);
    Artwork th2 = makeArtwork("theme2", 1000.0);
    Frame f("Frame", true, 50, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    ThematicExhibition ex("Thematic", &th1, &th2, nullptr, &f, &stand, &l, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeThematic_NoStand) {
    Artwork th1 = makeArtwork("theme1", 1000.0);
    Artwork th2 = makeArtwork("theme2", 1000.0);
    Artwork th3 = makeArtwork("theme3", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    ThematicExhibition ex("Thematic", &th1, &th2, &th3, &f, nullptr, &l, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeThematic_NoTimer) {
    Artwork th1 = makeArtwork("theme1", 1000.0);
    Artwork th2 = makeArtwork("theme2", 1000.0);
    Artwork th3 = makeArtwork("theme3", 1000.0);
    Frame f("Frame", true, 50, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    ThematicExhibition ex("Thematic", &th1, &th2, &th3, &f, &stand, &l, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizeRetrospective_NoArtwork1) {
    Artwork r2 = makeArtwork("retro2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    RestorationKit rk("RestorationKit", false);
    Timer t;
    Curator c("Curator");
    RetrospectiveExhibition ex("Retrospective", nullptr, &r2, &f, &cc, &rk, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeRetrospective_NoArtwork2) {
    Artwork r1 = makeArtwork("retro1", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    RestorationKit rk("RestorationKit", false);
    Timer t;
    Curator c("Curator");
    RetrospectiveExhibition ex("Retrospective", &r1, nullptr, &f, &cc, &rk, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeRetrospective_NoClimate) {
    Artwork r1 = makeArtwork("retro1", 1000.0);
    Artwork r2 = makeArtwork("retro2", 1000.0);
    Frame f("Frame", true, 50, 1);
    RestorationKit rk("RestorationKit", false);
    Timer t;
    Curator c("Curator");
    RetrospectiveExhibition ex("Retrospective", &r1, &r2, &f, nullptr, &rk, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeRetrospective_NoRestorationKit) {
    Artwork r1 = makeArtwork("retro1", 1000.0);
    Artwork r2 = makeArtwork("retro2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    Timer t;
    Curator c("Curator");
    RetrospectiveExhibition ex("Retrospective", &r1, &r2, &f, &cc, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeRetrospective_NoTimer) {
    Artwork r1 = makeArtwork("retro1", 1000.0);
    Artwork r2 = makeArtwork("retro2", 1000.0);
    Frame f("Frame", true, 50, 1);
    ClimateControl cc(20.0, true, true);
    RestorationKit rk("RestorationKit", false);
    Curator c("Curator");
    RetrospectiveExhibition ex("Retrospective", &r1, &r2, &f, &cc, &rk, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizeGroup_NoArtwork1) {
    Artwork g2 = makeArtwork("group2", 1000.0);
    Artwork g3 = makeArtwork("group3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    GroupExhibition ex("Group", nullptr, &g2, &g3, &stand, &l, &f, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeGroup_NoArtwork2) {
    Artwork g1 = makeArtwork("group1", 1000.0);
    Artwork g3 = makeArtwork("group3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    GroupExhibition ex("Group", &g1, nullptr, &g3, &stand, &l, &f, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeGroup_NoArtwork3) {
    Artwork g1 = makeArtwork("group1", 1000.0);
    Artwork g2 = makeArtwork("group2", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    GroupExhibition ex("Group", &g1, &g2, nullptr, &stand, &l, &f, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeGroup_NoStand) {
    Artwork g1 = makeArtwork("group1", 1000.0);
    Artwork g2 = makeArtwork("group2", 1000.0);
    Artwork g3 = makeArtwork("group3", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Timer t;
    Curator c("Curator");
    GroupExhibition ex("Group", &g1, &g2, &g3, nullptr, &l, &f, &cam, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeGroup_NoCamera) {
    Artwork g1 = makeArtwork("group1", 1000.0);
    Artwork g2 = makeArtwork("group2", 1000.0);
    Artwork g3 = makeArtwork("group3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    Timer t;
    Curator c("Curator");
    GroupExhibition ex("Group", &g1, &g2, &g3, &stand, &l, &f, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeGroup_NoTimer) {
    Artwork g1 = makeArtwork("group1", 1000.0);
    Artwork g2 = makeArtwork("group2", 1000.0);
    Artwork g3 = makeArtwork("group3", 1000.0);
    DisplayStand stand("Stand", 5.0, true, false);
    Lighting l("Lighting", true, false, 1);
    Frame f("Frame", true, 50, 1);
    SecurityCamera cam("Camera", 90.0, false, false);
    Curator c("Curator");
    GroupExhibition ex("Group", &g1, &g2, &g3, &stand, &l, &f, &cam, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizeSolo_NoArtwork1) {
    Artwork s2 = makeArtwork("solo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Timer t;
    Curator c("Curator");
    SoloExhibition ex("Solo", nullptr, &s2, &f, &l, &stand, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeSolo_NoArtwork2) {
    Artwork s1 = makeArtwork("solo1", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Timer t;
    Curator c("Curator");
    SoloExhibition ex("Solo", &s1, nullptr, &f, &l, &stand, &t, &c);
    EXPECT_THROW(ex.organize(), ArtworkNotFoundException);
}

TEST(Curator, OrganizeSolo_NoStand) {
    Artwork s1 = makeArtwork("solo1", 1000.0);
    Artwork s2 = makeArtwork("solo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    SoloExhibition ex("Solo", &s1, &s2, &f, &l, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizeSolo_NoTimer) {
    Artwork s1 = makeArtwork("solo1", 1000.0);
    Artwork s2 = makeArtwork("solo2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    DisplayStand stand("Stand", 5.0, true, false);
    Curator c("Curator");
    SoloExhibition ex("Solo", &s1, &s2, &f, &l, &stand, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Curator, OrganizePainting_NoFrame) {
    Artwork p1 = makeArtwork("painting1", 1000.0);
    Artwork p2 = makeArtwork("painting2", 1000.0);
    Lighting l("Lighting", true, false, 1);
    Timer t;
    Curator c("Curator");
    PaintingExhibition ex("Painting", &p1, &p2, nullptr, &l, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizePainting_NoLighting) {
    Artwork p1 = makeArtwork("painting1", 1000.0);
    Artwork p2 = makeArtwork("painting2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Timer t;
    Curator c("Curator");
    PaintingExhibition ex("Painting", &p1, &p2, &f, nullptr, &t, &c);
    EXPECT_THROW(ex.organize(), ToolNotAvailableException);
}

TEST(Curator, OrganizePainting_NoTimer) {
    Artwork p1 = makeArtwork("painting1", 1000.0);
    Artwork p2 = makeArtwork("painting2", 1000.0);
    Frame f("Frame", true, 50, 1);
    Lighting l("Lighting", true, false, 1);
    Curator c("Curator");
    PaintingExhibition ex("Painting", &p1, &p2, &f, &l, nullptr, &c);
    EXPECT_THROW(ex.organize(), TimerNotSetException);
}

TEST(Visitor, AddFavorite) {
    Visitor v("Test", 2, true);
    v.addFavorite();
    v.addFavorite();
}

TEST(Visitor, JoinGuidedTour) {
    Visitor v("Test", 2, true);
    v.joinGuidedTour();
}


