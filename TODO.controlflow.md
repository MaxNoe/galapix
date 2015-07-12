Galapix Control Flow
====================

Terms
-----

- deep archive: an archive within an archive
- fast-generation: JPEG thumbnail generation without decoding the higher resolutions
- slow-generation: loading a hi-res image and then down sampling it
- direct access: std::ifstream, Archive::get_file() -> BlobPtr
- indirect access: ???
- group-processing: file is part of an archive and the archive is handled as a whole
- individual-processing: file has direct-access and can be processed by itself


Procedure
---------

- galapix gets list of filenames

- filenames are converted to ResourceLocators

- ResourceLocator are classified
  - direct access, when plain filename
  - direct access, when indexed archive and JPEG
  - indirect access, when non-indexed archive
  - indirect access, when archive in archive

- file-type/handler detection
  - when direct access,

- Generation method are detected
  - fast generation when JPEG or JPEG in RAR file
  - slow generation when not a JPEG or JPEG inside an deep archive


- convert filename to ResourceLocator
-
  - request URLInfo for ResourceLocator
  - request FileInfo for ResourceLocator

ResLoc ---[request FileInfo]--->

Notes
-----

- processing mode can change when archives are extracted and allow fast access