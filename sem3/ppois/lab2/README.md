# Лабораторная работа №2: Gallery Management System

### Документация к лабораторной работе располагается по следующей <a href="https://press-cs.ru/PPOIS/BookWarehouse">ссылке</a>.

## Класс число_полей число_поведений(число_методов) -> Ассоциации:

Dimension 6 1(9)

Size 4 1(8) -> Dimension

Artwork 8 1(18) -> Size

GalleryTool 5 1(6)

Frame 8 1(8)

Lighting 8 1(8)

SecurityCamera 8 1(8)

DisplayStand 8 1(8)

Timer 4 1(9)

RestorationKit 6 1(7)

CleaningKit 6 1(3)

ConditionProfile 4 1(8)

ClimateControl 9 1(19) -> Timer, ConditionProfile

SecuritySystem 4 1(9)

Exhibition 1 1(5)

PaintingExhibition 4 1(2) -> Artwork, Frame

SculptureExhibition 3 1(2) -> Artwork, DisplayStand

PhotographyExhibition 4 1(2) -> Artwork, SecurityCamera

MixedMediaExhibition 5 1(2) -> Artwork, DisplayStand

DigitalArtExhibition 4 1(2) -> Artwork, DisplayStand

VintageExhibition 4 1(2) -> Artwork, ClimateControl

ContemporaryExhibition 4 1(2) -> Artwork, DisplayStand

MinimalistExhibition 4 1(2) -> Artwork, Frame

AbstractExhibition 4 1(2) -> Artwork, Lighting

PortraitExhibition 4 1(2) -> Artwork, Frame

LandscapeExhibition 4 1(2) -> Artwork, Lighting

RestorationExhibition 3 1(2) -> Artwork, RestorationKit

TemporaryExhibition 4 1(2) -> Artwork, DisplayStand

PermanentExhibition 4 1(2) -> Artwork, SecuritySystem

InteractiveExhibition 4 1(2) -> Artwork, SecurityCamera

ThematicExhibition 5 1(2) -> Artwork, DisplayStand

RetrospectiveExhibition 4 1(2) -> Artwork, ClimateControl

GroupExhibition 5 1(2) -> Artwork, DisplayStand

SoloExhibition 3 1(2) -> Artwork

Curator 1 1(2) -> Exhibition

Visitor 6 1(18) -> Artwork

GalleryMenu 4 1(11) -> Exhibition, Visitor

TicketSystem 7 1(15) -> Visitor, Exhibition

### Исключение количество_полей количество_методов:

ArtworkNotFoundException 0 1

NotEnoughArtworkException 0 1

ToolNotAvailableException 0 1

InvalidConditionException 0 1

TimerNotSetException 0 1

OverexposedArtworkException 0 1

UnderexposedArtworkException 0 1

StorageException 0 1

SecurityBreachException 0 1

RestorationFailedException 0 1

InsufficientSpaceException 0 1

InvalidAccessException 0 1

## Итого:

Поля: 183

Поведения (методы): 38(217)

Ассоциации: 47

Исключения: 12

Классы: 50

## Test Coverage

**Current test coverage: 88.6%**

- **Covered lines:** 1277
- **Total lines:** 1442
- **Uncovered lines:** 165

For detailed coverage report, see [Coverage Report](coverage_report.html) or [Detailed lcov Report](coverage_html/index.html).
