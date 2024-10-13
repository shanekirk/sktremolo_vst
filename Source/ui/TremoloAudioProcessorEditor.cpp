// MIT License
//
// Copyright(C), 2024, Shane Kirk

#include "ui/TremoloAudioProcessorEditor.h"
#include "ui/Version.h"
#include "dsp/TremoloAudioProcessor.h"

/**
 * @since 2024 Sept 25
 */
TremoloAudioProcessorEditor::TremoloAudioProcessorEditor(juce::AudioProcessor &audioProcessor, AudioParameters &audioParams) :
    AudioProcessorEditor(&audioProcessor),
    m_audioParams(audioParams)
{
    // Title Drawable
    m_spTitleDrawable = createTitleDrawable(foregroundColor);
    addAndMakeVisible(*m_spTitleDrawable);

    // About Button
    m_spAboutButton = createAboutButton(this, foregroundColor);
    addAndMakeVisible(*m_spAboutButton);

    // Knobs
    m_spSpeedKnob = createKnob(*m_audioParams.m_pSpeedParamInHz);
    addAndMakeVisible(*m_spSpeedKnob);

    m_spDepthKnob = createKnob(*m_audioParams.m_pDepthParamInPercent);
    addAndMakeVisible(*m_spDepthKnob);

    m_spGainKnob = createKnob(*m_audioParams.m_pGainParamInPercent);
    addAndMakeVisible(*m_spGainKnob);

    // Labels
    m_spSpeedLabel = createKnobLabel("Speed", *m_spSpeedKnob);
    addAndMakeVisible(*m_spSpeedLabel);

    m_spDepthLabel = createKnobLabel("Depth", *m_spDepthKnob);
    addAndMakeVisible(*m_spDepthLabel);

    m_spGainLabel = createKnobLabel("Gain", *m_spGainKnob);
    addAndMakeVisible(*m_spGainLabel);

    // Wave Form Buttons
    m_spSineButton = createWaveFormButton(0, *m_audioParams.m_pOscillatorType);
    addAndMakeVisible(*m_spSineButton);

    m_spSquareButton = createWaveFormButton(1, *m_audioParams.m_pOscillatorType);
    addAndMakeVisible(*m_spSquareButton);

    m_spTriangleButton = createWaveFormButton(2, *m_audioParams.m_pOscillatorType);
    addAndMakeVisible(*m_spTriangleButton);

    m_audioParams.addListener(this);

    // Window size
    setSize(600, 350);
}

/**
 * @since 2024 Oct 8
 */
TremoloAudioProcessorEditor::~TremoloAudioProcessorEditor()
{
    m_audioParams.removeListener(this);
}

/**
 * @since 2024 Sept 25
 */
void TremoloAudioProcessorEditor::paint(juce::Graphics &g)
{
    // Background Color
    auto bounds = getLocalBounds();
    auto backgroundGradient = juce::ColourGradient(
        backgroundGradientTop, 0.0f, 0.0f,
        backgroundGradientBottom, 0.0f, static_cast<float>(bounds.getHeight()), false);
    g.setGradientFill(backgroundGradient);
    g.fillAll();

    // Window Border
    const float leftMargin = 25.0f;
    const float topMargin = 38.0f;
    const float rightMargin = 25.0f;
    const float bottomMargin = 25.0f;
    const float rectWidth = bounds.getWidth() - (leftMargin + rightMargin);
    const float rectHeight = bounds.getHeight() - (topMargin + bottomMargin);

    g.setColour(foregroundColor);

    auto clipRect = m_titleRect;
    clipRect.expand(2, 0);
    g.excludeClipRegion(clipRect); // So we don't overlap with the title graphic
    g.drawRoundedRectangle(leftMargin, topMargin,rectWidth, rectHeight, 15.0f, 2.0f);
}

/**
 * @since 2024 Sept 25
 */
