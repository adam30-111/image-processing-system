#define NOMINMAX
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/fl_draw.H>
#include <FL/fl_ask.H>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <cstdlib>
#include "Image_Class.h"
#include "filters.h"
using namespace std;

// ---------------------------------------------------------------
// Global state
// ---------------------------------------------------------------
Image currentImage;
string currentFilename;
bool imageLoaded = false;
vector<Image> undoStack;
const size_t MAX_UNDO = 10;
const size_t MAX_UNDO_BYTES = 500ull * 1024 * 1024;

size_t undoBytes()
{
    size_t total = 0;
    for (const Image& im : undoStack)
        total += (size_t)im.width * im.height * im.channels;
    return total;
}
Fl_Box* statusBar = nullptr;
Fl_Choice* filterChoice = nullptr;
Fl_Button* undoBtn = nullptr;

void updateStatus();
bool askIntInRange(const string& prompt, int def, int lo, int hi, int& out);

void updateUndoButton()
{
    if (!undoBtn) return;
    if (undoStack.empty()) undoBtn->deactivate();
    else undoBtn->activate();
}

// ---------------------------------------------------------------
// ImageView: draws the image and handles zoom and pan
// ---------------------------------------------------------------
class ImageView : public Fl_Widget {
    double zoom_ = 1.0;
    double offX_ = 0, offY_ = 0;    
    bool fitMode_ = true;           
    int lastX_ = 0, lastY_ = 0;     
    Fl_RGB_Image* small_ = nullptr;
    int smallW_ = 0;
    int ix_ = 0, iy_ = 0, dx0_ = 0, dy0_ = 0;

    int iw() const { return max(1, (int)(currentImage.width * zoom_ + 0.5)); }
    int ih() const { return max(1, (int)(currentImage.height * zoom_ + 0.5)); }

    void invalidateSmall() { delete small_; small_ = nullptr; smallW_ = 0; }

    void clampOffset() {
        double vw = w(), vh = h();
        double sw = iw(), sh = ih();
        if (sw <= vw) offX_ = (vw - sw) / 2.0;
        else offX_ = min(0.0, max(vw - sw, offX_));
        if (sh <= vh) offY_ = (vh - sh) / 2.0;
        else offY_ = min(0.0, max(vh - sh, offY_));
    }

    void sampleRow(int x, int y, int n, uchar* buf) {
        int W = currentImage.width, H = currentImage.height;
        int ch = currentImage.channels;
        int sy = (int)((dy0_ + y - iy_) / zoom_);
        sy = max(0, min(H - 1, sy));
        const unsigned char* row = currentImage.imageData + (size_t)sy * W * ch;
        for (int i = 0; i < n; i++) {
            int sx = (int)((dx0_ + x + i - ix_) / zoom_);
            sx = max(0, min(W - 1, sx));
            const unsigned char* p = row + (size_t)sx * ch;
            buf[i * 3 + 0] = p[0];
            buf[i * 3 + 1] = p[ch > 1 ? 1 : 0];
            buf[i * 3 + 2] = p[ch > 2 ? 2 : 0];
        }
    }
    static void sampleCb(void* d, int x, int y, int n, uchar* buf) {
        ((ImageView*)d)->sampleRow(x, y, n, buf);
    }

public:
    ImageView(int X, int Y, int W, int H) : Fl_Widget(X, Y, W, H) {}
    ~ImageView() { delete small_; }

    double zoom() const { return zoom_; }

    void imageChanged(bool refit) {
        invalidateSmall();
        if (refit || fitMode_) fit();
        else { clampOffset(); redraw(); updateStatus(); }
    }

    void fit() {
        fitMode_ = true;
        if (imageLoaded) {
            double zx = (double)w() / currentImage.width;
            double zy = (double)h() / currentImage.height;
            zoom_ = min(min(zx, zy), 1.0);
            clampOffset();
        }
        invalidateSmall();
        redraw();
        updateStatus();
    }

