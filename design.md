# SnapDecision Program Design Summary

## Purpose
SnapDecision is a C++ program using Qt, designed for sorting large numbers of images from digital cameras into 'keep' and 'delete' categories. Key features include:
- Indexing all images in a directory for classification.
- Launching 3rd party applications for displayed images.

## Main Interface
- Primary input: Keyboard for classification (e.g., 'x' to mark for delete).
- Undo and Redo support.
- Color-coded bar indicating classification status.
- Additional views for correlating classifications with image metadata.

## Initial State
- Images start unclassified unless persistent data is found in a DB file.

## Image Metadata
Includes shutter speed, ISO, exposure compensation, zoom, time, and relative position in a burst.

## Program Structure
Follows the Model-View-Controller (MVC) architecture with MainModel, MainController, and MainWindow classes.

## Image Loading
Features asynchronous loading, caching, and predictive management of large images.

## Database
Utilizes SQLite for classification data management, with options for delayed database creation based on user actions.

---

# Design Document for DatabaseManager in SnapDecision

## Overview
The DatabaseManager class is essential for data persistence and efficient state management in SnapDecision.

## Key Responsibilities
- Data Persistence
- Database Mode Management
- Data Integrity and Schema Management

## Structure
- Encapsulates all database operations.
- Handles initialization and mode switching.
- Manages data saving and retrieval.

## Integration with SnapDecision
- Saves image classifications.
- Maintains state persistence for seamless user experience.
- Optimizes performance by starting with an in-memory database.

## Technical Details
- Uses SQLite and Qt's QtSql module.
- Error handling and logging.

---

# Design Document for Image Caching System in SnapDecision

## Purpose
Optimizes performance by managing image loading and caching.

## Components
- ImageCache Class for caching.
- DataElement Class representing image data and metadata.

## Functionality
- Asynchronous Image Loading
- Effective Memory Management

## System Structure
- Follows the MVC approach.
- Thread management for UI responsiveness.

## Performance Considerations
- Efficient memory use with dynamic adjustment.
- Predictive fetching.

---

# Design Document: Qt Model for Image Hierarchy Display

## Concept
Organizes images into a hierarchy based on time, location, and scenes.

## Key Components
- ImageDescription Struct for image metadata.
- TreeNode Structure for hierarchical representation.
- ImageTreeBuilder Class for tree construction.
- ImageTreeModel Class for interface with QTreeView.

## Customizations
- Icons and text coloring for decision representation.

---

# Database Structure


| Database Column Name | C++ Data Type         | Database Data Type | Default Value          | Getter Name           | Setter Name           | Conversion Functions           |
|----------------------|-----------------------|--------------------|------------------------|-----------------------|-----------------------|--------------------------------|
| AbsolutePath         | std::string           | TEXT               |                        | getAbsolutePath       | setAbsolutePath       |                                |
| Decision             | DecisionType          | TEXT               | 'Unknown'              | getDecision           | setDecision           | to_string, to_DecisionType     |
| ImageWidth           | int                   | INTEGER            | 0                      | getImageWidth         | setImageWidth         |                                |
| ImageHeight          | int                   | INTEGER            | 0                      | getImageHeight        | setImageHeight        |                                |
| Make                 | std::string           | TEXT               |                        | getMake               | setMake               |                                |
| Model                | std::string           | TEXT               |                        | getModel              | setModel              |                                |
| BitsPerSample        | int                   | INTEGER            | 0                      | getBitsPerSample      | setBitsPerSample      |                                |
| DateTime             | std::string           | TEXT               |                        | getDateTime           | setDateTime           |                                |
| DateTimeOriginal     | std::string           | TEXT               |                        | getDateTimeOriginal   | setDateTimeOriginal   |                                |
| SubSecTimeOriginal   | std::string           | TEXT               |                        | getSubSecTimeOriginal | setSubSecTimeOriginal |                                |
| FNumber              | double                | REAL               | 0.0                    | getFNumber            | setFNumber            |                                |
| ExposureProgram      | ExposureProgram       | TEXT               | 'NotDefined'           | getExposureProgram    | setExposureProgram    | to_string, to_ExposureProgram  |
| ISOSpeedRatings      | int                   | INTEGER            | 0                      | getISOSpeedRatings    | setISOSpeedRatings    |                                |
| Orientation          | int                   | INTEGER            | 0                      | getOrientation        | setOrientation    |                                |
| ShutterSpeedValue    | double                | REAL               | 0.0                    | getShutterSpeedValue  | setShutterSpeedValue  |                                |
| ApertureValue        | double                | REAL               | 0.0                    | getApertureValue      | setApertureValue      |                                |
| BrightnessValue      | double                | REAL               | 0.0                    | getBrightnessValue    | setBrightnessValue    |                                |
| ExposureBiasValue    | double                | REAL               | 0.0                    | getExposureBiasValue  | setExposureBiasValue  |                                |
| SubjectDistance      | double                | REAL               | 0.0                    | getSubjectDistance    | setSubjectDistance    |                                |
| FocalLength          | double                | REAL               | 0.0                    | getFocalLength        | setFocalLength        |                                |
| MeteringMode         | MeteringMode          | TEXT               | 'Unknown'              | getMeteringMode       | setMeteringMode       | to_string, to_MeteringMode     |
| CreationMs           | std::size_t           | BIGINT             | 0                      | getCreationMs         | setCreationMs         |                                |


