
# ImageEditorErasmus

A GIMP-like image editor built with **Qt** and **OpenCV**, featuring a GUI and advanced image-processing operations.  

---

## Dependencies

Before building the project, make sure you have installed the following:

- **C++17 compiler** (GCC/Clang/Visual Studio)
- **Qt 6.x** (Qt Widgets + LinguistTools)
- **OpenCV 4.x** (built with C++ support)
- **CMake 3.16 or higher**

Optional (for Windows deployment):
- `windeployqt` to bundle Qt DLLs with the executable

---

## Build Steps

1. Open a terminal and navigate to the project root:

```bash
cd /path/to/opencv-project-erasmus-team
```

2. Create and enter a build directory:

```bash
mkdir build
cd build
```

3. Run CMake to configure the project:

```bash
cmake ..
```

> On Windows, you may need to specify the generator or Qt/OpenCV paths:

```bash
cmake .. -G "Visual Studio 17 2022" -DCMAKE_PREFIX_PATH="C:/Qt/6.11.0/msvc2022_64;C:/opencv/build"
```

4. Compile the project:

- On Linux/macOS:

```bash
make
```

- On Windows (Visual Studio):

```bash
cmake --build . --config Release
```

The executable `imageEditorErasmus` will be generated in the `build` folder (or `Release` subfolder on Windows).

---

## How to Run

- On Linux/macOS:

```bash
./imageEditorErasmus
```

- On Windows (Release build):

```powershell
.\Release\imageEditorErasmus.exe
```

The GUI should open, allowing you to load images, apply operations, and save results.

---

## Sample Images for Testing

You can place sample images in the `test_images/` folder and load them in the editor:

```
test_images/
├─ bone_xray.png
├─ grabcut_test.png
├─ histogram_equalization_test.png
├─ isep_logo.png
├─ stitching1.png
├─ stitching2.png
├─ women_portrait.png
```

Use these images to test thresholding, histogram equalization, morphology, Canny edges, geometric transforms, and panorama stitching features.

---

## Notes

- Ensure that your `CMakeLists.txt` has the correct paths to Qt and OpenCV.
- Windows builds require Qt DLLs to be in the same folder as the executable (handled automatically if using `windeployqt`).
- Make sure to follow the project instructions for advanced features and GUI integration.
