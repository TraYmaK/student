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
#include <vector>
#include <memory>

namespace {
    Artwork createArtwork(const char* name, double size, double value, bool fragile, Dimension* dim, int id) {
        return Artwork(name, Size(size, dim, false, id), value, fragile);
    }
    
    void initializeDimensions(Dimension& cm, Dimension& inch, Dimension& meter) {
        cm = Dimension("cm", 1.0, true, 1);
        inch = Dimension("in", 2.54, false, 2);
        meter = Dimension("m", 100.0, true, 3);
    }
    
    void initializeArtworkCollection(std::vector<Artwork>& collection, Dimension* dim) {
        const struct {
            const char* name;
            double size;
            double value;
            bool fragile;
        } artworkData[] = {
            {"Mona Lisa", 77.0, 850000000.0, true},
            {"Starry Night", 73.7, 100000000.0, true},
            {"David", 517.0, 200000000.0, true},
            {"Ansel Adams Landscape", 40.6, 5000000.0, false},
            {"Dorothea Lange Portrait", 30.5, 3000000.0, false},
            {"Digital Abstract", 120.0, 50000.0, false},
            {"Interactive Installation", 200.0, 75000.0, false},
            {"Renaissance Masterpiece", 100.0, 50000000.0, true},
            {"Baroque Painting", 150.0, 30000000.0, true},
            {"Modern Abstract", 180.0, 2000000.0, false},
            {"Contemporary Sculpture", 250.0, 1500000.0, true},
            {"Minimalist Composition", 60.0, 800000.0, false},
            {"Abstract Expression", 140.0, 1200000.0, false},
            {"Color Field", 160.0, 900000.0, false},
            {"Portrait of Artist", 80.0, 600000.0, true},
            {"Self Portrait", 70.0, 550000.0, true},
            {"Mountain Landscape", 120.0, 700000.0, true},
            {"Seascape", 110.0, 650000.0, true},
            {"Damaged Masterpiece", 150.0, 10000000.0, true},
            {"Temporary Artwork 1", 100.0, 400000.0, false},
            {"Temporary Artwork 2", 95.0, 380000.0, false},
            {"Permanent Collection 1", 130.0, 5000000.0, true},
            {"Permanent Collection 2", 125.0, 4800000.0, true},
            {"Interactive Piece 1", 180.0, 300000.0, false},
            {"Interactive Piece 2", 175.0, 280000.0, false},
            {"Thematic Artwork 1", 85.0, 450000.0, false},
            {"Thematic Artwork 2", 90.0, 470000.0, false},
            {"Thematic Artwork 3", 88.0, 460000.0, false},
            {"Retrospective Work 1", 140.0, 2500000.0, true},
            {"Retrospective Work 2", 135.0, 2300000.0, true},
            {"Group Artwork 1", 100.0, 500000.0, false},
            {"Group Artwork 2", 105.0, 520000.0, false},
            {"Group Artwork 3", 98.0, 490000.0, false},
            {"Solo Artist Work 1", 115.0, 600000.0, false},
            {"Solo Artist Work 2", 110.0, 580000.0, false},
            {"Mixed Media 1", 95.0, 350000.0, false},
            {"Mixed Media 2", 100.0, 370000.0, false},
            {"Mixed Media 3", 92.0, 340000.0, false}
        };
        
        collection.reserve(sizeof(artworkData) / sizeof(artworkData[0]));
        for (size_t i = 0; i < sizeof(artworkData) / sizeof(artworkData[0]); ++i) {
            collection.push_back(createArtwork(
                artworkData[i].name,
                artworkData[i].size,
                artworkData[i].value,
                artworkData[i].fragile,
                dim,
                static_cast<int>(i + 1)
            ));
        }
    }
    
