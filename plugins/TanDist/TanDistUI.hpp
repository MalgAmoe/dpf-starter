#ifndef TANDIST_UI_HPP_INCLUDED
#define TANDIST_UI_HPP_INCLUDED

#include "DistrhoUI.hpp"
#include "DistrhoPluginInfo.h"
#include "Window.hpp"
#include <visage/app.h>
#include <embedded/fonts.h>
#include <memory>

START_NAMESPACE_DISTRHO

class TanDistUI : public UI
{
public:
    TanDistUI();
    ~TanDistUI();

protected:
    // DPF UI interface
    void parameterChanged(uint32_t index, float value) override;
    void onDisplay() override;
    bool onMouse(const MouseEvent &ev) override;
    void uiIdle() override;
    void onResize(const ResizeEvent &ev) override;

private:
    // Visage integration
    std::unique_ptr<visage::ApplicationWindow> mVisageApp;
    bool mVisageInitialized = false;
    visage::Font mFont;

    // Control structures
    struct KnobControl
    {
        float x, y;          // Center position
        float radius;        // Knob radius
        uint32_t paramIndex; // Which parameter this controls
    };

    KnobControl mGainKnob;

    // Parameter state
    float mGainValue = 0.5f; // 0.0 to 1.0 normalized
    float mGainMin = 1.0f;
    float mGainMax = 10.0f;

    // Interaction state
    bool mDraggingKnob = false;
    float mDragStartValue = 0.0f;
    int mDragStartY = 0;

    // Setup methods
    void initializeVisage();
    void setupVisageDrawing();
    void setupControls();

    // Drawing methods
    void drawKnob(visage::Canvas &canvas, const KnobControl &knob, float value);

    // Interaction
    bool hitTestKnob(const KnobControl &knob, float x, float y);
    float calculateKnobValue(int mouseY);

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TanDistUI)
};

END_NAMESPACE_DISTRHO

#endif // TANDIST_UI_HPP_INCLUDED