void TremoloAudioProcessorEditor::resized()
{
    const auto bounds = getLocalBounds();

    // Title
    m_spTitleDrawable->setTransformToFit(m_titleRect.toFloat(), juce::RectanglePlacement::onlyReduceInSize);

    // Knobs
    const int knobWidth = 150;
    const int knobHeight = 150;
    const int knobYPos = bounds.getCentreY() - (knobHeight / 2) - 20;
    int knobXPos = 50;
    m_spSpeedKnob->setBounds(knobXPos, knobYPos, knobWidth, knobHeight);

    knobXPos = bounds.getCentreX() - (knobWidth / 2);
    m_spDepthKnob->setBounds(knobXPos, knobYPos, knobWidth, knobHeight);

    knobXPos = bounds.getRight() - 50 - knobWidth;
    m_spGainKnob->setBounds(knobXPos, knobYPos, knobWidth, knobHeight);

    // Buttons.
    const int buttonWidth = 75;
    const int buttonHeight = 50;
    const int buttonY = knobYPos + knobHeight + 20;
    const int buttonX = bounds.getCentreX() - (buttonWidth / 2);
    m_spSquareButton->setBounds(buttonX, buttonY, buttonWidth, buttonHeight);
    m_spSineButton->setBounds(buttonX - (buttonWidth + 20), buttonY, buttonWidth, buttonHeight);
    m_spTriangleButton->setBounds(buttonX + (buttonWidth + 20), buttonY, buttonWidth, buttonHeight);

    m_spAboutButton->setBounds(bounds.getWidth() - 34, 8, 26, 26);
}

/**
 * @since 2024 Oct 8
 */
void TremoloAudioProcessorEditor::parameterValueChanged(int parameterIndex, float newValue)
{
    switch (parameterIndex)
    {
        case 0:
        {
            float speed = m_audioParams.m_pSpeedParamInHz->convertFrom0to1(newValue);
            m_spSpeedKnob->setValue(speed, juce::NotificationType::dontSendNotification);
            break;
        }
        case 1:
        {
            float depth = m_audioParams.m_pDepthParamInPercent->convertFrom0to1(newValue);
            m_spDepthKnob->setValue(depth, juce::NotificationType::dontSendNotification);
            break;
        }
        case 2:
        {
            float gain = m_audioParams.m_pGainParamInPercent->convertFrom0to1(newValue);
            m_spGainKnob->setValue(gain, juce::NotificationType::dontSendNotification);
            break;
        }
        case 3:
        {
            int waveFormType = static_cast<int>(m_audioParams.m_pOscillatorType->convertFrom0to1(newValue));
            switch (waveFormType)
            {
                case 1: m_spSquareButton->setToggleState(true, juce::NotificationType::dontSendNotification); break;
                case 2: m_spTriangleButton->setToggleState(true, juce::NotificationType::dontSendNotification); break;
                default: m_spSineButton->setToggleState(true, juce::NotificationType::dontSendNotification); break;
            }
            break;
        }
    }
}

/**
 * @since 2024 Oct 11
 */
std::unique_ptr<Knob> TremoloAudioProcessorEditor::createKnob(juce::AudioParameterFloat &param)
{
    auto normalizableRange = param.getNormalisableRange();
    auto range = normalizableRange.getRange();
    auto interval = normalizableRange.interval;

    std::unique_ptr<Knob> spKnob = std::make_unique<Knob>();
    spKnob->setRange({ range.getStart(), range.getEnd() }, interval);
    Knob *pKnob = spKnob.get(); // For the benefit of the lambda.
    spKnob->onValueChange = [&, pKnob]()
    {
        param = static_cast<float>(pKnob->getValue());
    };
    spKnob->setValue(param, juce::NotificationType::dontSendNotification);
    return spKnob;
}

/**
 * @since 2024 Oct 11
 */
std::unique_ptr<juce::Label> TremoloAudioProcessorEditor::createKnobLabel(juce::String text, Knob &knob)
{
    std::unique_ptr<juce::Label> spLabel = std::make_unique<juce::Label>();
    spLabel->setText(text, juce::NotificationType::dontSendNotification);
    spLabel->setJustificationType(juce::Justification::horizontallyCentred);
    spLabel->attachToComponent(&knob, false);
    return spLabel;
}

/**
 * Creates the "About" button that's displayed in the top right corner.
 */