    void zoomAt(double newZoom, int cx, int cy) {
        if (!imageLoaded) return;
        newZoom = max(0.05, min(32.0, newZoom));
        double px = (cx - offX_) / zoom_;
        double py = (cy - offY_) / zoom_;
        zoom_ = newZoom;
        offX_ = cx - px * zoom_;
        offY_ = cy - py * zoom_;
        fitMode_ = false;
        clampOffset();
        invalidateSmall();
        redraw();
        updateStatus();
    }

    void resize(int X, int Y, int W, int H) override {
        Fl_Widget::resize(X, Y, W, H);
        if (!imageLoaded) return;
        if (fitMode_) fit();
        else { clampOffset(); redraw(); }
    }

    void draw() override {
        fl_push_clip(x(), y(), w(), h());
        fl_color(fl_rgb_color(28, 28, 30));
        fl_rectf(x(), y(), w(), h());

        if (!imageLoaded) {
            fl_color(fl_rgb_color(150, 150, 150));
            fl_font(FL_HELVETICA, 16);
            fl_draw("Click Open to load an image", x(), y(), w(), h(), FL_ALIGN_CENTER);
        } else {
            int sw = iw(), sh = ih();
            ix_ = x() + (int)floor(offX_ + 0.5);
            iy_ = y() + (int)floor(offY_ + 0.5);

            int vx0 = max(ix_, x()),            vy0 = max(iy_, y());
            int vx1 = min(ix_ + sw, x() + w()), vy1 = min(iy_ + sh, y() + h());

            if (vx1 > vx0 && vy1 > vy0) {
                if (zoom_ < 1.0) {
                    if (!small_ || smallW_ != sw) {
                        delete small_;
                        Fl_RGB_Image tmp(currentImage.imageData, currentImage.width, currentImage.height, currentImage.channels);
                        Fl_Image::RGB_scaling(FL_RGB_SCALING_BILINEAR);
                        small_ = (Fl_RGB_Image*)tmp.copy(sw, sh);
                        smallW_ = sw;
                    }
                    small_->draw(ix_, iy_);
                } else {
                    dx0_ = vx0; dy0_ = vy0;
                    fl_draw_image(sampleCb, this, vx0, vy0, vx1 - vx0, vy1 - vy0, 3);
                }
            }
        }
        fl_pop_clip();
    }

    int handle(int e) override {
        if (!imageLoaded) return Fl_Widget::handle(e);
        switch (e) {
        case FL_FOCUS: case FL_UNFOCUS: case FL_ENTER: case FL_LEAVE:
            return 1;
        case FL_PUSH:
            Fl::focus(this);
            if (Fl::event_clicks() > 0) { fit(); return 1; }
            lastX_ = Fl::event_x(); lastY_ = Fl::event_y();
            if (window()) window()->cursor(FL_CURSOR_MOVE);
            return 1;
        case FL_DRAG:
            offX_ += Fl::event_x() - lastX_;
            offY_ += Fl::event_y() - lastY_;
            lastX_ = Fl::event_x(); lastY_ = Fl::event_y();
            clampOffset();
            redraw();
            return 1;
        case FL_RELEASE:
            if (window()) window()->cursor(FL_CURSOR_DEFAULT);
            return 1;
        case FL_MOUSEWHEEL: {
            double f = (Fl::event_dy() < 0) ? 1.15 : 1.0 / 1.15;
            zoomAt(zoom_ * f, Fl::event_x() - x(), Fl::event_y() - y());
            return 1;
        }
        case FL_KEYBOARD: {
            const char* k = Fl::event_text();
            if (!k || !*k) return 0;
            if (*k == '+' || *k == '=') zoomAt(zoom_ * 1.25, w() / 2, h() / 2);
            else if (*k == '-')         zoomAt(zoom_ / 1.25, w() / 2, h() / 2);
            else if (*k == '0')         fit();
            else return 0;
            return 1;
        }
        }
        return Fl_Widget::handle(e);
    }
};

ImageView* view = nullptr;
void updateStatus()
{
    if (!statusBar) return;
    static char buf[600];
    if (!imageLoaded || !view) {
        snprintf(buf, sizeof(buf), "  No image loaded");
    } else {
        snprintf(buf, sizeof(buf), "  %d x %d   |   Zoom: %d%%   |   %s",
                 currentImage.width, currentImage.height,
                 (int)(view->zoom() * 100 + 0.5), currentFilename.c_str());
    }
    statusBar->copy_label(buf);
}

