/*
** Surge Synthesizer is Free and Open Source Software
**
** Surge is made available under the Gnu General Public License, v3.0
** https://www.gnu.org/licenses/gpl-3.0.en.html
**
** Copyright 2004-2021 by various individuals as described by the Git transaction log
**
** All source at: https://github.com/surge-synthesizer/surge.git
**
** Surge was a commercial product from 2004-2018, with Copyright and ownership
** in that period held by Claes Johanson at Vember Audio. Claes made Surge
** open source in September 2018.
*/

#ifndef SURGE_XT_MULTISWITCH_H
#define SURGE_XT_MULTISWITCH_H

#include "SurgeJUCEHelpers.h"
#include "WidgetBaseMixin.h"

#include "juce_gui_basics/juce_gui_basics.h"

class SurgeImage;
class SurgeStorage;

namespace Surge
{
namespace Widgets
{
/*
 * MultiSwitch (f.k.a CHSwitch2 in VSTGUI land) takes a
 * glyph with rows and columns to allow multiple selection
 */
struct MultiSwitch : public juce::Component, public WidgetBaseMixin<MultiSwitch>
{
    MultiSwitch();
    ~MultiSwitch();

    SurgeStorage *storage{nullptr};
    void setStorage(SurgeStorage *s) { storage = s; }

    int rows{0}, columns{0}, heightOfOneImage{0}, frameOffset{0};
    int getRows() const { return rows; }
    int getColumns() const { return columns; }
    void setRows(int x) { rows = x; }
    void setColumns(int x) { columns = x; }
    void setHeightOfOneImage(int h) { heightOfOneImage = h; }
    void setFrameOffset(int h) { frameOffset = h; }

    int valueToOff(float v)
    {
        return (int)(frameOffset + ((v * (float)(rows * columns - 1) + 0.5f)));
    }
    int coordinateToSelection(int x, int y);
    float coordinateToValue(int x, int y);

    float value{0};
    float getValue() const override { return value; }
    int getIntegerValue() const { return (int)(value * (float)(rows * columns - 1) + 0.5f); }
    void setValue(float f) override { value = f; }

    bool draggable{false};
    void setDraggable(bool d) { draggable = d; }

    void paint(juce::Graphics &g) override;

    bool everDragged{false}, isMouseDown{false};
    void mouseDown(const juce::MouseEvent &event) override;
    void mouseEnter(const juce::MouseEvent &event) override;
    void mouseExit(const juce::MouseEvent &event) override;
    void mouseMove(const juce::MouseEvent &event) override;
    void mouseDrag(const juce::MouseEvent &event) override;
    void mouseUp(const juce::MouseEvent &event) override;
    void setCursorToArrow();

    void startHover(const juce::Point<float> &) override;
    void endHover() override;

    void focusGained(juce::Component::FocusChangeType cause) override
    {
        // fixme - probably use the location of the current element
        startHover(getBounds().getBottomLeft().toFloat());
        repaint();
    }
    void focusLost(juce::Component::FocusChangeType cause) override
    {
        endHover();
        repaint();
    }
    Surge::GUI::WheelAccumulationHelper wheelHelper;
    void mouseWheelMove(const juce::MouseEvent &event,
                        const juce::MouseWheelDetails &wheel) override;

    bool isHovered{false};
    int hoverSelection{0};

    SurgeImage *switchD{nullptr}, *hoverSwitchD{nullptr}, *hoverOnSwitchD{nullptr};
    void setSwitchDrawable(SurgeImage *d) { switchD = d; }
    void setHoverSwitchDrawable(SurgeImage *d) { hoverSwitchD = d; }
    void setHoverOnSwitchDrawable(SurgeImage *d) { hoverOnSwitchD = d; }

    void setupAccessibility();
#if SURGE_JUCE_ACCESSIBLE
    std::vector<std::unique_ptr<juce::Component>> selectionComponents;
    std::unique_ptr<juce::AccessibilityHandler> createAccessibilityHandler() override;
#endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiSwitch);
};

struct MultiSwitchSelfDraw : public MultiSwitch
{
    MultiSwitchSelfDraw() : MultiSwitch() {}
    void paint(juce::Graphics &g) override;

    std::vector<std::string> labels;
    void setLabels(const std::vector<std::string> &l)
    {
        labels = l;
        repaint();
    }

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MultiSwitchSelfDraw);
};
} // namespace Widgets
} // namespace Surge
#endif // SURGE_XT_MULTISWITCH_H