    void initializeTools(
        std::vector<Frame>& frames,
        std::vector<Lighting>& lights,
        std::vector<SecurityCamera>& cameras,
        std::vector<DisplayStand>& stands,
        std::vector<RestorationKit>& kits,
        std::vector<CleaningKit>& cleaningKits,
        std::vector<ClimateControl>& climates,
        std::vector<SecuritySystem>& securitySystems,
        std::vector<Timer>& timers
    ) {
        frames.push_back(Frame("Classic Frame", true, 50, 1));
        frames.push_back(Frame("Modern Frame", true, 60, 2));
        frames.push_back(Frame("Protective Frame", true, 55, 3));
        
        lights.push_back(Lighting("Gallery Lighting", true, false, 1));
        lights.push_back(Lighting("Accent Lighting", true, false, 2));
        lights.push_back(Lighting("Dim Lighting", true, true, 3));
        
        cameras.push_back(SecurityCamera("Main Security Camera", 90.0, false, false));
        cameras.push_back(SecurityCamera("Secondary Camera", 120.0, false, false));
        cameras.push_back(SecurityCamera("Entrance Camera", 180.0, false, false));
        
        stands.push_back(DisplayStand("Main Display Stand", 5.0, true, false));
        stands.push_back(DisplayStand("Sculpture Stand", 3.0, true, false));
        stands.push_back(DisplayStand("Temporary Stand", 4.0, true, false));
        
        kits.push_back(RestorationKit("Professional Restoration Kit", false));
        kits.push_back(RestorationKit("Standard Restoration Kit", false));
        
        cleaningKits.push_back(CleaningKit("Maintenance Cleaning Kit", 1));
        cleaningKits.push_back(CleaningKit("Delicate Cleaning Kit", 2));
        
        climates.push_back(ClimateControl(0.0, false, true));
        climates.push_back(ClimateControl(0.0, false, true));
        
        securitySystems.push_back(SecuritySystem(6, 0, true, false));
        securitySystems.push_back(SecuritySystem(4, 0, true, false));
        
        for (int i = 1; i <= 18; ++i) {
            timers.push_back(Timer(0, false, 0, i));
        }
    }
}

/**
 * @brief Run the gallery system
 * 
 * Creates artworks, tools, exhibitions, and runs the gallery menu system.
 * This function can be called from main() or from tests.
 * 
 * @param skipMenuRun if true, skips menu.run() call (useful for testing)
 * @return int exit code (0 on success, 1 on error)
 */