// ---------------------------------------------------------------
// Callbacks
// ---------------------------------------------------------------
void onLoadClicked(Fl_Widget*, void*)
{
    Fl_Native_File_Chooser chooser;
    chooser.title("Select an image to load");
    chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    chooser.filter("Images\t*.{jpg,jpeg,png,bmp,tga}");
    if (chooser.show() != 0) return;

    string path = chooser.filename();
    try {
        Image tmp;
        tmp.loadNewImage(path); 
        currentImage = tmp;
        currentFilename = path;
        imageLoaded = true;
        undoStack.clear();
        updateUndoButton();
        view->imageChanged(true);
    }
    catch (const exception& e) {
        fl_alert("Failed to load image:\n%s", e.what());
    }
}

void onSaveClicked(Fl_Widget*, void*)
{
    if (!imageLoaded) { fl_alert("Nothing to save yet."); return; }

    Fl_Native_File_Chooser chooser;
    chooser.title("Save image as");
    chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    chooser.options(Fl_Native_File_Chooser::SAVEAS_CONFIRM);
    chooser.filter("Images\t*.{jpg,jpeg,png,bmp,tga}");
    chooser.preset_file(currentFilename.c_str());
    if (chooser.show() != 0) return;

    string path = chooser.filename();

    size_t slash = path.find_last_of("/\\");
    size_t dot   = path.find_last_of('.');
    if (dot == string::npos || (slash != string::npos && dot < slash))
        path += ".png";

    try {
        currentImage.saveImage(path);
        currentFilename = path;
        updateStatus();
    }
    catch (const exception& e) {
        fl_alert("Failed to save image:\n%s", e.what());
    }
}

void onUndoClicked(Fl_Widget*, void*)
{
    if (undoStack.empty()) return;
    currentImage = undoStack.back();
    undoStack.pop_back();
    updateUndoButton();
    view->imageChanged(false);
}

void onFitClicked(Fl_Widget*, void*)     { view->fit(); }
void onZoomInClicked(Fl_Widget*, void*)  { view->zoomAt(view->zoom() * 1.25, view->w() / 2, view->h() / 2); }
void onZoomOutClicked(Fl_Widget*, void*) { view->zoomAt(view->zoom() / 1.25, view->w() / 2, view->h() / 2); }

bool applyFrame(Image& img)
{
    int c = fl_choice("Frame type:", "Cancel", "Simple", "Fancy");
    if (c == 0) return false;
    addFrame(img, c);
    return true;
}

bool applyFlip(Image& img)
{
    int c = fl_choice("Flip direction:", "Cancel", "Horizontal", "Vertical");
    if (c == 0) return false;
    flipImage(img, c == 1);
    return true;
}

bool applyRotate(Image& img)
{
    while (true) {
        const char* s = fl_input("Rotation angle (90, 180 or 270):", "90");
        if (!s) return false;

        string text = s;
        char* end = nullptr;
        long v = strtol(text.c_str(), &end, 10);

        if (end != text.c_str() && *end == '\0' && (v == 90 || v == 180 || v == 270)) {
            rotateImage(img, (int)v);
            return true;
        }
        fl_alert("Please enter 90, 180 or 270 only.");
    }
}

bool applyBrightness(Image& img)
{
    int c = fl_choice("Brightness:", "Cancel", "Darken", "Lighten");
    if (c == 0) return false;

    int p;
    if (!askIntInRange("Percentage (0 to 100):", 50, 0, 100, p)) return false;

    adjustBrightness(img, c == 2, p);
    return true;
}

bool applyResize(Image& img)
{
    int newW, newH;
    if (!askIntInRange("New width (1 - 10000):",  img.width,  1, 10000, newW)) return false;
    if (!askIntInRange("New height (1 - 10000):", img.height, 1, 10000, newH)) return false;

    resizeImage(img, newW, newH);
    return true;
}

