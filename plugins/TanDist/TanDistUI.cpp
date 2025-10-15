#include "TanDistUI.hpp"
#include "TanDistParameters.h"
#include <cmath>
#include <algorithm>

START_NAMESPACE_DISTRHO

// UI dimensions
static constexpr uint kUIWidth = 400;
static constexpr uint kUIHeight = 300;

TanDistUI::TanDistUI()
    : UI(kUIWidth, kUIHeight)
{
    // Initialize parameter ranges from shared definitions
    mGainMin = TanDistParams::kGainMin;
    mGainMax = TanDistParams::kGainMax;

    // Setup control positions
    setupControls();

    // Initial value will be set via parameterChanged() by the host

    // Visage will be initialized in uiIdle() when window is ready
}

TanDistUI::~TanDistUI()
{
    if (mVisageApp)
    {
        mVisageApp->close();
        mVisageApp.reset();
    }
}

void TanDistUI::setupControls()
{
    // Position gain knob in center of UI
    mGainKnob.x = kUIWidth * 0.5f;
    mGainKnob.y = kUIHeight * 0.5f;
    mGainKnob.radius = 40.0f;
    mGainKnob.paramIndex = kGain;
}

void TanDistUI::uiIdle()
{
    // Initialize Visage once DPF window is ready
    if (!mVisageInitialized)
    {
        uintptr_t handle = getWindow().getNativeWindowHandle();
        if (handle != 0)
        {
            initializeVisage();
            mVisageInitialized = true;
            // Force initial draw
            if (mVisageApp)
            {
                mVisageApp->redraw();
            }
        }
    }

    // Process Visage events (needed on Linux for X11 events)
    if (mVisageApp && mVisageApp->window())
    {
        mVisageApp->window()->processPluginFdEvents();
    }
}

void TanDistUI::initializeVisage()
{
    // Create Visage application window
    mVisageApp = std::make_unique<visage::ApplicationWindow>();
    mVisageApp->setWindowDimensions(getWidth(), getHeight());

    // Initialize font using Visage's embedded Lato font
    mFont = visage::Font(14, visage::fonts::Lato_Regular_ttf);

    setupVisageDrawing();

    // Attach to DPF's native window handle
    uintptr_t parentWindow = getWindow().getNativeWindowHandle();
    if (parentWindow)
    {
        mVisageApp->show(reinterpret_cast<void *>(parentWindow));
    }
}

void TanDistUI::setupVisageDrawing()
{
    mVisageApp->onDraw() = [this](visage::Canvas &canvas)
    {
        // Clear background with dark color
        canvas.setColor(0xff2a2a2a);
        canvas.fill(0, 0, mVisageApp->width(), mVisageApp->height());

        // Draw gain knob
        drawKnob(canvas, mGainKnob, mGainValue);

        // Draw parameter label below knob
        canvas.setColor(0xffcccccc);
        float labelY = mGainKnob.y + mGainKnob.radius + 20;
        canvas.text(TanDistParams::kGainName, mFont, visage::Font::kCenter,
                    mGainKnob.x - 50, labelY, 100, 20);

        // Draw parameter value
        char valueText[32];
        float actualValue = mGainMin + mGainValue * (mGainMax - mGainMin);
        snprintf(valueText, sizeof(valueText), "%.2f", actualValue);
        canvas.setColor(0xffffffff);
        canvas.text(valueText, mFont, visage::Font::kCenter,
                    mGainKnob.x - 50, labelY + 20, 100, 20);
    };

    // Handle mouse events in Visage
    mVisageApp->onMouseDown() = [this](const visage::MouseEvent &e)
    {
        if (e.isLeftButton())
        {
            if (hitTestKnob(mGainKnob, e.position.x, e.position.y))
            {
                mDraggingKnob = true;
                editParameter(kGain, true);
                mDragStartValue = mGainValue;
                mDragStartY = e.position.y;
                mVisageApp->redraw();
            }
        }
    };

    mVisageApp->onMouseUp() = [this](const visage::MouseEvent &e)
    {
        if (mDraggingKnob && e.isLeftButton())
        {
            editParameter(kGain, false);
            mDraggingKnob = false;
            mVisageApp->redraw();
        }
    };

    mVisageApp->onMouseDrag() = [this](const visage::MouseEvent &e)
    {
        if (mDraggingKnob)
        {
            float normalizedValue = calculateKnobValue(e.position.y);
            // Update UI immediately
            mGainValue = normalizedValue;
            // Denormalize from UI range (0.0-1.0) to plugin range
            float pluginValue = mGainMin + normalizedValue * (mGainMax - mGainMin);
            setParameterValue(kGain, pluginValue);
            mVisageApp->redraw();
        }
    };
}

