/**
 * @file main.cpp
 * @author James Maxwell
 * @brief Main entry point for the Gallery management system
 * @version 0.1
 * @date 2025-01-15
 * 
 * This file contains the main function that demonstrates the gallery system
 * with various artworks, tools, and exhibitions.
 */

#include "gallery.hpp"

/**
 * @brief Main function
 * 
 * Creates artworks, tools, exhibitions, and runs the gallery menu system.
 * 
 * @return int exit code (0 on success, 1 on error)
 */
int main() {
    try {
        Dimension cmUnit("cm", 1.0, true, 1);
        Dimension inchUnit("in", 2.54, false, 2);
        Dimension meterUnit("m", 100.0, true, 3);
        Artwork painting1(
            "Mona Lisa",
            Size(77.0, &cmUnit, false, 1),
            850000000.0,
            true
        );
        Artwork painting2(
            "Starry Night",
            Size(73.7, &cmUnit, false, 2),
            100000000.0,
            true
        );
        Artwork sculpture1(
            "David",
            Size(517.0, &cmUnit, false, 3),
            200000000.0,
            true
        );
        Artwork photo1(
            "Ansel Adams Landscape",
            Size(40.6, &cmUnit, false, 4),
            5000000.0,
            false
        );
        Artwork photo2(
            "Dorothea Lange Portrait",
            Size(30.5, &cmUnit, false, 5),
            3000000.0,
            false
        );
        Artwork digital1(
            "Digital Abstract",
            Size(120.0, &cmUnit, false, 6),
            50000.0,
            false
        );
        Artwork digital2(
            "Interactive Installation",
            Size(200.0, &cmUnit, false, 7),
            75000.0,
            false
        );
        Artwork vintage1(
            "Renaissance Masterpiece",
            Size(100.0, &cmUnit, false, 8),
            50000000.0,
            true
        );
        Artwork vintage2(
            "Baroque Painting",
            Size(150.0, &cmUnit, false, 9),
            30000000.0,
            true
        );
        Artwork contemporary1(
            "Modern Abstract",
            Size(180.0, &cmUnit, false, 10),
            2000000.0,
            false
        );
        Artwork contemporary2(
            "Contemporary Sculpture",
            Size(250.0, &cmUnit, false, 11),
            1500000.0,
            true
        );
        Artwork minimalist1(
            "Minimalist Composition",
            Size(60.0, &cmUnit, false, 12),
            800000.0,
            false
        );
        Artwork abstract1(
            "Abstract Expression",
            Size(140.0, &cmUnit, false, 13),
            1200000.0,
            false
        );
        Artwork abstract2(
            "Color Field",
            Size(160.0, &cmUnit, false, 14),
            900000.0,
            false
        );
        Artwork portrait1(
            "Portrait of Artist",
            Size(80.0, &cmUnit, false, 15),
            600000.0,
            true
        );
        Artwork portrait2(
            "Self Portrait",
            Size(70.0, &cmUnit, false, 16),
            550000.0,
            true
        );
        Artwork landscape1(
            "Mountain Landscape",
            Size(120.0, &cmUnit, false, 17),
            700000.0,
            true
        );
        Artwork landscape2(
            "Seascape",
            Size(110.0, &cmUnit, false, 18),
            650000.0,
            true
        );
        Artwork restoration1(
            "Damaged Masterpiece",
            Size(150.0, &cmUnit, false, 19),
            10000000.0,
            true
        );
        Artwork temp1(
            "Temporary Artwork 1",
            Size(100.0, &cmUnit, false, 20),
            400000.0,
            false
        );
        Artwork temp2(
            "Temporary Artwork 2",
            Size(95.0, &cmUnit, false, 21),
            380000.0,
            false
        );
        Artwork permanent1(
            "Permanent Collection 1",
            Size(130.0, &cmUnit, false, 22),
            5000000.0,
            true
        );
        Artwork permanent2(
            "Permanent Collection 2",
            Size(125.0, &cmUnit, false, 23),
            4800000.0,
            true
        );
        Artwork interactive1(
            "Interactive Piece 1",
            Size(180.0, &cmUnit, false, 24),
            300000.0,
            false
        );
        Artwork interactive2(
            "Interactive Piece 2",
            Size(175.0, &cmUnit, false, 25),
            280000.0,
            false
        );
        Artwork theme1(
            "Thematic Artwork 1",
            Size(85.0, &cmUnit, false, 26),
            450000.0,
            false
        );
        Artwork theme2(
            "Thematic Artwork 2",
            Size(90.0, &cmUnit, false, 27),
            470000.0,
            false
        );
        Artwork theme3(
            "Thematic Artwork 3",
            Size(88.0, &cmUnit, false, 28),
            460000.0,
            false
        );
        Artwork retro1(
            "Retrospective Work 1",
            Size(140.0, &cmUnit, false, 29),
            2500000.0,
            true
        );
        Artwork retro2(
            "Retrospective Work 2",
            Size(135.0, &cmUnit, false, 30),
            2300000.0,
            true
        );
        Artwork group1(
            "Group Artwork 1",
            Size(100.0, &cmUnit, false, 31),
            500000.0,
            false
        );
        Artwork group2(
            "Group Artwork 2",
            Size(105.0, &cmUnit, false, 32),
            520000.0,
            false
        );
        Artwork group3(
            "Group Artwork 3",
            Size(98.0, &cmUnit, false, 33),
            490000.0,
            false
        );
        Artwork solo1(
            "Solo Artist Work 1",
            Size(115.0, &cmUnit, false, 34),
            600000.0,
            false
        );
        Artwork solo2(
            "Solo Artist Work 2",
            Size(110.0, &cmUnit, false, 35),
            580000.0,
            false
        );
        Artwork mixed1(
            "Mixed Media 1",
            Size(95.0, &cmUnit, false, 36),
            350000.0,
            false
        );
        Artwork mixed2(
            "Mixed Media 2",
            Size(100.0, &cmUnit, false, 37),
            370000.0,
            false
        );
        Artwork mixed3(
            "Mixed Media 3",
            Size(92.0, &cmUnit, false, 38),
            340000.0,
            false
        );
        Frame classicFrame("Classic Frame", true, 50, 1);
        Frame modernFrame("Modern Frame", true, 60, 2);
        Frame protectiveFrame("Protective Frame", true, 55, 3);
        Lighting galleryLighting("Gallery Lighting", true, false, 1);
        Lighting accentLighting("Accent Lighting", true, false, 2);
        Lighting dimLighting("Dim Lighting", true, true, 3);
        SecurityCamera mainCamera("Main Security Camera", 90.0, false, false);
        SecurityCamera secondaryCamera("Secondary Camera", 120.0, false, false);
        SecurityCamera entranceCamera("Entrance Camera", 180.0, false, false);
        DisplayStand mainStand("Main Display Stand", 5.0, true, false);
        DisplayStand sculptureStand("Sculpture Stand", 3.0, true, false);
        DisplayStand temporaryStand("Temporary Stand", 4.0, true, false);
        RestorationKit professionalKit("Professional Restoration Kit", false);
        RestorationKit standardKit("Standard Restoration Kit", false);
        CleaningKit maintenanceKit("Maintenance Cleaning Kit", 1);
        CleaningKit delicateKit("Delicate Cleaning Kit", 2);
        ClimateControl mainClimate(0.0, false, true);
        ClimateControl storageClimate(0.0, false, true);
        SecuritySystem mainSecurity(6, 0, true, false);
        SecuritySystem gallerySecurity(4, 0, true, false);
        Timer paintingTimer(0, false, 0, 1);
        Timer sculptureTimer(0, false, 0, 2);
        Timer photoTimer(0, false, 0, 3);
        Timer digitalTimer(0, false, 0, 4);
        Timer vintageTimer(0, false, 0, 5);
        Timer contemporaryTimer(0, false, 0, 6);
        Timer abstractTimer(0, false, 0, 7);
        Timer portraitTimer(0, false, 0, 8);
        Timer landscapeTimer(0, false, 0, 9);
        Timer restorationTimer(0, false, 0, 10);
        Timer temporaryTimer(0, false, 0, 11);
        Timer permanentTimer(0, false, 0, 12);
        Timer interactiveTimer(0, false, 0, 13);
        Timer thematicTimer(0, false, 0, 14);
        Timer retrospectiveTimer(0, false, 0, 15);
        Timer groupTimer(0, false, 0, 16);
        Timer soloTimer(0, false, 0, 17);
        Timer mixedMediaTimer(0, false, 0, 18);
        Curator mainCurator("Main Curator");
        PaintingExhibition paintingExhibition(
            "Classic Paintings",
            &painting1,
            &painting2,
            &classicFrame,
            &galleryLighting,
            &paintingTimer,
            &mainCurator
        );
        SculptureExhibition sculptureExhibition(
            "Sculpture Collection",
            &sculpture1,
            &sculptureStand,
            &accentLighting,
            &modernFrame,
            &mainCurator
        );
        PhotographyExhibition photographyExhibition(
            "Photography Showcase",
            &photo1,
            &photo2,
            &modernFrame,
            &galleryLighting,
            &mainCamera,
            &photoTimer,
            &mainCurator
        );
        MixedMediaExhibition mixedMediaExhibition(
            "Mixed Media Exhibition",
            &mixed1,
            &mixed2,
            &mixed3,
            &mainStand,
            &galleryLighting,
            &classicFrame,
            &mixedMediaTimer,
            &professionalKit,
            &mainCurator
        );
        DigitalArtExhibition digitalArtExhibition(
            "Digital Art Display",
            &digital1,
            &digital2,
            &temporaryStand,
            &accentLighting,
            &secondaryCamera,
            &digitalTimer,
            &mainCurator
        );
        VintageExhibition vintageExhibition(
            "Vintage Collection",
            &vintage1,
            &vintage2,
            &protectiveFrame,
            &mainClimate,
            &professionalKit,
            &mainCurator
        );
        ContemporaryExhibition contemporaryExhibition(
            "Contemporary Art",
            &contemporary1,
            &contemporary2,
            &mainStand,
            &galleryLighting,
            &mainCamera,
            &contemporaryTimer,
            &mainCurator
        );
        MinimalistExhibition minimalistExhibition(
            "Minimalist Showcase",
            &minimalist1,
            &modernFrame,
            &dimLighting,
            &mainCurator
        );
        AbstractExhibition abstractExhibition(
            "Abstract Art Collection",
            &abstract1,
            &abstract2,
            &mainStand,
            &galleryLighting,
            &classicFrame,
            &abstractTimer,
            &mainCurator
        );
        PortraitExhibition portraitExhibition(
            "Portrait Gallery",
            &portrait1,
            &portrait2,
            &classicFrame,
            &accentLighting,
            &entranceCamera,
            &mainCurator
        );
        LandscapeExhibition landscapeExhibition(
            "Landscape Collection",
            &landscape1,
            &landscape2,
            &modernFrame,
            &galleryLighting,
            &landscapeTimer,
            &mainCurator
        );
        RestorationExhibition restorationExhibition(
            "Restoration Showcase",
            &restoration1,
            &professionalKit,
            &delicateKit,
            &storageClimate,
            &restorationTimer,
            &mainCurator
        );
        TemporaryExhibition temporaryExhibition(
            "Temporary Display",
            &temp1,
            &temp2,
            &temporaryStand,
            &accentLighting,
            &secondaryCamera,
            &temporaryTimer,
            &mainCurator
        );
        PermanentExhibition permanentExhibition(
            "Permanent Collection",
            &permanent1,
            &permanent2,
            &protectiveFrame,
            &mainClimate,
            &gallerySecurity,
            &permanentTimer,
            &mainCurator
        );
        InteractiveExhibition interactiveExhibition(
            "Interactive Art",
            &interactive1,
            &interactive2,
            &mainStand,
            &galleryLighting,
            &mainCamera,
            &interactiveTimer,
            &mainCurator
        );
        ThematicExhibition thematicExhibition(
            "Thematic Collection",
            &theme1,
            &theme2,
            &theme3,
            &classicFrame,
            &mainStand,
            &galleryLighting,
            &thematicTimer,
            &mainCurator
        );
        RetrospectiveExhibition retrospectiveExhibition(
            "Retrospective Show",
            &retro1,
            &retro2,
            &protectiveFrame,
            &mainClimate,
            &standardKit,
            &retrospectiveTimer,
            &mainCurator
        );
        GroupExhibition groupExhibition(
            "Group Art Show",
            &group1,
            &group2,
            &group3,
            &mainStand,
            &galleryLighting,
            &modernFrame,
            &mainCamera,
            &groupTimer,
            &mainCurator
        );
        SoloExhibition soloExhibition(
            "Solo Artist Exhibition",
            &solo1,
            &solo2,
            &classicFrame,
            &accentLighting,
            &sculptureStand,
            &soloTimer,
            &mainCurator
        );
        Visitor galleryVisitor("Gallery Visitor", 2, true);
        galleryVisitor.purchaseTicket();
        galleryVisitor.enterGallery();
        GalleryMenu menu;
        menu.setVisitor(&galleryVisitor);
        menu.addExhibition(&paintingExhibition);
        menu.addExhibition(&sculptureExhibition);
        menu.addExhibition(&photographyExhibition);
        menu.addExhibition(&mixedMediaExhibition);
        menu.addExhibition(&digitalArtExhibition);
        menu.addExhibition(&vintageExhibition);
        menu.addExhibition(&contemporaryExhibition);
        menu.addExhibition(&minimalistExhibition);
        menu.addExhibition(&abstractExhibition);
        menu.addExhibition(&portraitExhibition);
        menu.addExhibition(&landscapeExhibition);
        menu.addExhibition(&restorationExhibition);
        menu.addExhibition(&temporaryExhibition);
        menu.addExhibition(&permanentExhibition);
        menu.addExhibition(&interactiveExhibition);
        menu.addExhibition(&thematicExhibition);
        menu.addExhibition(&retrospectiveExhibition);
        menu.addExhibition(&groupExhibition);
        menu.addExhibition(&soloExhibition);
        menu.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "\nCritical error during program execution: "
                  << ex.what() << std::endl;
        return 1;
    }
    return 0;
}