bool applyMerge(Image& img)
{
    Fl_Native_File_Chooser chooser;
    chooser.title("Select the second image to merge");
    chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    chooser.filter("Images\t*.{jpg,jpeg,png,bmp,tga}");
    if (chooser.show() != 0) return false;

    string path = chooser.filename();
    Image img2;
    try {
        img2.loadNewImage(path);
    }
    catch (const exception& e) {
        fl_alert("Failed to load the second image:\n%s", e.what());
        return false;
    }

    bool scaleUp = true;
    if (img.width != img2.width || img.height != img2.height) {
        int c = fl_choice("The images have different sizes:", "Cancel", "Scale up", "Crop");
        if (c == 0) return false;
        scaleUp = (c == 1);
    }

    mergeImages(img, img2, scaleUp);
    return true;
}

bool askIntInRange(const string& prompt, int def, int lo, int hi, int& out)
{
    while (true) {
        string d = to_string(def);
        const char* s = fl_input("%s", d.c_str(), prompt.c_str());
        if (!s) return false;
        string text = s;
        char* end = nullptr;
        long v = strtol(text.c_str(), &end, 10);
        if (end == text.c_str() || *end != '\0' || v < lo || v > hi) {
            fl_alert("Please enter a whole number from %d to %d.", lo, hi);
            continue;
        }
        out = (int)v;
        return true;
    }
}

bool applyCrop(Image& img)
{
    string size = "Image size: " + to_string(img.width) + " x " + to_string(img.height) + " pixels\n\n";
    int x, y, W, H;

    if (!askIntInRange(size + "Step 1/4 - Left edge of the crop area\n(0 = left side of the image):",
                       0, 0, img.width - 1, x)) return false;

    if (!askIntInRange(size + "Step 2/4 - Top edge of the crop area\n(0 = top of the image):",
                       0, 0, img.height - 1, y)) return false;

    if (!askIntInRange(size + "Step 3/4 - Width of the crop area\n(how many pixels to keep, to the right of the left edge):",
                       img.width - x, 1, img.width - x, W)) return false;

    if (!askIntInRange(size + "Step 4/4 - Height of the crop area\n(how many pixels to keep, below the top edge):",
                       img.height - y, 1, img.height - y, H)) return false;

    cropImage(img, x, y, W, H);
    return true;
}

bool applyBlur(Image& img)
{
    int maxLevel = min(10, (min(img.width, img.height) - 1) / 2);
    if (maxLevel < 1) { fl_alert("The image is too small to blur."); return false; }

    int level;
    if (!askIntInRange("Blur level (1 = light, " + to_string(maxLevel) + " = strongest):",
                       min(2, maxLevel), 1, maxLevel, level)) return false;

    blurWithLevel(img, level);
    return true;
}

bool applySkew(Image& img)
{
    int angle;
    if (!askIntInRange("Skew angle in degrees (1 = slight, 75 = extreme):", 40, 1, 75, angle))
        return false;

    double t = tan(angle * 3.14159265358979 / 180.0);
    if (img.width + img.height * t > 10000) {
        fl_alert("This angle makes the image too wide for this image size.\nTry a smaller angle.");
        return false;
    }
    skewImage(img, angle);
    return true;
}

void onApplyClicked(Fl_Widget*, void*)
{
    if (!imageLoaded) { fl_alert("Load an image first."); return; }

    undoStack.push_back(currentImage);

    bool applied = false;
    bool failed = false;
    try {
        switch (filterChoice->value() + 1) {
            case 1: Grayscaleflow(currentImage); applied = true; break;
            case 2: BlackandWhiteflow(currentImage); applied = true; break;
            case 3: invertColorsflow(currentImage); applied = true; break;
            case 4: applied = applyFrame(currentImage); break;
            case 5: applied = applyFlip(currentImage); break;
            case 6: applied = applyRotate(currentImage); break;
            case 7: applied = applyBrightness(currentImage); break;
            case 8: applied = applyResize(currentImage); break;
            case 9: applied = applyMerge(currentImage); break;
            case 10: detectEdges(currentImage); applied = true; break;
            case 11: applied = applyCrop(currentImage); break;
            case 12: applied = applyBlur(currentImage); break;
            case 13: wanoSunlightFlow(currentImage); applied = true; break;
            case 14: oldTV(currentImage); applied = true; break;
            case 15: purpleFilter(currentImage); applied = true; break;
            case 16: infrared(currentImage); applied = true; break;
            case 17: applied = applySkew(currentImage); break;
            case 18: oilPaint(currentImage); applied = true; break;
            default: break;
        }
    }
    catch (const exception& e) {
        failed = true;
        fl_alert("Filter failed:\n%s", e.what());
    }

    if (failed) {                              
        currentImage = undoStack.back();
        undoStack.pop_back();
        updateUndoButton();
        view->imageChanged(false);
        return;
    }
    if (!applied) { undoStack.pop_back(); updateUndoButton(); return; }

    while (undoStack.size() > MAX_UNDO ||
           (undoStack.size() > 1 && undoBytes() > MAX_UNDO_BYTES))
        undoStack.erase(undoStack.begin());

    updateUndoButton();
    view->imageChanged(false);
}