void TanDistUI::drawKnob(visage::Canvas &canvas, const KnobControl &knob, float value)
{
    constexpr float kStartAngleDeg = -220.0f;
    constexpr float kAngleRangeDeg = 260.0f;

    float currentAngleDeg = kStartAngleDeg + (value * kAngleRangeDeg);
    float currentAngleRad = currentAngleDeg * M_PI / 180.0f;

    // Draw range indicator arc (thin line showing available range)
    float arcRadius = knob.radius + 8;
    float centerAngleRad = (kStartAngleDeg + kAngleRangeDeg * 0.5f) * M_PI / 180.0f;
    float halfSweepRad = (kAngleRangeDeg * 0.5f) * M_PI / 180.0f;

    canvas.setColor(0x88cccccc); // Semi-transparent gray
    canvas.arc(knob.x - arcRadius, knob.y - arcRadius,
               arcRadius * 2.0f, 2.0f, centerAngleRad, halfSweepRad, true);

    // Draw knob body (circle)
    canvas.setColor(0xff505050);
    canvas.circle(knob.x - knob.radius, knob.y - knob.radius, knob.radius * 2.0f);

    // Draw knob indicator dot at the edge
    float indicatorLength = knob.radius * 0.75f;
    float endX = knob.x + std::cos(currentAngleRad) * indicatorLength;
    float endY = knob.y + std::sin(currentAngleRad) * indicatorLength;

    canvas.setColor(0xffffffff); // White indicator
    canvas.circle(endX - 3.5f, endY - 3.5f, 7.0f);

    // Highlight if being dragged
    if (mDraggingKnob)
    {
        canvas.setColor(0x3300ff88); // Semi-transparent green glow
        canvas.circle(knob.x - knob.radius - 4, knob.y - knob.radius - 4,
                      (knob.radius + 4) * 2.0f);
    }
}

void TanDistUI::parameterChanged(uint32_t index, float value)
{
    if (index == kGain)
    {
        // Normalize from plugin range to UI range (0.0-1.0)
        mGainValue = (value - mGainMin) / (mGainMax - mGainMin);

        // Trigger Visage redraw
        if (mVisageApp)
        {
            mVisageApp->redraw();
        }
    }
}

void TanDistUI::onDisplay()
{
    // Visage handles its own rendering
}

bool TanDistUI::onMouse(const MouseEvent &ev)
{
    // Visage handles its own mouse events
    return false;
}

void TanDistUI::onResize(const ResizeEvent &ev)
{
    if (mVisageApp)
    {
        mVisageApp->setWindowDimensions(ev.size.getWidth(), ev.size.getHeight());
    }
}

bool TanDistUI::hitTestKnob(const KnobControl &knob, float x, float y)
{
    float dx = x - knob.x;
    float dy = y - knob.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSquared = knob.radius * knob.radius;

    return distanceSquared <= radiusSquared;
}

float TanDistUI::calculateKnobValue(int mouseY)
{
    // Vertical drag: up = increase, down = decrease
    float deltaY = mDragStartY - mouseY;
    float sensitivity = 0.005f; // Adjust for feel
    float newValue = mDragStartValue + (deltaY * sensitivity);

    // Clamp to 0.0 - 1.0 range
    return std::clamp(newValue, 0.0f, 1.0f);
}

// DPF UI factory function
UI *createUI()
{
    return new TanDistUI();
}

END_NAMESPACE_DISTRHO