std::unique_ptr<juce::DrawableButton> TremoloAudioProcessorEditor::createAboutButton(juce::AudioProcessorEditor *pParent, const juce::Colour &fgColor)
{
    auto spAboutXml = juce::XmlDocument::parse(BinaryData::about_svg);
    if (!spAboutXml)
        throw std::runtime_error("Error parsing About icon SVG.");

    auto spAboutUnpressedDrawable = juce::Drawable::createFromSVG(*spAboutXml);
    if (!spAboutUnpressedDrawable)
        throw std::runtime_error("Could not create SVG from About icon XML.");

    // "Released" image.
    spAboutUnpressedDrawable->replaceColour(juce::Colours::white, fgColor);
    spAboutUnpressedDrawable->setTransformToFit({ 0.0, 0.0, 25.0, 25.0 }, juce::RectanglePlacement::stretchToFit);

    // "Pressed" image.
    auto spAboutPressedDrawable = spAboutUnpressedDrawable->createCopy();
    spAboutPressedDrawable->setTransformToFit({ 1.0, 1.0, 23.0, 23.0 }, juce::RectanglePlacement::stretchToFit);

    auto spAboutButton = std::make_unique<juce::DrawableButton>("About...", juce::DrawableButton::ButtonStyle::ImageRaw);
    spAboutButton->setImages(spAboutUnpressedDrawable.get(), nullptr, spAboutPressedDrawable.get());
    spAboutButton->onClick = [pParent]()
    {
        juce::String message = "Version ";
        message += VERSION_STR;
        message += "\n\n";
        message += COPYRIGHT_STR;
        message += "\n\nVisit http://www.shanekirk.com/2024/10/sktremolo/ to learn more!";
        juce::AlertWindow::showMessageBoxAsync(juce::MessageBoxIconType::NoIcon, "SKTremolo", message, "Close", pParent);
    };

    return spAboutButton;
}

/**
 * @since 2024 Oct 11
 */
std::unique_ptr<juce::Drawable> TremoloAudioProcessorEditor::createTitleDrawable(const juce::Colour &fgColor)
{
    auto spTitleXml = juce::XmlDocument::parse(BinaryData::title_svg);
    if (!spTitleXml)
        throw std::runtime_error("Error parsing title SVG.");

    auto spDrawable = juce::Drawable::createFromSVG(*spTitleXml);
    if (!spDrawable)
        throw std::runtime_error("Could not create SVG from title graphics XML.");

    spDrawable->replaceColour(juce::Colours::black, fgColor);
    return spDrawable;
}

/**
 * @since 2024 Oct 11
 */
std::unique_ptr<IconButton> TremoloAudioProcessorEditor::createWaveFormButton(int waveFormId, juce::AudioParameterChoice &param)
{
    if ((waveFormId < 0) || (waveFormId >= param.choices.size()))
        return nullptr;

    std::unique_ptr<IconButton> spButton = std::make_unique<IconButton>();
    spButton->setRadioGroupId(RADIO_BUTTON_GROUP_ID);
    spButton->onClick = [&param, waveFormId]() { param = waveFormId; };
    spButton->setToggleState(param == waveFormId, juce::NotificationType::dontSendNotification);

    std::unique_ptr<juce::XmlElement> spSVGData;
    if (param.choices[waveFormId] == "Sine")
    {
        spSVGData = juce::XmlDocument::parse(BinaryData::sine_wave_svg);
    }
    else
    if (param.choices[waveFormId] == "Square")
    {
        spSVGData = juce::XmlDocument::parse(BinaryData::square_wave_svg);
    }
    else
    if (param.choices[waveFormId] == "Triangle")
    {
        spSVGData = juce::XmlDocument::parse(BinaryData::triangle_wave_svg);
    }

    if (spSVGData)
    {
        auto spIcon = juce::Drawable::createFromSVG(*spSVGData);
        const juce::Colour iconColor{ 227, 227, 221 };
        spIcon->replaceColour(juce::Colours::white, iconColor);
        spButton->setIcon(std::move(spIcon));
    }

    return spButton;
}