// ---------------------------------------------------------------
// Window layout
// ---------------------------------------------------------------
static Fl_Button* makeButton(int x, int w, const char* label, Fl_Callback* cb)
{
    Fl_Button* b = new Fl_Button(x, 7, w, 30, label);
    b->labelsize(13);
    b->callback(cb);
    return b;
}

int main()
{
    Fl::scheme("gtk+");
    Fl::background(50, 50, 54);
    Fl::background2(35, 35, 38);
    Fl::foreground(235, 235, 235);
    Fl::set_color(FL_SELECTION_COLOR, 0, 122, 204);

    const int WIN_W = 1000, WIN_H = 720, BAR_H = 44, STATUS_H = 26;

    Fl_Double_Window* window = new Fl_Double_Window(WIN_W, WIN_H, "CS213 - Image Processing");

    Fl_Group* bar = new Fl_Group(0, 0, WIN_W, BAR_H);
    bar->box(FL_FLAT_BOX);
    bar->color(fl_rgb_color(40, 40, 44));

    makeButton(10,  80, "Open", onLoadClicked);          
    makeButton(98,  80, "Save", onSaveClicked);       

    undoBtn = makeButton(186, 80, "Undo", onUndoClicked);
    undoBtn->shortcut(FL_CTRL + 'z');
    undoBtn->tooltip("Undo the last filter (Ctrl+Z)");
    undoBtn->shortcut(FL_COMMAND + 'z');
    undoBtn->tooltip("Undo the last filter (Cmd+Z)");

    filterChoice = new Fl_Choice(282, 7, 210, 30);
    filterChoice->labelsize(13);
    filterChoice->textsize(13);
    filterChoice->add("Grayscale");
    filterChoice->add("Black and White");
    filterChoice->add("Invert");
    filterChoice->add("Frame");
    filterChoice->add("Flip");
    filterChoice->add("Rotate");
    filterChoice->add("Darken and Lighten");
    filterChoice->add("Resize");
    filterChoice->add("Merge");
    filterChoice->add("Edge Detection");
    filterChoice->add("Crop");
    filterChoice->add("Blur");
    filterChoice->add("Wano Sunlight");
    filterChoice->add("Old TV");
    filterChoice->add("Wano Night");
    filterChoice->add("Infrared");
    filterChoice->add("Skew");
    filterChoice->add("Oil Painting");
    filterChoice->value(0);
    
    makeButton(500, 80, "Apply", onApplyClicked); 
    makeButton(600, 60, "Fit", onFitClicked);   
    makeButton(668, 36, "-",   onZoomOutClicked); 
    makeButton(712, 36, "+",   onZoomInClicked); 
    bar->end();

    view = new ImageView(0, BAR_H, WIN_W, WIN_H - BAR_H - STATUS_H);
    statusBar = new Fl_Box(0, WIN_H - STATUS_H, WIN_W, STATUS_H);
    statusBar->box(FL_FLAT_BOX);
    statusBar->color(fl_rgb_color(40, 40, 44));
    statusBar->labelsize(12);
    statusBar->align(FL_ALIGN_LEFT | FL_ALIGN_INSIDE);
    updateStatus();
    updateUndoButton();

    window->end();
    window->resizable(view);
    window->size_range(760, 480);
    window->show();

    return Fl::run();
}