int runGallerySystem(bool skipMenuRun = false) {
    try {
        Dimension cmUnit, inchUnit, meterUnit;
        initializeDimensions(cmUnit, inchUnit, meterUnit);
        
        std::vector<Artwork> artworkCollection;
        initializeArtworkCollection(artworkCollection, &cmUnit);
        
        std::vector<Frame> frames;
        std::vector<Lighting> lights;
        std::vector<SecurityCamera> cameras;
        std::vector<DisplayStand> stands;
        std::vector<RestorationKit> restorationKits;
        std::vector<CleaningKit> cleaningKits;
        std::vector<ClimateControl> climates;
        std::vector<SecuritySystem> securitySystems;
        std::vector<Timer> timers;
        
        initializeTools(frames, lights, cameras, stands, restorationKits, 
                       cleaningKits, climates, securitySystems, timers);
        
        Curator mainCurator("Main Curator");
        PaintingExhibition paintingExhibition(
            "Classic Paintings",
            &artworkCollection[0], &artworkCollection[1],
            &frames[0], &lights[0], &timers[0], &mainCurator
        );
        
        SculptureExhibition sculptureExhibition(
            "Sculpture Collection",
            &artworkCollection[2], &stands[1],
            &lights[1], &frames[1], &mainCurator
        );
        
        PhotographyExhibition photographyExhibition(
            "Photography Showcase",
            &artworkCollection[3], &artworkCollection[4],
            &frames[1], &lights[0], &cameras[0], &timers[2], &mainCurator
        );
        
        MixedMediaExhibition mixedMediaExhibition(
            "Mixed Media Exhibition",
            &artworkCollection[35], &artworkCollection[36], &artworkCollection[37],
            &stands[0], &lights[0], &frames[0], &timers[17], &restorationKits[0], &mainCurator
        );
        
        DigitalArtExhibition digitalArtExhibition(
            "Digital Art Display",
            &artworkCollection[5], &artworkCollection[6],
            &stands[2], &lights[1], &cameras[1], &timers[3], &mainCurator
        );
        
        VintageExhibition vintageExhibition(
            "Vintage Collection",
            &artworkCollection[7], &artworkCollection[8],
            &frames[2], &climates[0], &restorationKits[0], &mainCurator
        );
        
        ContemporaryExhibition contemporaryExhibition(
            "Contemporary Art",
            &artworkCollection[9], &artworkCollection[10],
            &stands[0], &lights[0], &cameras[0], &timers[5], &mainCurator
        );
        
        MinimalistExhibition minimalistExhibition(
            "Minimalist Showcase",
            &artworkCollection[11], &frames[1], &lights[2], &mainCurator
        );
        
        AbstractExhibition abstractExhibition(
            "Abstract Art Collection",
            &artworkCollection[12], &artworkCollection[13],
            &stands[0], &lights[0], &frames[0], &timers[6], &mainCurator
        );
        
        PortraitExhibition portraitExhibition(
            "Portrait Gallery",
            &artworkCollection[14], &artworkCollection[15],
            &frames[0], &lights[1], &cameras[2], &mainCurator
        );
        
        LandscapeExhibition landscapeExhibition(
            "Landscape Collection",
            &artworkCollection[16], &artworkCollection[17],
            &frames[1], &lights[0], &timers[8], &mainCurator
        );
        
        RestorationExhibition restorationExhibition(
            "Restoration Showcase",
            &artworkCollection[18], &restorationKits[0],
            &cleaningKits[1], &climates[1], &timers[9], &mainCurator
        );
        
        TemporaryExhibition temporaryExhibition(
            "Temporary Display",
            &artworkCollection[19], &artworkCollection[20],
            &stands[2], &lights[1], &cameras[1], &timers[10], &mainCurator
        );
        
        PermanentExhibition permanentExhibition(
            "Permanent Collection",
            &artworkCollection[21], &artworkCollection[22],
            &frames[2], &climates[0], &securitySystems[1], &timers[11], &mainCurator
        );
        
        InteractiveExhibition interactiveExhibition(
            "Interactive Art",
            &artworkCollection[23], &artworkCollection[24],
            &stands[0], &lights[0], &cameras[0], &timers[12], &mainCurator
        );
        
        ThematicExhibition thematicExhibition(
            "Thematic Collection",
            &artworkCollection[25], &artworkCollection[26], &artworkCollection[27],
            &frames[0], &stands[0], &lights[0], &timers[13], &mainCurator
        );
        
        RetrospectiveExhibition retrospectiveExhibition(
            "Retrospective Show",
            &artworkCollection[28], &artworkCollection[29],
            &frames[2], &climates[0], &restorationKits[1], &timers[14], &mainCurator
        );
        
        GroupExhibition groupExhibition(
            "Group Art Show",
            &artworkCollection[30], &artworkCollection[31], &artworkCollection[32],
            &stands[0], &lights[0], &frames[1], &cameras[0], &timers[15], &mainCurator
        );
        
        SoloExhibition soloExhibition(
            "Solo Artist Exhibition",
            &artworkCollection[33], &artworkCollection[34],
            &frames[0], &lights[1], &stands[1], &timers[16], &mainCurator
        );
        
        Visitor galleryVisitor("Gallery Visitor", 2, false);
        galleryVisitor.purchaseTicket();
        galleryVisitor.enterGallery();
        
        GalleryMenu menuSystem;
        menuSystem.setVisitor(&galleryVisitor);
        
        Exhibition* allExhibitions[] = {
            &paintingExhibition, &sculptureExhibition, &photographyExhibition, &mixedMediaExhibition,
            &digitalArtExhibition, &vintageExhibition, &contemporaryExhibition, &minimalistExhibition,
            &abstractExhibition, &portraitExhibition, &landscapeExhibition, &restorationExhibition,
            &temporaryExhibition, &permanentExhibition, &interactiveExhibition, &thematicExhibition,
            &retrospectiveExhibition, &groupExhibition, &soloExhibition
        };
        
        for (Exhibition* exhibition : allExhibitions) {
            menuSystem.addExhibition(exhibition);
        }
        
        if (!skipMenuRun) {
            menuSystem.run();
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "\n[ERROR] System failure: " << ex.what() << std::endl;
        return 1;
    }
    return 0;
}

/**
 * @brief Main function
 * 
 * Entry point for the gallery management system.
 * 
 * @return int exit code (0 on success, 1 on error)
 */
#ifndef TESTING
int main() {
    return runGallerySystem();
}
#endif