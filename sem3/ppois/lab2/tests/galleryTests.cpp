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

